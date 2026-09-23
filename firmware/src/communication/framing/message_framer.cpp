#include "message_framer.h"

namespace Communication
{
    bool MessageFramer::processByte(uint8_t byte)
    {
        // discard until '\n' if overflowing
        if (overflowing_)
        {
            if (byte == '\n')
            {
                overflowing_ = false;
                length_ = 0;
                return false;
            }
            else 
            {
                return false;
            }
        }

        if (byte == '\n')
        {
            if (length_ == 0)
            {
                return false;
            }

            return true;
        }
        
        if (length_ < MAX_FRAME_SIZE)
        {
            frame_[length_] = byte;
            length_++;
            return false;
        }
        else
        {
            overflowing_ = true;
            return false;
        }

    }

    const uint8_t* MessageFramer::data() const
    {
        return frame_;
    }

    size_t MessageFramer::length() const
    {
        return length_;
    }

    void MessageFramer::reset()
    {

    }
}