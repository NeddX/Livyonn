#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

namespace avm
{
    using namespace std;

    class ByteBuffer
    {
        public:
            vector<uint16_t> buffer;

            void Write(int16_t data);
            void Write32(int32_t data);
            void Write64(int64_t data);

            void WriteToAddress(uint16_t data, int address);
            void WriteToAddress32(uint32_t data, int address);
            void WriteToAddress64(uint64_t data, int address);

            void RemoveRange(size_t start, size_t end);
            void InsertBack(ByteBuffer& v);
            void Reserve(size_t size);
            void Push(int16_t data);

            void Pop();
            void Pop32();
            void Pop64();

            constexpr uint16_t& operator[](size_t index)
            {
                return buffer[index];
            }
            constexpr int16_t Read()
            {
                uint16_t data = buffer.back();
                buffer.pop_back();
                return data;
            }
            constexpr int32_t Read32()
            {
                return (static_cast<uint32_t>(Read()) << 16) | (uint16_t) (Read()); // Static cast for stability
            }
            constexpr int64_t Read64()
            {
                return (static_cast<uint64_t>(Read32()) << 32) | (uint32_t) Read32(); // Static cast for stability
            }               
            constexpr uint16_t Last()
            {
                return buffer.back();
            }
            constexpr vector<uint16_t>::iterator Begin()
            {
                return buffer.begin();
            }
            constexpr vector<uint16_t>::iterator End()
            {
                return buffer.end();
            }
            constexpr size_t Capacity()
            {
                return buffer.capacity();
            }
            constexpr size_t Size()
            {
                return buffer.size();
            }
    };
}