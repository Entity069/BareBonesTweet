#include "../include/request.h"
#include "../include/encode.h"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace MicroWeb {

Request::Request() {
}

void Request::parse(const std::string& raw_request) {
    std::istringstream iss(raw_request);
    std::string line;

    if (std::getline(iss, line)) {
        std::istringstream first_line(line);
        first_line >> method >> path;
        
        std::string http_version;
        first_line >> http_version;
    }

    body.clear();

    std::string accumulated_headers;
    while (std::getline(iss, line) && !line.empty() && line != "\r") {
        accumulated_headers += line + "\n";
        
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            
            key.erase(0, key.find_first_not_of(" "));
            key.erase(key.find_last_not_of(" ") + 1);
            value.erase(0, value.find_first_not_of(" "));
            value.erase(value.find_last_not_of(" ") + 1);
            
            headers[key] = value;
        }
    }

    std::string body_content;
    std::string remaining_line;
    while (std::getline(iss, remaining_line)) {
        body_content += remaining_line;
    }
    
    body = body_content;
    // std::cout << "Method: " << method << std::endl;
    // std::cout << "Path: " << path << std::endl;
    // std::cout << "Headers:\n" << accumulated_headers << std::endl;
    // std::cout << "Body: " << decodeURIComponent(body) << std::endl;

    size_t query_pos = path.find('?');
    if (query_pos != std::string::npos) {
        std::string query_string = path.substr(query_pos + 1);
        path = path.substr(0, query_pos);

        std::istringstream query_stream(query_string);
        std::string param;
        while (std::getline(query_stream, param, '&')) {
            size_t equal_pos = param.find('=');
            if (equal_pos != std::string::npos) {
                std::string key = param.substr(0, equal_pos);
                std::string value = param.substr(equal_pos + 1);
                query_params[key] = value;
            }
        }
    }
}

std::string Request::getMethod() const {
    return method;
}

std::string Request::getPath() const {
    return path;
}

std::string Request::getBody() const {
    return body;
}

std::string Request::getHeader(const std::string& key) const {
    auto it = headers.find(key);
    return (it != headers.end()) ? it->second : "";
}

std::string Request::getQueryParam(const std::string& key) const {
    auto it = query_params.find(key);
    return (it != query_params.end()) ? decodeURIComponent(it->second) : "";
}

std::string Request::getCookie(const std::string& key) const {
    auto it = cookies.find(key);
    return (it != cookies.end()) ? it->second : "";
}

nlohmann::json Request::getJsonBody() const {
    nlohmann::json parsedData;
    std::istringstream bodyStream(body);
    std::string pair;
    while (std::getline(bodyStream, pair, '&')) {
        size_t equalPos = pair.find('=');
        
        if (equalPos != std::string::npos) {
            std::string key = pair.substr(0, equalPos);
            std::string value = pair.substr(equalPos + 1);

            key = decodeURIComponent(key);
            value = decodeURIComponent(value);

            parsedData[key] = value;
        }
    }

    return parsedData;
}

void Request::setMethod(const std::string& m) {
    method = m;
}

void Request::setPath(const std::string& p) {
    path = p;
}

void Request::addHeader(const std::string& key, const std::string& value) {
    headers[key] = value;
}

void Request::addQueryParam(const std::string& key, const std::string& value) {
    query_params[key] = value;
}

void Request::addCookie(const std::string& key, const std::string& value) {
    cookies[key] = value;
}

Request::~Request() {
    headers.clear();
    query_params.clear();
    cookies.clear();
}

} // namespace MicroWeb