#include "../include/router.h"
#include "../include/constants.h"
#include <iostream>
#include <regex>

namespace MicroWeb {

Router::Router() {
}

void Router::addRoute(const std::string& method, const std::string& path, RouteHandler handler) {
    routes[method][path] = handler;
}

bool Router::handleRoute(Request& request, Response& response) {
    std::string method = request.getMethod();
    std::string path = request.getPath();

    auto method_routes = routes.find(method);
    if (method_routes != routes.end()) {
        auto route = method_routes->second.find(path);
        if (route != method_routes->second.end()) {
            route->second(request, response);
            return true;
        }
    }

    for (const auto& [route_method, route_map] : routes) {
        if (route_method != method) {
            response.setStatus(HTTP_METHOD_NOT_ALLOWED);
            response.json({
                {"error", "Method Not Allowed"},
                {"message", "Method " + method + " not allowed on this route"}
            });
            return false;
        } else if (route_method == method) {
            for (const auto& [pattern, handler] : route_map) {
                try {
                    std::regex route_regex(pattern);
                    if (std::regex_match(path, route_regex)) {
                        handler(request, response);
                        return true;
                    }
                } catch (const std::regex_error& e) {
                    // add regex error if any error in route finding due to it
                    std::cerr << "Regex error: " << e.what() << std::endl;
                }
            }
        }
    }

    // no route found
    response.setStatus(HTTP_NOT_FOUND);
    response.json({
        {"error", "Not Found"},
        {"message", "No route matches " + path}
    });
    return false;
}

void Router::removeRoute(const std::string& method, const std::string& path) {
    auto method_routes = routes.find(method);
    if (method_routes != routes.end()) {
        method_routes->second.erase(path);
    }
}

void Router::addPatternRoute(const std::string& method, const std::string& pattern, RouteHandler handler) {
    routes[method][pattern] = handler;
}

Router::~Router() {
    routes.clear();
}

} // namespace MicroWeb