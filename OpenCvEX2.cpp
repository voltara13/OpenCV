#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

enum class NoiseType {
  Uniform,
  Gaussian,

  Count
};

enum class FilterType {
  Gaussian,
  Median,
  Filter2D,
  Sobel,
  Laplace,

  Count
};

double NOISE_POWER = 0.5;

NoiseType SELECTED_NOISE_TYPE = NoiseType::Uniform;
FilterType SELECTED_FILTER_TYPE = FilterType::Gaussian;

const char* INPUT_PATH = "../../../input.png";
const char* WINDOW_NAME = "Filter image";

cv::Mat getNoiseMat(const cv::Size& iSize)
{
  cv::Mat randomMat(iSize, CV_32F);

  switch (SELECTED_NOISE_TYPE) {
    case NoiseType::Uniform:
      cv::randu(randomMat, -255, 255);
      break;
    case NoiseType::Gaussian:
      constexpr float mean = 0.0;
      constexpr float stddev = 100;
      cv::randn(randomMat, mean, stddev);
      break;
  }

  return randomMat;
}

cv::Mat getNoise(const cv::Mat& iImage)
{
  const auto noiseMat = getNoiseMat(iImage.size());

  cv::Mat noiseImage;
  iImage.convertTo(noiseImage, CV_32F);
  noiseImage = noiseImage + noiseMat * NOISE_POWER;
  noiseImage.convertTo(noiseImage, iImage.type());

  return noiseImage;
}

cv::Mat getFiltered(const cv::Mat& iImage)
{
  cv::Mat filteredImage;

  const auto filter2DKernel = cv::Mat(3, 3, CV_32F, 1 / 9.0);
  const auto sobelDx = 1;
  const auto sobelDy = 0;

  switch (SELECTED_FILTER_TYPE) {
    case FilterType::Gaussian:
      cv::blur(iImage, filteredImage, cv::Size(3, 3));
      break;
    case FilterType::Median:
      cv::medianBlur(iImage, filteredImage, 3);
      break;
    case FilterType::Filter2D:
      cv::filter2D(iImage, filteredImage, -1, filter2DKernel);
      break;
    case FilterType::Sobel:
      cv::Sobel(iImage, filteredImage, -1, sobelDx, sobelDy);
      break;
    case FilterType::Laplace:
      cv::Laplacian(iImage, filteredImage, -1, 5, 10);
      break;
  }

  return filteredImage;
}

void onTrackbarNoisePower(int iPower, void*) {
  NOISE_POWER = iPower / 10.0;
}

void onTrackbarNoise(int iNoise, void*) {
  SELECTED_NOISE_TYPE = static_cast<NoiseType>(iNoise);
}

void onTrackbarFilter(int iFilter, void*) {
  SELECTED_FILTER_TYPE = static_cast<FilterType>(iFilter);
}

int main(int argc, char** argv)
{
  auto noisePower = 3;
  auto noiseType = static_cast<int>(SELECTED_NOISE_TYPE);
  auto filterType = static_cast<int>(SELECTED_FILTER_TYPE);

  const auto image = cv::imread(INPUT_PATH, cv::IMREAD_GRAYSCALE);

  cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);

  cv::createTrackbar("Power", WINDOW_NAME, &noisePower, 10, onTrackbarNoisePower);
  cv::createTrackbar("Noise", WINDOW_NAME, &noiseType, static_cast<int>(NoiseType::Count) - 1, onTrackbarNoise);
  cv::createTrackbar("Filter", WINDOW_NAME, &filterType, static_cast<int>(FilterType::Count) - 1, onTrackbarFilter);

  while (cv::waitKey(30) != 'q') {
    const auto noiseImage = getNoise(image);
    const auto filteredImage = getFiltered(noiseImage);

    cv::Mat concatenatedImages;
    cv::hconcat(std::vector<cv::Mat>{image, noiseImage, filteredImage}, concatenatedImages);
    cv::imshow(WINDOW_NAME, concatenatedImages);
  }

  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}