#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace Communication
{
    // metadata associated with a protocol command.
    struct CommandDefinition
    {
        size_t nameLength;
        size_t argumentCount;
    };

    class ProtocolConfig
    {
    public:
        ProtocolConfig();

        // looks up a command definition using the command field.
        const CommandDefinition* findCommand(
            const uint8_t* data,
            size_t fieldStart,
            size_t fieldLength
        ) const;

    private:
        std::unordered_map<std::string, CommandDefinition> commands_;
    };
}