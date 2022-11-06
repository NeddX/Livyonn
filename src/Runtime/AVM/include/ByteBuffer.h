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
            vector<uint8_t> buffer;

            inline void Write(int8_t data)
            {
                buffer.push_back(data);
            }

            inline void Write16(int16_t data)
            {
                Write(data & 0xff);
                Write(data >> 8);
            }

            inline void Write32(int32_t data)
            {
                Write16(data & 0xffff);
                Write16(data >> 16);
            }

            inline void WriteAt(size_t address, int8_t data)
            {
                if (buffer.size() - 1 < address) buffer.resize(buffer.size() + 32);
                buffer[address] = data;
            }

            inline void WriteAt16(size_t address, int16_t data)
            {
                WriteAt(address, data & 0xff);
                WriteAt(address + 1, data >> 8);
            }

            inline void WriteAt32(size_t address, int32_t data)
            {
                WriteAt16(address, data & 0xffff);
                WriteAt16(address + 2, data >> 16);
            }

            inline int8_t ReadFrom(size_t addr)
            {
                int8_t data = buffer[addr];
                buffer.erase(buffer.begin() + addr);
                return data;
            }

            inline int16_t ReadFrom16(size_t addr)
            {
                return (uint8_t) ReadFrom(addr + 1) << 8 | (uint8_t) ReadFrom(addr);
            }

            inline int32_t ReadFrom32(size_t addr)
            {
                return (uint16_t) ReadFrom16(addr + 2) << 16 | (uint16_t) ReadFrom16(addr);
            }

            inline void InsertBack(ByteBuffer& v)
            {
                buffer.insert(buffer.end(), v.buffer.begin(), v.buffer.end());
            }

            inline void Reserve(size_t size)
            {
                buffer.reserve(size);
            }

            inline void Resize(size_t size)
            {
                buffer.resize(size);
            }

            inline void RemoveRange(size_t start, size_t end)
            {
                buffer.erase(buffer.begin() + end, buffer.end());
                buffer.erase(buffer.begin(), buffer.begin() + start);
            }

            inline void Pop()
            {
                buffer.pop_back();
            }

            inline void Pop16()
            {
                Pop();
                Pop();
            }

            inline void Pop32()
            {
                Pop16();
                Pop16();
            }

            uint8_t& operator[](size_t index)
            {
                return buffer[index];
            }

            inline constexpr int8_t Read()
            {
                uint8_t data = buffer.back();
                buffer.pop_back();
                return data;
            }

            inline constexpr int16_t Read16()
            {
                return (uint8_t) Read() << 8 | (uint8_t) Read();
            }

            inline int32_t Read32()
            {
                return (uint16_t) Read16() << 16 | (uint16_t) Read16();
            }

            inline uint8_t Last()
            {
                return buffer.back();
            }

            inline vector<uint8_t>::iterator Begin()
            {
                return buffer.begin();
            }

            inline vector<uint8_t>::iterator End()
            {
                return buffer.end();
            }

            inline size_t Capacity()
            {
                return buffer.capacity();
            }

            inline size_t Size()
            {
                return buffer.size();
            }

            inline uint8_t* Ptr()
            {
                return buffer.data();
            }
    };
}