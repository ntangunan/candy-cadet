#pragma once
#include "message_parser.h"

namespace Communication
{
    struct CommandDefinition
    {
        const char* name;
        size_t argumentCount;
    };

    // look up function that finds the command definition within our config table
    const CommandDefinition* findCommand(
        const uint8_t* data,
        size_t fieldStart,
        size_t fieldLength
    );
}