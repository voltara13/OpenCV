#include <stdio.h>
#include <opencv2/opencv.hpp>

const char* INPUT_PATH = "../../../input.jpg";
const char* OUTPUT_PATH = "../../../output.jpg";
const char* WINDOW_NAME = "Adaptive Threshold";

void onTrackbar(int, void*) {
}

int main(int argc, char** argv)
{
  const auto image = cv::imread(INPUT_PATH, cv::IMREAD_GRAYSCALE);

  int c = 7;
  int blockSize = 20;

  cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);

  cv::createTrackbar("C", WINDOW_NAME, &c, 50, onTrackbar);
  cv::createTrackbar("Block Size", WINDOW_NAME, &blockSize, 50, onTrackbar);

  cv::Mat destImage;

  while (cv::waitKey(30) != 'q') {
    cv::adaptiveThreshold(image, destImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize * 2 + 3 /*43*/, c /*5*/);
    cv::imshow("Adaptive Threshold", destImage);
  }

  cv::destroyAllWindows();
  cv::imwrite(OUTPUT_PATH, destImage);

  return 0;
}