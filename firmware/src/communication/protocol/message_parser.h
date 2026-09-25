#pragma once

#include <cstddef>
#include <cstdint> // for fixed-width integer bytes
#include <string>

#include "protocol_config.h"

namespace Communication
{
    enum class MessageType
    {
        Command
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
        const CommandDefinition* command;
        uint32_t targetId;
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
    
        private:
            // splits message into fields separated by space characters
            ParseResult splitFields(
                const uint8_t* data,
                size_t length,
                size_t& fieldCount
            );

            // validates the fields of a message by comparing them to protocol_config
            ParseResult validateFields(
                size_t fieldCount,
                ParsedMessage& message
            );


            ProtocolConfig protocolConfig_;
            
            static constexpr size_t MAX_FIELDS_ = 6;
            static constexpr size_t MAX_FIELD_LENGTH_ = 128;

            std::string fields_[MAX_FIELDS_];
    };


}
