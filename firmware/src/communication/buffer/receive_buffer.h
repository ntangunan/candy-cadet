#pragma once

#include <cstddef>
#include <cstdint> // for storing bytes

namespace Communication
{
    class ReceiveBuffer
    {
    public:
        ReceiveBuffer();

        bool write(uint8_t byte);

        bool read(uint8_t& byte);

        // returns the number of bytes currently stored and available for reading
        size_t available() const;

        // checks if buffer is empty
        bool isEmpty() const;

        // checks if buffer is full
        bool isFull() const;

        void clear();

    private:
        static constexpr size_t BUFFER_CAPACITY_ = 64;

        uint8_t buffer_[BUFFER_CAPACITY_];

        size_t head_; // where the n ext byte will be written
        size_t tail_; // where the next byte will be read

        bool full_;
    };
}