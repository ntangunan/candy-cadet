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

    }

    bool ReceiveBuffer::read(uint8_t byte)
    {

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