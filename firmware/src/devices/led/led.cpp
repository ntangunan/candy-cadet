#include "led.h"

// #include "../../config/board_config.h"
#include "../../hal/gpio/gpio.h"

namespace Devices
{
    LED::LED(uint8_t pin)
        : pin_(pin)
    {

    }
    void LED::initialize()
    {
        HAL::GPIO::initialize(pin_, true);
        off();
    }

    void LED::on()
    {
        state_ = true;
        HAL::GPIO::write(pin_, true);
    }

    void LED::off()
    {
        state_ = false;
        HAL::GPIO::write(pin_, false);
    }

    void LED::toggle()
    {
        state_ = !state_;
        HAL::GPIO::write(pin_, state_);
    }
}