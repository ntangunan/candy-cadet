#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

namespace Communication
{
    struct UARTConfig
    {
        uint32_t baudRate;
        int8_t rxPin;
        int8_t txPin;

    };

    enum class UARTStatus
    {
        Ok,
        NoData,
        NotInitialized,
        InvalidConfig
    };

    class UART
    {
    public:
        UART();

        UARTStatus initialize(const UARTConfig& config);

        size_t available();
        
        UARTStatus readByte(uint8_t& byte);
        
        size_t sendByte(uint8_t byte);
        
        size_t send(const uint8_t* buffer, size_t length);

    private:
        HardwareSerial serial_;
        
        bool initialized_;
    };
}
