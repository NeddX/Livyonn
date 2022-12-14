#include "include/ByteBuffer.h"

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

    void ByteBuffer::WriteToAddress(uint16_t data, size_t address)
    {
        buffer[address] = data;
        //buffer.pop_back();
    }

    void ByteBuffer::WriteToAddress32(uint32_t data, size_t address)
    {
        WriteToAddress(data & 0xffff, address);
        WriteToAddress(data >> 16, address + 1);
    }

    void ByteBuffer::WriteToAddress64(uint64_t data, size_t address)
    {
        WriteToAddress32(data & 0xffffffff, address);
        WriteToAddress32(data >> 32, address + 2);
    }   

    void ByteBuffer::PushFromAddress(size_t address)
    {
        buffer.push_back(buffer[address]);
    }

    void ByteBuffer::PushFromAddress32(size_t address)
    {
        PushFromAddress(address);
        PushFromAddress(address + 1);
    }

    void ByteBuffer::PushFromAddress64(size_t address)
    {
        PushFromAddress32(address);
        PushFromAddress32(address + 2); 
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

    void ByteBuffer::Push(int16_t data)
    {
        buffer.push_back(data);
    }

    void ByteBuffer::InsertBack(ByteBuffer& v)
    {
        buffer.insert(buffer.end(), v.buffer.begin(), v.buffer.end());
    }

    void ByteBuffer::Reserve(size_t size)
    {
        buffer.reserve(size);
    }

    void ByteBuffer::RemoveRange(size_t start, size_t end)
    {
        buffer.erase(buffer.begin() + end, buffer.end());
        buffer.erase(buffer.begin(), buffer.begin() + start);
    }
}