#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include <optional>

std::optional<std::string> ReadToString(std::string path);