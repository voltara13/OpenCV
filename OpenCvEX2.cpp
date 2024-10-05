#include <opencv2/opencv.hpp>
#include <vector>
#include <random>
#include <iostream>

const char* INPUT_PATH = "../../../input.png";
const char* WINDOW_SOURCE_NAME = "Source image";
const char* WINDOW_NOISE_NAME = "Noise image";
const char* WINDOW_OUTPUT_NAME = "Output image";

constexpr int NOISE_COUNT = 3;
constexpr double NOISE_POWER = 0.1;

uchar findAvg(const std::vector<uchar>& arr) 
{
  auto sum = 0;

  for (const auto& value : arr)
    sum += value;

  return cv::saturate_cast<uchar>(sum / arr.size());
}

std::vector<cv::Mat> getNoise(const cv::Mat& iImage)
{
  std::vector<cv::Mat> noiseImages;

  const auto rows = iImage.rows;
  const auto cols = iImage.cols;

  for (int i = 0; i < NOISE_COUNT; ++i)
    noiseImages.emplace_back(iImage.size(), iImage.type());

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<> dist(0, 511);

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      for (int i = 0; i < noiseImages.size(); ++i) {
        noiseImages[i].at<uchar>(row, col) = cv::saturate_cast<uchar>(iImage.at<uchar>(row, col) + (dist(rng) - 256) * NOISE_POWER);
      }
    }
  }

  return noiseImages;
}

cv::Mat replaceImage(const std::vector<cv::Mat>& iNoises)
{
  if (iNoises.empty())
    return {};

  const auto rows = iNoises[0].rows;
  const auto cols = iNoises[0].cols;

  cv::Mat replaced(rows, cols, CV_8UC1, cv::Scalar(0));

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      std::vector<uchar> pixelsVariant(iNoises.size());

      for (int i = 0; i < iNoises.size(); ++i)
        pixelsVariant[i] = iNoises[i].at<uchar>(row, col);

      replaced.at<uchar>(row, col) = findAvg(pixelsVariant);
    }
  }

  return replaced;
}

int main(int argc, char** argv)
{
  auto image = cv::imread(INPUT_PATH, cv::IMREAD_GRAYSCALE);
  auto noiseImages = getNoise(image);
  
  cv::imshow(WINDOW_SOURCE_NAME, image);
 
  for (int i = 0; i < noiseImages.size(); ++i) {
    auto name = cv::String(WINDOW_NOISE_NAME);
    name += "_" + std::to_string(i);

    cv::imshow(name, noiseImages[i]);
  }

  const auto replacedImage = replaceImage(noiseImages);
  cv::imshow(WINDOW_OUTPUT_NAME, replacedImage);

  cv::waitKey();

  return EXIT_SUCCESS;
}