#pragma once

#include "CryDust/Core/Buffer.h"

namespace CryDust {

	class FileSystem
	{
	public:
		// TODO: move to FileSystem class
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};

}
