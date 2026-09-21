#include "receive_buffer.h"

namespace Communication
{
    ReceiveBuffer::ReceiveBuffer()
        : head_(0),
          tail_(0),
          full_(false)
    {
    }

    bool ReceiveBuffer::write(uint8_t byte)
    {
        if (isFull())
        {
            return false;
        }

        buffer_[head_] = byte;

        head_++;

        if (head_ > BUFFER_CAPACITY_ - 1)
        {
            head_ = 0;
        }

        if (head_ == tail_)
        {
            full_ = true;
        }

        return true;
    }

    bool ReceiveBuffer::read(uint8_t& byte)
    {
        if (isEmpty())
        {
            return false;
        }

        byte = buffer_[tail_];

        tail_++;

        if (tail_ > BUFFER_CAPACITY_ - 1)
        {
            tail_ = 0;
        }

        full_ = false;

        return true;
    }

    size_t ReceiveBuffer::available() const
    {
        if (isEmpty())
        {
            return 0;
        }

        if (isFull())
        {
            return BUFFER_CAPACITY_;
        }

        if (head_ > tail_)
        {
            return head_ - tail_;
        }
        
        return BUFFER_CAPACITY_ - tail_ + head_;
    }

    bool ReceiveBuffer::isEmpty() const
    {
        // head_ and tail_ are equal at empty state
        if (head_ == tail_ && full_ == false)
        {
            return true;
        }

        return false;
    }

    bool ReceiveBuffer::isFull() const
    {
        // head_ wraps around back to front when full
        if (head_ == tail_ && full_ == true)
        {
            return true;
        }

        return false;
    }

    void ReceiveBuffer::clear()
    {

    }
}