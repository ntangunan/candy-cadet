#include "protocol_config.h"

namespace Communication
{
    ProtocolConfig::ProtocolConfig()
    {
        commands_.emplace(
            "MOTOR",
            5,
            CommandDefinition{1}
        );

        commands_.emplace(
            "SERVO",
            5,
            CommandDefinition{1}
        );

        commands_.emplace(
            "STATUS",
            6,
            CommandDefinition{0}
        );

        commands_.emplace(
            "MODE",
            4,
            CommandDefinition{1}
        );
    }

    const CommandDefinition* ProtocolConfig::findCommand(
        const uint8_t* data,
        size_t fieldStart,
        size_t fieldLength
    ) const
    {
        if (data == nullptr || fieldLength == 0)
        {
            return nullptr;
        }

        std::string command(
            reinterpret_cast<const char*>(data + fieldStart),
            fieldLength
        );

        auto iterator = commands_.find(command);

        if (iterator == commands_.end())
        {
            return nullptr;
        }

        return &iterator->second;
    }
}