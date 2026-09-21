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

        size_t available() const;

        bool isEmpty() const;

        bool isFull() const;

        void clear();

    private:
        static constexpr size_t CAPACITY = 64;

        uint8_t buffer_[CAPACITY];

        size_t head_;
        size_t tail_;

        bool full_;
    };
}