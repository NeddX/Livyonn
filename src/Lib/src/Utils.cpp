#include "../include/Utils.h"

std::optional<std::string> ReadToString(std::string path)
{
    std::ifstream fs(path, std::ios::in);
    if (fs.is_open())
    {
        size_t size = std::filesystem::file_size(path);        
        std::string content(size, '\0');
        fs.read(content.data(), size);
        return content;
    }
    else return std::nullopt;
}