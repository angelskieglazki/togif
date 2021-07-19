#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <atomic>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "MessageBuilder.h"


class Logger
{
public:
    enum MessageType
    {
        kDetail = 0,
        kDebug = 1,
        kInfo = 2,
        kWarning = 3,
        kStats = 4
    };

    void setLogLevel(MessageType type) { m_log_level.store(type); }
    MessageType logLevel() { return m_log_level.load(); }

    template<typename ...Args>
    void log(MessageType type, Args&& ...args) {
        auto msg = MessageBuilder::createMessage(std::forward<Args>(args)...);
   
        if (type < m_log_level.load()) return;

        auto now = std::chrono::high_resolution_clock::now();
        auto now_time_t = std::chrono::high_resolution_clock::to_time_t(now);
        auto now_microseconds = getMicroseconds(now);

        std::cout << std::put_time(std::localtime(&now_time_t), "%H:%M:%S") << ":" << now_microseconds << " | " << msg << "\n";
    }

    static std::string stripFunctionName(const std::string& pretty_name) {
        size_t colons = pretty_name.find("::");
        size_t begin = pretty_name.substr(0,colons).rfind(" ") + 1;
        size_t end = pretty_name.rfind("(") - begin;

        return pretty_name.substr(begin,end) + "()";
    }
private:
    template<typename T>
    int64_t getMicroseconds(T time_point)
    {
        auto now_in_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time_point.time_since_epoch());
        auto now_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch());
        auto now_microseconds = now_in_microseconds - now_in_seconds;
        return now_microseconds.count();
    }
private:
    std::atomic<MessageType> m_log_level = kDetail;
};

#define BOLD_GREEN "\033[1;32m"
#define BOLD_RED "\033[1;31m"
#define RESET_COLOR "\033[0m"

#define METHOD_NAME Logger::stripFunctionName(__PRETTY_FUNCTION__)

#define LOG_DETAIL(logger, ...) logger.log(Logger::kDetail, METHOD_NAME, " | ", __VA_ARGS__)
#define LOG_DEBUG(logger, ...) logger.log(Logger::kDebug, METHOD_NAME, " | ", __VA_ARGS__)
#define LOG_INFO(logger, ...) logger.log(Logger::kInfo, BOLD_GREEN, METHOD_NAME, " | ", RESET_COLOR, __VA_ARGS__)
#define LOG_WARNING(logger, ...) logger.log(Logger::kWarning, BOLD_RED, METHOD_NAME, " | ", RESET_COLOR, __VA_ARGS__)
#define LOG_STATS(logger, ...) logger.log(Logger::kStats, BOLD_GREEN, METHOD_NAME, " | ", RESET_COLOR, __VA_ARGS__)

#define LOG_WARNING_RETURN(logger, ...) { \
     logger.log(Logger::kWarning, BOLD_RED, METHOD_NAME, " | ", RESET_COLOR, __VA_ARGS__); \
     return; \
}

#endif
