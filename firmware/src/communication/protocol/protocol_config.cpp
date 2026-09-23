#include "protocol_config.h"

namespace
{
    constexpr Communication::CommandDefinition COMMANDS[] =
    {
        {"MOTOR", 5, 1},
        {"SERVO", 5, 1},
        {"STATUS", 6, 0},
        {"MODE", 4, 1}

    };
}

namespace Communication
{
    ProtocolConfig::ProtocolConfig()
    {
        // initialize table
        for (size_t i = 0; i < COMMAND_TABLE_SIZE_; i++)
        {
            commandTable_[i].definition = nullptr;
        }
        
        // insert command definitions
        for (const CommandDefinition& command : COMMANDS)
        {
            addCommand_(command);
        }
    }

}