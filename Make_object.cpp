#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;

int main()
{
    // 1. Create a 512X512 grayscale image
    const int imageSize = 512;
    const int squareSize = 128;

    const int background = 40;
    const int foreground = 220;

    cv::Mat image(imageSize, imageSize, CV_8UC1, cv:: Scalar(background));

    //2. Calculate the square's top-left corner
    const int x0 = (imageSize - squareSize) / 2;
    const int y0 = (imageSize - squareSize) / 2;

    // 3. Draw the square
    cv::rectangle(image, cv::Rect(x0,y0,squareSize,squareSize), cv::Scalar(foreground), cv::FILLED);

    const double centerX = x0 + (squareSize - 1) / 2.0;
    const double centerY = y0 + (squareSize - 1) / 2.0;

    cout << "Square X: " << x0 << " to "
              << x0 + squareSize - 1 << '\n';

    cout << "Square Y: " << y0 << " to "
              << y0 + squareSize - 1 << '\n';

    cout << "Ground Truth Center: ("
              << centerX << ", " << centerY << ")\n";

    // 5. Save and display
    cv::imwrite("square_original.png", image);

    cv::imshow("Original Square", image);
    cv::waitKey(0);

    return 0;
}