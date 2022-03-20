#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cxxabi.h>

// Hash function
namespace Hash
{
	static const uint32_t FNV32_PRIME = 0x01000193;
	static const uint32_t FNV32_IV    = 0x811C9DC5;

	static inline uint32_t FromString(const char *string)
	{
		uint32_t accumulator = FNV32_IV;
		while (*string != '\0')
			accumulator = (accumulator ^ static_cast<uint32_t>(*string++)) * FNV32_PRIME;
		return accumulator;
	}
}

// Entry point
int main(int argc, char *argv[])
{
	// Get arguments
	if (argc < 3)
	{
		std::cout << "usage: " << argv[0] << " in.map in.h out.map" << std::endl;
		return 0;
	}

	std::string path_imap(argv[1]);
	std::string path_h(argv[2]);
	std::string path_omap(argv[3]);

	// Read public functions from .h
	std::unordered_set<std::string> public_functions;
	{
		// Open .h file
		std::ifstream stream_h(path_h);
		if (!stream_h)
		{
			std::cout << "Failed to open " << path_h << std::endl;
			return 1;
		}

		// Dump file to buffer
		std::stringstream buffer;
		buffer << stream_h.rdbuf();

		// Read function list
		size_t list_pos = 0;
		while (1)
		{
			// Find delim start
			size_t start_pos = buffer.str().find("PUBLIC(", list_pos);
			if (start_pos == std::string::npos)
				break;
			start_pos += 7;

			// Find delim end
			size_t end_pos = buffer.str().find(")", start_pos);
			if (end_pos == std::string::npos)
				break;
			list_pos = end_pos + 1;
			
			// Get function name
			public_functions.emplace(buffer.str().substr(start_pos, end_pos - start_pos));
		}
	}

	// Read symbols from .map
	std::unordered_map<std::string, uint32_t> symbols;
	{
		// Open .map file
		std::ifstream stream_imap(path_imap);
		if (!stream_imap)
		{
			std::cout << "Failed to open " << path_imap << std::endl;
			return 1;
		}

		// Read line by line
		std::string line;
		while (std::getline(stream_imap, line))
		{
			// Read function info
			std::istringstream iss(line);
			std::string name, type;
			uint32_t addr;

			iss >> name >> type >> std::hex >> addr;

			if (type == "T")
			{
				// Demangle name
				int status;    
				char *demangle = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
				if (status == -1 || status == -1)
				{
					std::cout << "Failed to demangle " << name << std::endl;
					return 1;
				}

				std::string name_demangle = name;
				if (demangle != nullptr)
				{
					name_demangle = demangle;
					free(demangle);
				}

				// Cut arguments out of name
				size_t argument_start = name_demangle.find_first_of('(');
				if (argument_start != std::string::npos)
					name_demangle = name_demangle.substr(0, argument_start);

				// Push to symbol map if defined in public header
				if (public_functions.find(name_demangle) != public_functions.end())
					symbols.emplace(std::make_pair(name, addr));
			}
		}
	}

	// Check for hash conflicts
	std::unordered_map<uint32_t, std::string> hashes;
	for (auto &i : symbols)
	{
		uint32_t hash = Hash::FromString(i.first.c_str());
		auto hash_find = hashes.find(hash);
		if (hash_find != hashes.end())
		{
			std::cout << "Hash conflict between " << i.first << " and " << hash_find->second << std::endl;
			return 1;
		}
		else
		{
			hashes.emplace(std::make_pair(hash, i.first));
		}
	}

	// Write symbol map
	{
		// Open .map file
		std::ofstream stream_omap(path_omap, std::ofstream::binary);
		if (!stream_omap)
		{
			std::cout << "Failed to open " << path_omap << std::endl;
			return 1;
		}

		// Write symbol map
		uint32_t write_size = symbols.size();
		stream_omap.write((char*)&write_size, 4);

		for (auto &i : symbols)
		{
			uint32_t hash = Hash::FromString(i.first.c_str());
			stream_omap.write((char*)&hash, 4);
			uint32_t addr = i.second;
			stream_omap.write((char*)&addr, 4);
		}
	}

	return 0;
}
