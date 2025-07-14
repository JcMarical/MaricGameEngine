#include "cdpch.h"
#include "FileSystem.h"

namespace CryDust {

	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);//快速流读入文件并定位到文件末尾，用于快速获取大小或追加数据

		if (!stream)
		{
			// Failed to open the file
			return {};
		}

		//先拿到末尾，再回到开头，计算差值获得文件大小。
		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint64_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.As<char>(), size);
		stream.close();
		return buffer;
	}

}
