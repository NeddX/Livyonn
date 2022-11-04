#pragma once

#include <iostream>
#include <fstream>
#include <exception>
#include <optional>
#include <vector>

namespace System
{
	using namespace std;

	namespace Stack
	{
		/// <summary>
		/// Convert an unsigned 16 bit int based stack to a byte one.
		/// </summary>
		/// <param name="buffer">Reference to the uint16 based stack.</param>
		/// <returns>The byte based stack as a uint8_t vector.</returns>
		std::vector<uint8_t> Convert_16bitTo8bit(const std::vector<uint16_t>& buffer);
		/// <summary>
		/// Convert a byte based stack to a unsigned 16bit int one.
		/// </summary>
		/// <param name="buffer">Copy to the byte based stack.</param>
		/// <returns>The uint16 based stack as a uint16_t vector.</returns>
		std::vector<uint16_t> Convert_8bitTo16bit(std::vector<uint8_t> buffer);
	}

	namespace File
	{
		/// <summary>
		/// Reads the specified file to a string.
		/// </summary>
		/// <param name="path">Path to the file.</param>
		/// <returns>The string of the contents of the file, std::nullopt if failed to read.</returns>
		std::optional<std::string> ReadToString(const std::string& path);
		/// <summary>
		/// Reads the file as a byte array.
		/// </summary>
		/// <param name="path">Path to the file.</param>
		/// <returns>The byte array as a uint8_t vector, nullopt if failed to read.</returns>
		std::optional<std::vector<uint8_t>> ReadAllBytes(const std::string& path);
		/// <summary>
		/// Writes the byte array to the specified file in binary.
		/// </summary>
		/// <param name="path">Path to the file.</param>
		/// <returns>True If operation was successfull, False otherwise.</returns>
		bool WriteToBytes(const std::string& path, const std::vector<uint8_t>& buffer);
	}
}
