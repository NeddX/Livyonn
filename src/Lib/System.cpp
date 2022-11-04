#include "include/System.h"

namespace System
{
    std::optional<std::string> File::ReadToString(const std::string& path)
    {
        std::ifstream fs(path, std::ios::in);
        if (fs.is_open())
        {
			fs.seekg(SEEK_END);
            size_t size = fs.tellg();
			fs.seekg(0);

            std::string content(size, '\0');
            fs.read(content.data(), size);
            return content;
        }
        else return std::nullopt;
    }

    std::vector<uint8_t> Stack::Convert_16bitTo8bit(const std::vector<uint16_t>& buffer)
    {
        std::vector<uint8_t> n_buffer;
        for (auto& data : buffer)
        {
            n_buffer.push_back(data & 0xff);
            n_buffer.push_back(data >> 8);
        }
        return n_buffer;
    }

    std::vector<uint16_t> Stack::Convert_8bitTo16bit(std::vector<uint8_t> buffer)
    {
        std::vector<uint16_t> n_buffer;
        while (true)
        {
            if (buffer.empty()) break;

            uint8_t lowEnd = buffer.back();
            buffer.pop_back();
            uint8_t highEnd = buffer.back();
            buffer.pop_back();

            n_buffer.insert(n_buffer.begin(), (uint16_t)lowEnd << 8 | highEnd);
        }
        return n_buffer;
    }

    std::optional<std::vector<uint8_t>> File::ReadAllBytes(const std::string& path)
    {
        std::ifstream fs;
        fs.open(path, std::ios::in);

		fs.seekg(SEEK_END);
        size_t size = fs.tellg();
		fs.seekg(0);

        std::vector<uint8_t> buffer(size);
        if (!fs.is_open()) return std::nullopt;
        fs.read((char*)buffer.data(), size);

        if (buffer.size() != size) return std::nullopt;
        return buffer;
    }

    bool File::WriteToBytes(const std::string& path, const std::vector<uint8_t>& buffer)
    {
        std::ofstream fs;
        fs.open(path);
        if (!fs.is_open()) return false;
        fs.write((char*)buffer.data(), buffer.size());
        return true;
    }
}
