#include "../include/server.h"
#include <iostream>
#include <cstring>
#include <sys/wait.h>

namespace MicroWeb {

Server::Server(int listen_port) 
    : port(listen_port), server_socket(-1), logger(Logger::getInstance()) {
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
}

void Server::setupSocket() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        logger->critical("Failed to create socket");
        throw std::runtime_error("Socket creation failed");
    }

    // address reuse
    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        logger->error("Failed to set socket options");
        close(server_socket);
        throw std::runtime_error("Socket option setting failed");
    }

    // bind
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        logger->critical("Failed to bind socket to port " + std::to_string(port));
        close(server_socket);
        throw std::runtime_error("Socket binding failed");
    }

    // listen
    if (listen(server_socket, MAX_CONNECTIONS) < 0) {
        logger->critical("Failed to listen on socket");
        close(server_socket);
        throw std::runtime_error("Listen failed");
    }

    logger->info("Server socket setup complete on port " + std::to_string(port));
}

void Server::handleClient(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_read <= 0) {
        logger->error("Error reading client socket");
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';  // Null-terminate the buffer
    
    Request request;
    Response response;

    try {
        request.parse(std::string(buffer));
        middleware.execute(request, response);
        router.handleRoute(request, response);

        std::string response_str = response.serialize();
        send(client_socket, response_str.c_str(), response_str.length(), 0);
    }
    catch (const std::exception& e) {
        logger->error("Request processing error: " + std::string(e.what()));
        response.setStatus(500);
        response.json({{"error", "Internal Server Error"}});
        std::string error_response = response.serialize();
        send(client_socket, error_response.c_str(), error_response.length(), 0);
    }

    close(client_socket);
}

void Server::start() {
    setupSocket();

    logger->info("Server starting on port " + std::to_string(port));

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket < 0) {
            logger->error("Failed to accept client connection");
            continue;
        }

        // fork to handle concurrent connections. inspired by https://ruslanspivak.com/lsbaws-part3/
        pid_t pid = fork();

        if (pid < 0) {
            logger->error("Fork failed");
            close(client_socket);
        }
        else if (pid == 0) {
            close(server_socket);
            handleClient(client_socket);
            exit(0);
        }
        else {
            close(client_socket);
            waitpid(-1, NULL, WNOHANG);
        }
    }
}

void Server::route(const std::string& method, const std::string& path, Router::RouteHandler handler) {
    router.addRoute(method, path, handler);
}

void Server::use(const std::string& name, Middleware::MiddlewareFunc middlewareFunc) {
    middleware.use(name, middlewareFunc);
}

void Server::stop() {
    if (server_socket >= 0) {
        close(server_socket);
        logger->info("Server stopped");
    }
}

Server::~Server() {
    stop();
}

} // namespace MicroWeb