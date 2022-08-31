#pragma once

#include "Wgine/Log.h"
#include <fstream>
#include <string>

namespace Wgine
{
	class FileUtils
	{
	public:
		static std::string ReadFile(const std::string &filepath)
		{
			std::ifstream file(filepath, std::ios::in, std::ios::binary);
			if (!file)
			{
				WGINE_CORE_ERROR("Trying to read from non-existent file: {0}", filepath);
				return "";
			}
			std::string result;
			file.seekg(0, std::ios::end);
			result.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(&result[0], result.size());
			file.close();

			return result;
		}
	};
}
