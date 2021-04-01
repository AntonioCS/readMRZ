#pragma once

#include <tesseract/baseapi.h>
#include <string>


namespace readMRZ
{
	class readMRZ
	{		
		std::string m_tesseractDataPath{};
		tesseract::TessBaseAPI m_ocr{};
	public:
		readMRZ() = default;
		readMRZ(const std::string& tesseractDataPath);
		~readMRZ();

		std::string processImage(const std::string& imagePath);
		
	};
}
