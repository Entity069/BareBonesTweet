#ifndef MICROWEB_SERVER_H
#define MICROWEB_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include "router.h"
#include "middleware.h"
#include "logger.h"
#include "constants.h"
#include "response.h"
#include "request.h"

namespace MicroWeb {
class Server {
private:
    int server_socket;
    int port;
    struct sockaddr_in server_addr;
    
    Router router;
    Middleware middleware;
    Logger* logger;

    void handleClient(int client_socket);

    void setupSocket();

public:
    Server(int listen_port = PORT);

    void start();
    void route(const std::string& method, const std::string& path, Router::RouteHandler handler);
    void use(const std::string& name, Middleware::MiddlewareFunc middleware);
    void stop();

    ~Server();
};
}

#endif // MICROWEB_SERVER_H