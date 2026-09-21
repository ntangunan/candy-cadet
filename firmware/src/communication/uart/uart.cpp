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
}