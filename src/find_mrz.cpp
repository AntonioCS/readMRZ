#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/mat.hpp>

#include "find_mrz.h"

using cv::Mat;
using cv::Size;
using cv::MORPH_BLACKHAT;
using cv::MORPH_CLOSE;
using cv::MORPH_RECT;
using cv::COLOR_BGR2GRAY;
using cv::THRESH_BINARY;
using cv::THRESH_OTSU;
using cv::Rect;
using cv::Point;
using cv::RETR_EXTERNAL;
using cv::CHAIN_APPROX_SIMPLE;

//from https://github.com/developer79433/passport_mrz_detector_cpp
bool find_mrz(const Mat& original, Mat& mrz)
{
    // initialize a rectangular and square structuring kernel
    Mat rectKernel = getStructuringElement(MORPH_RECT, Size(13, 5));
    Mat sqKernel = getStructuringElement(MORPH_RECT, Size(21, 21));

    // resize the image and convert it to grayscale
    Mat image;
    resize(original, image, Size(original.size().width * 600 / original.size().height, 600));
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // smooth the image using a 3x3 Gaussian, then apply the blackhat
    // morphological operator to find dark regions on a light background
    GaussianBlur(gray, gray, Size(3, 3), 0);
    Mat blackhat{};
    morphologyEx(gray, blackhat, MORPH_BLACKHAT, rectKernel);

#ifdef DISPLAY_INTERMEDIATE_IMAGES
    imshow("Blackhat", blackhat);
#endif /* DISPLAY_INTERMEDIATE_IMAGES */

    // compute the Scharr gradient of the blackhat image and scale the
    // result into the range [0, 255]
    Mat gradX;
    Sobel(blackhat, gradX, CV_32F, 1, 0, -1);
    gradX = abs(gradX);
    double minVal, maxVal;
    minMaxIdx(gradX, &minVal, &maxVal);
    Mat gradXfloat = (255 * ((gradX - minVal) / (maxVal - minVal)));
    gradXfloat.convertTo(gradX, CV_8UC1);

#ifdef DISPLAY_INTERMEDIATE_IMAGES
    imshow("Gx", gradX);
#endif /* DISPLAY_INTERMEDIATE_IMAGES */

    // apply a closing operation using the rectangular kernel to close
    // gaps in between letters -- then apply Otsu's thresholding method
    morphologyEx(gradX, gradX, MORPH_CLOSE, rectKernel);
    Mat thresh;
    threshold(gradX, thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

#ifdef DISPLAY_INTERMEDIATE_IMAGES
    imshow("Horizontal closing", thresh);
#endif /* DISPLAY_INTERMEDIATE_IMAGES */

    // perform another closing operation, this time using the square
    // kernel to close gaps between lines of the MRZ, then perform a
    // series of erosions to break apart connected components
    morphologyEx(thresh, thresh, MORPH_CLOSE, sqKernel);
    Mat nullKernel;
    erode(thresh, thresh, nullKernel, Point(-1, -1), 4);

#ifdef DISPLAY_INTERMEDIATE_IMAGES
    imshow("Vertical closing", thresh);
#endif /* DISPLAY_INTERMEDIATE_IMAGES */

    // during thresholding, it's possible that border pixels were
    // included in the thresholding, so let's set 5% of the left and
    // right borders to zero
    double p = image.size().height * 0.05;
    thresh = thresh(Rect(p, p, image.size().width - 2 * p, image.size().height - 2 * p));

#ifdef DISPLAY_INTERMEDIATE_IMAGES
    imshow("Border removal", thresh);
#endif /* DISPLAY_INTERMEDIATE_IMAGES */

    // find contours in the thresholded image and sort them by their
    // size
    std::vector<std::vector<Point> > contours;
    findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // Sort the contours in decreasing area
    sort(
        contours.begin(),
        contours.end(),
        [](const std::vector<Point>& c1, const std::vector<Point>& c2) {
            return contourArea(c1, false) > contourArea(c2, false);
        }
    );

    // Find the first contour with the right aspect ratio and a large width relative to the width of the image
    Rect roiRect(0, 0, 0, 0);
    //std::vector<std::vector<Point> >::iterator
    auto border_iter = std::find_if(
        contours.begin(),
        contours.end(),
        [&roiRect, gray](std::vector<Point>& contour) {
            // compute the bounding box of the contour and use the contour to
            // compute the aspect ratio and coverage ratio of the bounding box
            // width to the width of the image
            roiRect = boundingRect(contour);
            // dump_rect("Bounding rect", roiRect);
            // pprint([x, y, w, h])
            const double aspect = static_cast<double>(roiRect.size().width) / static_cast<double>(roiRect.size().height);
            const double coverageWidth = static_cast<double>(roiRect.size().width) / static_cast<double>(gray.size().height);
            // check to see if the aspect ratio and coverage width are within
            // acceptable criteria

            return (aspect > 5 && coverageWidth > 0.5);
        });

    if (border_iter == contours.end()) {
        return false;
    }

    // Correct ROI for border removal offset
    roiRect += Point(p, p);
    // pad the bounding box since we applied erosions and now need
    // to re-grow it
    int pX = (roiRect.x + roiRect.size().width) * 0.03;
    int pY = (roiRect.y + roiRect.size().height) * 0.03;
    roiRect -= Point(pX, pY);
    roiRect += Size(pX * 2, pY * 2);
    // Ensure ROI is within image
    roiRect &= Rect(0, 0, image.size().width, image.size().height);
    // Make it relative to original image again
    float scale = static_cast<float>(original.size().width) / static_cast<float>(image.size().width);
    roiRect.x *= scale;
    roiRect.y *= scale;
    roiRect.width *= scale;
    roiRect.height *= scale;
	
    mrz = original(roiRect);

#if defined(DISPLAY_INTERMEDIATE_IMAGES)
    // Draw a bounding box surrounding the MRZ
    Mat display_roi = original.clone();
    rectangle(display_roi, roiRect, cv::Scalar(0, 255, 0), 2);
    imshow("MRZ detection results", display_roi);
#endif /* DISPLAY_INTERMEDIATE_IMAGES */

    return true;
}
