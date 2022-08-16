#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

namespace avm
{
    using namespace std;

    class ByteBuffer
    {
        public:
            vector<int16_t> buffer;

            void Write(int16_t data);
            void Write32(int32_t data);
            void Write64(int64_t data);

            void WriteToAddress(int16_t data, int address);
            void WriteToAddress32(int32_t data, int address);
            void WriteToAddress64(int64_t data, int address);

            int16_t Read();
            int32_t Read32();
            int64_t Read64();

            void Pop();
            void Pop32();
            void Pop64();
            int16_t& operator[](size_t index);
            int16_t Last();
            void Push(int16_t data);
    };
}