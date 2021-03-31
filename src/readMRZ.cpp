
#include <readMRZ/readMRZ.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/mat.hpp>
#include <tesseract/baseapi.h>
#include <algorithm>
#include "find_mrz.h"

namespace readMRZ
{
	readMRZ::readMRZ(const std::string& tesseractDataPath) : m_tesseractDataPath(tesseractDataPath)
	{
		
	}

	std::string readMRZ::processImage(const std::string& imagePath) const
	{
		cv::Mat imgGrey = cv::imread(imagePath); // , cv::IMREAD_GRAYSCALE);
		cv::Mat mrz{};
		std::string result{};

		if (imgGrey.empty()) {
			throw std::runtime_error{"Could not read image: " + imagePath};
		}

		if (find_mrz(imgGrey, mrz)) {
			cv::Mat roi_grey;
			cvtColor(mrz, roi_grey, cv::COLOR_BGR2GRAY);
			cv::Mat roi_thresh;
			threshold(roi_grey, roi_thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

			tesseract::TessBaseAPI ocr{};
			ocr.Init(!m_tesseractDataPath.empty() ? m_tesseractDataPath.c_str() : nullptr, "eng", tesseract::OEM_LSTM_ONLY);
			ocr.SetPageSegMode(tesseract::PSM_AUTO);			
			ocr.SetImage(static_cast<uchar*>(roi_thresh.data), roi_thresh.size().width, roi_thresh.size().height, roi_thresh.channels(), roi_thresh.step1());
			ocr.Recognize(0);

			result = ocr.GetUTF8Text();
			ocr.End();
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
