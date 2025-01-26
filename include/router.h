#ifndef MICROWEB_ROUTER_H
#define MICROWEB_ROUTER_H

#include <unordered_map>
#include <functional>
#include <regex>
#include "request.h"
#include "response.h"

namespace MicroWeb {
class Router {
public:
    using RouteHandler = std::function<void(Request&, Response&)>;

private:
    std::unordered_map<std::string, 
        std::unordered_map<std::string, RouteHandler>> routes;

public:
    Router();

    void addRoute(const std::string& method, const std::string& path, RouteHandler handler);
    bool handleRoute(Request& request, Response& response);
    void removeRoute(const std::string& method, const std::string& path);
    void addPatternRoute(const std::string& method, const std::string& pattern, RouteHandler handler);

    ~Router();
};
}

#endif // MICROWEB_ROUTER_H