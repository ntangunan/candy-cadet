#include "led.h"
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

    void LED::enablePWM(PWM& pwm)
    {
        pwm_ = &pwm;
    }

    void LED::setBrightness(int percentage)
    {
        if (pwm_ == nullptr)
        {
            return;
        }

        pwm_->setPercentage(percentage);
    }


}