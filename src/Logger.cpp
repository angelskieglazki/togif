#include "Logger.h"

std::ostream& operator<<(std::ostream& o, Logger::MessageType log_level) {
    switch(log_level)
    {
        case Logger::MessageType::kDetail : o << "Detail"; break;
        case Logger::MessageType::kDebug : o << "Debug"; break;
        case Logger::MessageType::kInfo : o << "Info"; break;
        case Logger::MessageType::kWarning : o << "Warning"; break;
        case Logger::MessageType::kStats : o << "Stats"; break;
        case Logger::MessageType::kDisableLogs : o << "DisableLogs"; break;
        case Logger::MessageType::kError : o << "Error"; break;
        default: o << "Unsupported log_level " << (int)log_level;
    };

    return o;
}
