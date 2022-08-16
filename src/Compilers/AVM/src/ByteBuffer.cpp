#include "../include/ByteBuffer.h"

namespace avm
{
    void ByteBuffer::Write(int16_t data)
    {
        buffer.push_back(data);
    }

    void ByteBuffer::Write32(int32_t data)
    {
        Write(data & 0xffff);
        Write(data >> 16);
    }

    void ByteBuffer::Write64(int64_t data)
    {
        Write32(data & 0xffffffff);
        Write32(data >> 32);
    }

    void ByteBuffer::WriteToAddress(int16_t data, int address)
    {
        buffer.push_back(buffer[address]);
    }

    void ByteBuffer::WriteToAddress32(int32_t data, int address)
    {

    }

    void ByteBuffer::WriteToAddress64(int64_t data, int address)
    {

    }   

    int16_t ByteBuffer::Read()
    {
        int16_t data = buffer.back();
        buffer.pop_back();
        return data;
    }

    int32_t ByteBuffer::Read32()
    {
        return (static_cast<int32_t>(Read()) << 16) | Read(); // Static cast for stability
    }

    int64_t ByteBuffer::Read64()
    {
        return (static_cast<int64_t>(Read32()) << 32) | Read32(); // Static cast for stability
    }

    void ByteBuffer::Pop()
    {
        buffer.pop_back();
    }

    void ByteBuffer::Pop32()
    {
        Pop();
        Pop();
    }

    void ByteBuffer::Pop64()
    {
        Pop32();
        Pop32();
    }    

    int16_t& ByteBuffer::operator[](size_t index)
    {
        return buffer[index];
    }

    int16_t ByteBuffer::Last()
    {
        return buffer.back();
    }

    void ByteBuffer::Push(int16_t data)
    {
        buffer.push_back(data);
    }
}