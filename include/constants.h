// Declares most used constants in the project

#ifndef MICROWEB_CONSTANTS_H
#define MICROWEB_CONSTANTS_H

#include <string>

namespace MicroWeb {
    constexpr int MAX_CONNECTIONS = 1000;
    constexpr int BUFFER_SIZE = 1024;
    constexpr int PORT = 8080;
    constexpr const char* SERVER_NAME = "heyserver/1.0";

    constexpr int HTTP_OK = 200;
    constexpr int HTTP_BAD_REQUEST = 400;
    constexpr int HTTP_NOT_FOUND = 404;
    constexpr int HTTP_METHOD_NOT_ALLOWED = 405;
    constexpr int HTTP_INTERNAL_SERVER_ERROR = 500;

    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };
    // Adding only a few MIME types for demonstration purposes
    // TODO: Add more MIME types
    const std::string MIME_JSON = "application/json";
    const std::string MIME_HTML = "text/html";
    const std::string MIME_PLAIN = "text/plain";
}

#endif // MICROWEB_CONSTANTS_H