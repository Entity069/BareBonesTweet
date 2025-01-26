#include "../include/middleware.h"
#include <iostream>

namespace MicroWeb {

Middleware::Middleware() {
}

void Middleware::use(const std::string& name, MiddlewareFunc middleware) {
    middleware_stack[name] = middleware;
}

bool Middleware::execute(Request& request, Response& response) {
    for (const auto& [name, middleware] : middleware_stack) {
        if (!middleware(request, response)) {
            return false;
        }
    }
    return true;
}

void Middleware::remove(const std::string& name) {
    auto it = middleware_stack.find(name);
    if (it != middleware_stack.end()) {
        middleware_stack.erase(it);
    }
}

//bool Middleware::authMiddleware(Request& request, Response& response) {
//    std::string auth_token = request.getHeader("Authorization");
//
//    if (auth_token.empty() || auth_token != "Bearer valid_token") {
//        response.setStatus(401);
//        response.json({
//            {"error", "Authentication failed"},
//            {"message", "Invalid or missing authentication token"}
//        });
//        return false;
//    }
//    return true;
//}

bool Middleware::cookieMiddleware(Request& request, Response& response) {
    std::string cookie_header = request.getHeader("Cookie");
    
    if (!cookie_header.empty()) {
        std::stringstream ss(cookie_header);
        std::string cookie;
        
        while (std::getline(ss, cookie, ';')) {
            size_t delim_pos = cookie.find('=');
            if (delim_pos != std::string::npos) {
                std::string key = cookie.substr(0, delim_pos);
                std::string value = cookie.substr(delim_pos + 1);
                
                // Trim whitespace
                key.erase(0, key.find_first_not_of(" "));
                key.erase(key.find_last_not_of(" ") + 1);
                value.erase(0, value.find_first_not_of(" "));
                value.erase(value.find_last_not_of(" ") + 1);
                
                request.addCookie(key, value);
            }
        }
    }
    
    return true;
}

Middleware::~Middleware() {
    middleware_stack.clear();
}

} // namespace MicroWeb