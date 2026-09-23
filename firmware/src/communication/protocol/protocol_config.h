#pragma once
#include <cstddef>
#include <cstdint> 

namespace Communication
{
    // the required elements that make up a command's definition
    struct CommandDefinition
    {
        const char* name;
        size_t nameLength;
        size_t argumentCount;
    };

    class ProtocolConfig
    {
    public:
        ProtocolConfig();

        // look up function that finds the command definition within our config table
        const CommandDefinition* findCommand(
            const uint8_t* data,
            size_t fieldStart,
            size_t fieldLength
        ) const;

    private:
        static constexpr size_t COMMAND_TABLE_SIZE_ = 16;

        struct CommandEntry
        {
            const CommandDefinition* definition;
        };

        CommandEntry commandTable_[COMMAND_TABLE_SIZE_];

        static uint32_t hash_(
            const uint8_t* data,
            size_t length
        );

        bool addCommand_(
            const CommandDefinition& definition
        );
    };
} 