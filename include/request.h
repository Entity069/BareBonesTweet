#ifndef MICROWEB_REQUEST_H
#define MICROWEB_REQUEST_H

#include <string>
#include <unordered_map>
#include "./nlohmann/json.hpp"

namespace MicroWeb {
class Request {
private:
    std::string method;
    std::string path;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> query_params;
    std::unordered_map<std::string, std::string> cookies;

public:
    Request();

    void parse(const std::string& raw_request);

    std::string getMethod() const;
    std::string getPath() const;
    std::string getBody() const;
    std::string getHeader(const std::string& key) const;
    std::string getQueryParam(const std::string& key) const;
    std::string getCookie(const std::string& key) const;

    nlohmann::json getJsonBody() const;

    void setMethod(const std::string& m);
    void setPath(const std::string& p);
    void addHeader(const std::string& key, const std::string& value);
    void addQueryParam(const std::string& key, const std::string& value);
    void addCookie(const std::string& key, const std::string& value);

    ~Request();
};
}

#endif // MICROWEB_REQUEST_H