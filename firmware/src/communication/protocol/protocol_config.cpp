#include "protocol_config.h"

#include <utility>

namespace Communication
{
    ProtocolConfig::ProtocolConfig()
    {
        static const ArgumentDefinition motorArguments[] =
        {
            {"targetId", ArgumentType::UnsignedInteger},
            {"value", ArgumentType::Integer}
        };

        static const ArgumentDefinition servoArguments[] =
        {
            {"targetId", ArgumentType::UnsignedInteger},
            {"value", ArgumentType::Integer}
        };

        static const ArgumentDefinition statusArguments[] =
        {
            {"targetId", ArgumentType::UnsignedInteger},
        };

        static const ArgumentDefinition modeArguments[] =
        {
            {"targetId", ArgumentType::UnsignedInteger},
            {"value", ArgumentType::Integer}
        };

        CommandTable commandTable;

        commandTable.emplace(
            "MOTOR",
            CommandDefinition{
                motorArguments,
                2
            }
        );

        commandTable.emplace(
            "SERVO",
            CommandDefinition{
                servoArguments,
                2
            }
        );

        commandTable.emplace(
            "STATUS",
            CommandDefinition{
                statusArguments,
                1
            }
        );

        commandTable.emplace(
            "MODE",
            CommandDefinition{
                modeArguments,
                2
            }
        );

        tables_.emplace(
            "COMMAND",
            std::move(commandTable)
        );
    };

    const CommandTable* ProtocolConfig::findTable(
        const std::string& tableName
    ) const
    {
        auto iterator = tables_.find(tableName);

        if (iterator == tables_.find(tableName))
        {
            return nullptr;
        }

        return &iterator->second;
    }
}