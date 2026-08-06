#include "led.h"

#include "../../config/board_config.h"
#include "../../hal/gpio/gpio.h"

namespace Devices
{
    void LED::initialize()
    {
        HAL::GPIO::initialize(Board::STATUS_LED_PIN, true);

        off();
    }

    void LED::on()
    {
        state_ = true;
        HAL::GPIO::write(Board::STATUS_LED_PIN, true);
    }

    void LED::off()
    {
        state_ = false;
        HAL::GPIO::write(Board::STATUS_LED_PIN, false);
    }

    void LED::toggle()
    {
        state_ = !state_;
        HAL::GPIO::write(Board::STATUS_LED_PIN, state_);
    }
}