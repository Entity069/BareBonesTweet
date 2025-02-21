#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <vector>
#include "./include/server.h"
#include "./include/inja/inja.hpp"

using namespace MicroWeb;
using json = nlohmann::json;

struct Tweet {
    int id;
    std::string tweet_content;
    std::string timestamp;
    int likes;
};

std::string loadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    Server server;
    inja::Environment env;


    // Basic route
    server.route("GET", "/tweets", [](Request& req, Response& res) {

        res.setContentType(MIME_HTML);

        sqlite3* db;
        sqlite3_open("tweets.db", &db);

        std::string query = "SELECT id, content, timestamp, likes FROM tweets";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

        std::vector<Tweet> tweets;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Tweet tweet;
            tweet.id = sqlite3_column_int(stmt, 0);
            tweet.tweet_content = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            tweet.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            tweet.likes = sqlite3_column_int(stmt, 3);
            tweets.push_back(tweet);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        inja::Environment env;
        inja::json context;
        for (const auto& tweet : tweets) {
            context["tweets"].push_back({
                {"id", tweet.id},
                {"tweet_content", tweet.tweet_content},
                {"timestamp", tweet.timestamp},
                {"likes", tweet.likes}
            });
        }

        std::string template_str = loadFile("./templates/home.html");
        std::string rendered = env.render(template_str, context);

        res.setBody(rendered);

    });

    server.route("GET", "/create-tweet", [](Request& req, Response& res) {
        res.setContentType(MIME_HTML);
        std::string template_str = loadFile("./templates/create.html");
        res.setBody(template_str);
    });

    server.route("POST", "/create", [](Request& req, Response& res) {
        res.setContentType(MIME_JSON);
        json body = req.getJsonBody();
        std::string tweet_content = (body["tweet_content"]);

        sqlite3* db;
        sqlite3_open("tweets.db", &db);
        std::string query = "INSERT INTO tweets (content) VALUES ('" + tweet_content + "')";
        sqlite3_stmt* stmt;

        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            res.setStatus(500);
            res.json("{'status': '500', 'msg': 'Erorr while creating tweet'}");
            return;
        } else {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            res.setStatus(200);
            res.json("{'status': '200', 'msg': 'Tweet created successfully'}");
        }
    });

    server.route("POST", "/like", [](Request& req, Response& res) {
        res.setContentType(MIME_JSON);
        json body = req.getJsonBody();
        std::string id = (body["id"]);

        sqlite3* db;
        sqlite3_open("tweets.db", &db);
        std::string query = "UPDATE tweets SET likes = likes + 1 WHERE id = " + id;
        sqlite3_stmt* stmt;

        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            res.setStatus(500);
            res.setBody("{'status': '500', 'msg': 'Error while liking tweet'}");
            return;
        } else {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            res.setStatus(200);
            res.json("{'status': '200', 'msg': 'Tweet liked successfully'}");
        }
    });

    server.use("cookie_parser", Middleware::cookieMiddleware);

    server.start();

    return 0;
}