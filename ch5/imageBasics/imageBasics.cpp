#include <iostream>
#include <chrono>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char **argv) {
  // Read the image specified by argv[1]
  cv::Mat image;
  image = cv::imread(argv[1]); // cv::imread function reads the image at the specified path

  // Check if the image file is read correctly
  if (image.data == nullptr) { // Data does not exist, possibly the file does not exist
    cerr << "File " << argv[1] << " does not exist." << endl;
    return 0;
  }

  // File is read successfully, first output some basic information
  cout << "Image width: " << image.cols << ", height: " << image.rows << ", number of channels: " << image.channels() << endl;
  cv::imshow("image", image);      // Use cv::imshow to display the image
  cv::waitKey(0);                  // Pause the program, wait for a key input

  // Check the type of the image
  if (image.type() != CV_8UC1 && image.type() != CV_8UC3) {
    // The image type is not as required
    cout << "Please input a color image or grayscale image." << endl;
    return 0;
  }

  // Traverse the image, note that the following traversal method can also be used for random pixel access
  // Use std::chrono to time the algorithm
  chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
  for (size_t y = 0; y < image.rows; y++) {
    // Use cv::Mat::ptr to get the row pointer of the image
    unsigned char *row_ptr = image.ptr<unsigned char>(y);  // row_ptr is the pointer to the beginning of the y-th row
    for (size_t x = 0; x < image.cols; x++) {
      // Access the pixel at position (x, y)
      unsigned char *data_ptr = &row_ptr[x * image.channels()]; // data_ptr points to the pixel data to be accessed
      // Output each channel of the pixel; if it's a grayscale image, there is only one channel
      for (int c = 0; c != image.channels(); c++) {
        unsigned char data = data_ptr[c]; // data is the value of the c-th channel of I(x, y)
      }
    }
  }
  chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
  chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cout << "Time used to traverse the image: " << time_used.count() << " seconds." << endl;

  // About cv::Mat copying
  // Direct assignment does not copy data
  cv::Mat image_another = image;
  // Modifying image_another will cause changes to image
  image_another(cv::Rect(0, 0, 100, 100)).setTo(0); // Set the top-left 100x100 block to zero
  cv::imshow("image", image);
  cv::waitKey(0);

  // Use the clone function to copy data
  cv::Mat image_clone = image.clone();
  image_clone(cv::Rect(0, 0, 100, 100)).setTo(255);
  cv::imshow("image", image);
  cv::imshow("image_clone", image_clone);
  cv::waitKey(0);

  // There are many basic operations on images, such as cropping, rotation, scaling, etc. Due to limited space, they are not introduced one by one. Please refer to the OpenCV official documentation for the usage of each function.
  cv::destroyAllWindows();
  return 0;
}
