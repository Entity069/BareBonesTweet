#ifndef MICROWEB_RESPONSE_H
#define MICROWEB_RESPONSE_H

#include <string>
#include <unordered_map>
#include "./nlohmann/json.hpp"
#include "constants.h"

namespace MicroWeb {
class Response {
private:
    int status_code;
    std::string body;
    std::string content_type;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> cookies;

public:
    Response();

    void setStatus(int code);
    void setBody(const std::string& b);
    void setContentType(const std::string& type);
    void addHeader(const std::string& key, const std::string& value);
    void addCookie(const std::string& key, const std::string& value);

    void json(const nlohmann::json& data);
    void html(const std::string& html_content);
    void text(const std::string& text_content);

    std::string serialize() const;

    int getStatusCode() const;
    std::string getBody() const;

    ~Response();
};
}

#endif // MICROWEB_RESPONSE_H