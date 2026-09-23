#pragma once

#include <cstddef>
#include <cstdint> // for fixed-width integer bytes

namespace Communication
{
    enum class MessageType
    {
        Command
    };

    // these represent protocol commands, not hardware objects
    enum class CommandType
    {
        Motor,
        Servo,
        Status,
        Mode
    };

    enum class ParseResult
    {
        Success,
        InvalidInput,
        InvalidFormat,
        InvalidField
    };

    struct CommandArguments
    {
        // int32_t means exactly 32 bits
        int32_t value;
    };

    struct ParsedMessage
    {
        MessageType type;
        CommandType command;
        uint32_t messageId;
        CommandArguments arguments;

    };

    class MessageParser
    {
    public:
        MessageParser();

        ParseResult parse (
            const uint8_t* data,
            size_t length,
            ParsedMessage& message
        );
    };


}
