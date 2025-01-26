// Includes logging server activities to a file


#ifndef MICROWEB_LOGGER_H
#define MICROWEB_LOGGER_H

#include <string>
#include <fstream>
#include <ctime>
#include "constants.h"

namespace MicroWeb {
class Logger {
private:
    std::ofstream log_file;
    static Logger* instance;
    LogLevel current_level;

    Logger(const std::string& filename = "server.log", 
           LogLevel level = LogLevel::INFO);

public:
    static Logger* getInstance(const std::string& filename = "server.log", 
                               LogLevel level = LogLevel::INFO);

    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);

    ~Logger();
};
}

#endif // MICROWEB_LOGGER_H