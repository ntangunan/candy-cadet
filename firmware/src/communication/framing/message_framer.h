#pragma once
#include <cstddef>
#include <cstdint> // for storing bytes

namespace Communication
{
    class MessageFramer
    {
    public:
        bool processByte(uint8_t byte);

        // give me a pointer to the framer bytes but don't allow modification
        const uint8_t* data() const;

        size_t length() const;

        void reset();
        
    private:
        static constexpr size_t MAX_FRAME_SIZE = 128;
        uint8_t frame_[MAX_FRAME_SIZE];

        size_t length_;
        bool overflowing_;

    };
}