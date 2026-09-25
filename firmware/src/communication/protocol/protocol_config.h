#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>

namespace Communication
{
    enum class ArgumentType
    {
        Integer,
        UnsignedInteger,
        Float
    };

    struct ArgumentDefinition
    {
        const char* name;
        ArgumentType type;
    };

    struct CommandDefinition
    {
        const ArgumentDefinition* arguments;
        size_t argumentCount;
    };

    using CommandTable = 
        std::unordered_map<std::string, CommandDefinition>;
    
    using ProtocolTableMap = 
        std::unordered_map<std::string, CommandTable>;

    class ProtocolConfig
    {
    public:
        ProtocolConfig();

        // looks up a specific table given the first field of a message (tableName)
        const CommandTable* findTable(
            const std::string& tableName
        ) const;

    private:
        ProtocolTableMap tables_;
    };
}