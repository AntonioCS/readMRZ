
#include <readMRZ/readMRZ.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/mat.hpp>
#include <tesseract/baseapi.h>

#include "find_mrz.h"

namespace readMRZ
{
	readMRZ::readMRZ(const std::string& tesseractDataPath) : m_tesseractDataPath(tesseractDataPath)
	{
		m_ocr.SetVariable("debug_file", "/dev/null"); //suppress all warnings

		m_ocr.Init(!m_tesseractDataPath.empty() ? m_tesseractDataPath.c_str() : nullptr, "ocrb");// , tesseract::OEM_LSTM_ONLY);
		m_ocr.SetPageSegMode(tesseract::PSM_AUTO);
	}

	readMRZ::~readMRZ()
	{
		m_ocr.End();
	}

	std::string readMRZ::processImage(const std::string& imagePath)
	{
		cv::Mat img{ cv::imread(imagePath) };

		if (img.empty()) {
			throw std::runtime_error{"Could not read image: " + imagePath};
		}

		cv::Mat mrz{};
		std::string result{};

		if (find_mrz(img, mrz)) {
			m_ocr.SetImage(static_cast<uchar*>(mrz.data), mrz.size().width, mrz.size().height, mrz.channels(), mrz.step1());
			m_ocr.SetSourceResolution(209);
			m_ocr.Recognize(nullptr);

			result = m_ocr.GetUTF8Text();
		} else {
			throw std::runtime_error{ "Could not extract MRZ from image" };
		}

		if (!result.empty()) {
			result.erase(
				std::remove(
					result.begin(), 
					result.end(), 
					'\n'
				), 
				result.end()
			);
		}

		return result;
	}
}
