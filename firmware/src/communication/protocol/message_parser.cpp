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

            // reached the end of a field
            if (c = ' ')
            {
                if (currentField >= MAX_FIELDS_)
                {
                    return ParseResult::InvalidFormat;
                }

                if (fields_[currentField].empty())
                {
                    return ParseResult::InvalidFormat;
                }

                currentField++;
            }
            // creating current individual field string
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
        // field 0; message type
        if (fields_[0] != "COMMAND")
        {
            return ParseResult::InvalidField;
        }
    }
}