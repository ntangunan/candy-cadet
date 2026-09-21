#include "uart.h"

namespace Communication
{
    UART::UART()
        : serial_(1),
          initialized_(false)
    {
    }

    UARTStatus UART::initialize(const UARTConfig& config)
    {
        if (config.baudRate == 0)
        {
            return UARTStatus::InvalidConfig;
        }

        serial_.begin(
            config.baudRate,
            SERIAL_8N1,
            config.rxPin,
            config.txPin
        );

        initialized_ = true;

        return UARTStatus::Ok;
    }

    size_t UART::available()
    {
        if (!initialized_)
        {
            return 0;
        }

        return serial_.available();
    }

    UARTStatus UART::readByte(uint8_t& byte)
    {
        if (!initialized_)
        {
            return UARTStatus::NotInitialized;
        }

        int recieved = serial_.read();

        // serial_.read() returns -1 if there is no data to be read
        if (recieved == -1)
        {
            return UARTStatus::NoData;
        }

        // convert received value to uint8_t
        byte = static_cast<uint8_t>(recieved);

        return UARTStatus::Ok;
    }

    size_t UART::sendByte(uint8_t byte)
    {
        if (!initialized_)
        {
            return 0;
        }

        size_t numAcceptedBytes = serial_.write(byte);

        return numAcceptedBytes;
    }

    size_t UART::send(const uint8_t* buffer, size_t length)
    {
        if (!initialized_)
        {
            return 0;
        }

        if (length == 0 || buffer == nullptr)
        {
            return 0;
        }
        
        size_t numAcceptedBytes = serial_.write(buffer, length);

        return numAcceptedBytes;
    }
}