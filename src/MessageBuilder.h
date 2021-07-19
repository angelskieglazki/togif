#ifndef MESSAGE_BUILDER_H
#define MESSAGE_BUILDER_H

#include <sstream>

class MessageBuilder
{
public:
    template <typename... Args>
    static std::string createMessage(Args&& ...args) {
        std::ostringstream s;
        (s << ... << args);
        return s.str();
    }
};

#endif