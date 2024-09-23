#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>

//Ex3_2//

const char* INPUT_PATH = "../../../input.jpg";
const char* OUTPUT_PATH = "../../../output.jpg";
const char* WINDOW_NAME = "Brightness and contrast correction. Linear and gamma correction";

const double ALPHA_MAX = 3.0;
const double ALPHA_MIN = 0.0;

const int BETA_MAX = 100;
const int BETA_MIN = -100;

const double GAMMA_MAX = 3.0;
const double GAMMA_MIN = 0.1;


void onTrackbar(int, void*) {
}

void adjustBrightnessContrastGamma(cv::Mat& iImage, double iAlpha, int iBeta, int iGamma) 
{
  for (auto it = iImage.begin<cv::Vec3b>(); it < iImage.end<cv::Vec3b>(); ++it) {
    for (int channel = 0; channel < iImage.channels(); channel++) {
      auto& pixelChannel = (*it)[channel];
      pixelChannel = cv::saturate_cast<uchar>(iAlpha * (pow(pixelChannel / 255.0, iGamma) * 255.0) + iBeta);
    }
  }
}

void adjustGamma(cv::Mat& iImage, double iGamma) 
{
  cv::Mat lookUpTable(1, 256, CV_8U);

  for (int i = 0; i < 256; i++)
    lookUpTable.at<uchar>(i) = cv::saturate_cast<uchar>(pow(i / 255.0, iGamma) * 255.0);

  cv::LUT(iImage, lookUpTable, iImage);
}

int main(int argc, char** argv)
{
  const auto image = cv::imread(INPUT_PATH, cv::IMREAD_COLOR);

  cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);

  int alpha = 100;
  int beta = 100;
  int gamma = 100;

  cv::createTrackbar("Contrast", WINDOW_NAME, &alpha, ALPHA_MAX * 100, onTrackbar);
  cv::createTrackbar("Brightness", WINDOW_NAME, &beta, BETA_MAX - BETA_MIN, onTrackbar);
  cv::createTrackbar("Gamma", WINDOW_NAME, &gamma, GAMMA_MAX * 100, onTrackbar);

  cv::Mat destImage;

  while (cv::waitKey(30) != 'q') {
    const auto normalizedAlpha = std::max(alpha / 100.0, ALPHA_MIN);
    const auto normalizedBeta = beta - BETA_MAX;
    const auto normalizedGamma = std::max(gamma / 100.0, GAMMA_MIN);

    destImage = image.clone();

    image.convertTo(destImage, -1, normalizedAlpha, normalizedBeta); // adjustBrightnessContrast
    adjustGamma(destImage, normalizedGamma);

    //adjustBrightnessContrastGamma(destImage, normalizedAlpha, normalizedBeta, normalizedGamma);
    
    cv::imshow(WINDOW_NAME, destImage);
  }

  cv::destroyAllWindows();
  cv::imwrite(OUTPUT_PATH, destImage);

  return 0;
}