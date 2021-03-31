#pragma once
#include <string>


namespace readMRZ
{
	class readMRZ
	{		
		std::string m_tesseractDataPath{};
	public:
		readMRZ() = default;
		readMRZ(const std::string& tesseractDataPath);
		~readMRZ() = default;

		std::string processImage(const std::string& imagePath) const;
		
	};
}
