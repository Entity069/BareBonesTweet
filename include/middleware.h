#ifndef MICROWEB_MIDDLEWARE_H
#define MICROWEB_MIDDLEWARE_H

#include <functional>
#include <unordered_map>
#include <string>
#include "request.h"
#include "response.h"

namespace MicroWeb {
class Middleware {
public:
    using MiddlewareFunc = std::function<bool(Request&, Response&)>;

private:
    std::unordered_map<std::string, MiddlewareFunc> middleware_stack;

public:
    Middleware();
    void use(const std::string& name, MiddlewareFunc middleware);
    bool execute(Request& request, Response& response);
    void remove(const std::string& name);

//    static bool authMiddleware(Request& request, Response& response);
    static bool cookieMiddleware(Request& request, Response& response);

    ~Middleware();
};
}

#endif // MICROWEB_MIDDLEWARE_H