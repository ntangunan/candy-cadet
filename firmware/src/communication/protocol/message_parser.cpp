#include "message_parser.h"

namespace Communication
{
    MessageParser::MessageParser()
    {
    }

    ParseResult MessageParser::parse(
        const uint8_t* data,
        size_t length,
        ParsedMessage& message
    )
    {
        // validate input
        if (data == nullptr || length == 0)
        {
            return ParseResult::InvalidInput;
        }

        // scan the fields
        size_t fieldCount = 0;

        ParseResult result = splitFields(
            data,
            length,
            fieldCount
        );

        if (result != ParseResult::Success)
        {
            return result;
        }

        return validateFields(
            fieldCount,
            message
        );
    }

    ParseResult MessageParser::splitFields(
        const uint8_t* data,
        size_t length,
        size_t& fieldCount
    )
    {
        fieldCount = 0;

        // clear fields form previous message
        for (size_t i = 0; i < MAX_FIELDS_; i++)
        {
            fields_[i].clear();
        }

        size_t currentField = 0;

        for (size_t i = 0; i < length; i++)
        {
            char c = static_cast<char>(data[i]);

            // space means we reached the end of a field
            if (c = ' ')
            {
                if (currentField >= MAX_FIELDS_)
                {
                    return ParseResult::InvalidFormat;
                }

                // reject empty fields
                // also includes leading, trailing, or consecutive spaces
                if (fields_[currentField].empty())
                {
                    return ParseResult::InvalidFormat;
                }

                currentField++;
            }
            // add character to current field
            else
            {
                if (currentField >= MAX_FIELDS_)
                {
                    return ParseResult::InvalidFormat;
                }

                if (fields_[currentField].length() >= MAX_FIELD_LENGTH_)
                {
                    return ParseResult::InvalidFormat;
                }

                fields_[currentField] += c;
            }
        }

        // add final field
        if (currentField >= MAX_FIELDS_)
        {
            return ParseResult::InvalidFormat;
        }

        if (fields_[currentField].empty())
        {
            return ParseResult::InvalidFormat;
        }

        fieldCount = currentField + 1;

        return ParseResult::Success;
    }

    ParseResult MessageParser::validateFields(
        size_t fieldCount,
        ParsedMessage& message
    )
    {
        // every command requires:
        // 
        // COMMAND <command> <arguments...>
        // 
        // therefore we need at least:
        //  fields_[0] = COMMAND
        //  fields_[1] = command
        //  fields_[2] = first argument
        
        if (fieldCount < 3)
        {
            return ParseResult::InvalidFormat;
        }

        // fields_[0] selects the protocol table
        const CommandTable* table = 
            protocolConfig_.findTable(fields_[0]);
        
        if (table == nullptr)
        {
            return ParseResult::InvalidField;
        }

        // fields_[1] selects the command
        auto commandIterator = table->find(fields_[1]);

        if (commandIterator == table->end())
        {
            return ParseResult::InvalidField;
        }

        const CommandDefinition& command =
            commandIterator->second;

        // the command definition includes all arguments including targetID
        // 
        // fields_[0] = table
        // fields_[1] = command
        // fields_[2...] = arguments
        // 
        // therefore:
        // 
        // total fields = 2 + argument count
        if (fieldCount != 2 + command.argumentCount)
        {
            return ParseResult::InvalidFormat;
        }

        message.command = &command;

        return ParseResult::Success;
    }
}