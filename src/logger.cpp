#include "../include/logger.h"
#include <iostream>
#include <iomanip>
#include <chrono>

namespace MicroWeb {

Logger* Logger::instance = nullptr;

Logger::Logger(const std::string& filename, LogLevel level)
    : log_file(filename, std::ios::app), current_level(level) {
    if (!log_file.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

Logger* Logger::getInstance(const std::string& filename, LogLevel level) {
    if (instance == nullptr) {
        instance = new Logger(filename, level);
    }
    return instance;
}
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level >= current_level) {
        std::string log_message = getCurrentTimestamp() + " [" + logLevelToString(level) + "]: " + message;
        
        if (log_file.is_open()) {
            log_file << log_message << std::endl;
            log_file.flush();
        }

        std::cout << log_message << std::endl;
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::critical(const std::string& message) {
    log(LogLevel::CRITICAL, message);
}

Logger::~Logger() {
    if (log_file.is_open()) {
        log_file.close();
    }
    instance = nullptr;
}

} // namespace MicroWeb