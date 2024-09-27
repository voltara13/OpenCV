#include <opencv2/opencv.hpp>
#include <vector>

const char* INPUT_PATH = "../../../input.jpg";
const char* WINDOW_SOURCE_NAME = "Source image";
const char* WINDOW_OUTPUT_NAME = "Output image";
const char* WINDOW_SOURCE_HIST_NAME = "Hist source";
const char* WINDOW_OUTPUT_HIST_NAME = "Hist output";

constexpr int HIST_SIZE = 256;
constexpr int HIST_W = 512;
constexpr int HIST_H = 400;

constexpr float RANGE[] = { 0, HIST_SIZE };

void printHist(const cv::Mat& iImage, const cv::Scalar& iColor, const cv::String& iWindowName)
{
  const float* histRange[] = { RANGE };

  cv::Mat hist;
  calcHist(&iImage, 1, 0, cv::Mat(), hist, 1, &HIST_SIZE, histRange, true, false);
  int bin_w = cvRound((double)HIST_W / HIST_SIZE);
  cv::Mat histImage(HIST_H, HIST_W, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
  for (int i = 1; i < HIST_SIZE; i++)
  {
    line(histImage, cv::Point(bin_w * i, HIST_H),
      cv::Point(bin_w * i, HIST_H - cvRound(hist.at<float>(i))),
      iColor, 1, 8, 0);
  }

  cv::imshow(iWindowName, histImage);
}

int main(int argc, char** argv)
{
  cv::Mat image = cv::imread(INPUT_PATH, cv::IMREAD_GRAYSCALE);
  cv::Mat equalizedImage;

  cv::equalizeHist(image, equalizedImage);
  
  cv::imshow(WINDOW_SOURCE_NAME, image);
  cv::imshow(WINDOW_OUTPUT_NAME, equalizedImage);

  printHist(image, cv::Scalar(255, 0, 0), WINDOW_SOURCE_HIST_NAME);
  printHist(equalizedImage, cv::Scalar(0, 0, 255), WINDOW_OUTPUT_HIST_NAME);

  cv::waitKey();

  return EXIT_SUCCESS;
}