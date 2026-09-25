#include <unity.h>
#include <cstring>

#include "communication/protocol/message_parser.h"

using namespace Communication;

void test_valid_motor_command()
{
    MessageParser parser;
    ParsedMessage message{};

    const char* input = "COMMAND MOTOR 1 50";

    ParseResult result = parser.parse(
        reinterpret_cast<const uint8_t*>(input),
        strlen(input),
        message
    );

    TEST_ASSERT_EQUAL(
        ParseResult::Success,
        result
    );

    TEST_ASSERT_EQUAL_UINT32(
        1,
        message.targetId
    );

    TEST_ASSERT_EQUAL_INT32(
        50,
        message.arguments.value
    );
}

void test_unknown_command()
{
    MessageParser parser;
    ParsedMessage message{};

    const char* input = "COMMAND UNKNOWN 1 50";

    ParseResult result = parser.parse(
        reinterpret_cast<const uint8_t*>(input),
        strlen(input),
        message
    );

    TEST_ASSERT_EQUAL(
        ParseResult::InvalidField,
        result
    );
}

void test_invalid_argument_count()
{
    MessageParser parser;
    ParsedMessage message{};

    const char* input = "COMMAND MOTOR 1";

    ParseResult result = parser.parse(
        reinterpret_cast<const uint8_t*>(input),
        strlen(input),
        message
    );

    TEST_ASSERT_EQUAL(
        ParseResult::InvalidFormat,
        result
    );
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_valid_motor_command);
    RUN_TEST(test_unknown_command);
    RUN_TEST(test_invalid_argument_count);

    UNITY_END();
}

void loop()
{
}