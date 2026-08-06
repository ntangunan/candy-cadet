#include "button.h"

#include "../../config/board_config.h"
#include "../../hal/gpio/gpio.h"

namespace Devices
{
    void Button::initialize()
    {
        HAL::GPIO::initialize(Board::USER_BUTTON_PIN, false);
    }

    bool Button::isPressed()
    {
        // INPUT_PULLUP means LOW = pressed
        return !HAL::GPIO::read(Board::USER_BUTTON_PIN);
    }
}