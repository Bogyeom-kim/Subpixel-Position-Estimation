
#include <opencv2/opencv.hpp>
#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include <string>

int main()
{
    // =========================================================
    // 1. Load the input image
    // =========================================================

    cv::Mat image = cv::imread("square_original.png", cv::IMREAD_GRAYSCALE);

    if (image.empty())
    {
        std::cerr << "Failed to load image!" << std::endl;
        return -1;
    }

    // Display basic image information
    std::cout << "Image Width: " << image.cols << std::endl;
    std::cout << "Image Height: " << image.rows << std::endl;


    // =========================================================
    // 2. Calculate the intensity histogram
    // =========================================================

    // Initialize 256 histogram bins for 8-bit grayscale values
    std::array<int, 256> histogram{};

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            uchar intensity = image.at<uchar>(y, x);
            histogram[intensity]++;
        }
    }

    // Print histogram bins with nonzero pixel counts
    std::cout << "\nIntensity Histogram\n";

    for (int i = 0; i < 256; i++)
    {
        if (histogram[i] > 0)
        {
            std::cout << "Intensity " << i
                      << ": " << histogram[i]
                      << " pixels" << std::endl;
        }
    }


    // =========================================================
    // 3. Estimate the background intensity
    // =========================================================

    // Assume the background is the most frequent intensity
    int background = 0;

    for (int i = 1; i < 256; i++)
    {
        if (histogram[i] > histogram[background])
        {
            background = i;
        }
    }

    std::cout << "\nEstimated Background: "
              << background << std::endl;


    // =========================================================
    // 4. Calculate the intensity-weighted centroid
    // =========================================================

    double sumWeight = 0.0;
    double sumX = 0.0;
    double sumY = 0.0;

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            int intensity = image.at<uchar>(y, x);

            // Subtract the background and suppress negative values
            double weight = std::max(0, intensity - background);

            // Accumulate the signal weight and spatial moments
            sumWeight += weight;
            sumX += x * weight;
            sumY += y * weight;
        }
    }

    // Prevent division by zero when no positive signal exists
    if (sumWeight <= 0.0)
    {
        std::cerr << "No object signal detected." << std::endl;
        return -1;
    }

    // Estimate the centroid coordinates
    double centerX = sumX / sumWeight;
    double centerY = sumY / sumWeight;

    std::cout << "\nCentroid Measurement Results\n";
    std::cout << "Total Weight: " << sumWeight << std::endl;
    std::cout << "Centroid X: " << centerX << std::endl;
    std::cout << "Centroid Y: " << centerY << std::endl;


    // =========================================================
    // 5. Visualize the measured centroid
    // =========================================================

    // Convert grayscale to BGR for colored annotations
    cv::Mat displayImage;
    cv::cvtColor(image, displayImage, cv::COLOR_GRAY2BGR);

    // Convert subpixel coordinates to integer display coordinates
    cv::Point centerPoint(
        static_cast<int>(std::round(centerX)),
        static_cast<int>(std::round(centerY))
    );

    // Draw a blue horizontal crosshair
    cv::line(
        displayImage,
        cv::Point(centerPoint.x - 20, centerPoint.y),
        cv::Point(centerPoint.x + 20, centerPoint.y),
        cv::Scalar(255, 0, 0),
        1,
        cv::LINE_AA
    );

    // Draw a blue vertical crosshair
    cv::line(
        displayImage,
        cv::Point(centerPoint.x, centerPoint.y - 20),
        cv::Point(centerPoint.x, centerPoint.y + 20),
        cv::Scalar(255, 0, 0),
        1,
        cv::LINE_AA
    );

    // Draw a filled red circle at the measured centroid
    cv::circle(
        displayImage,
        centerPoint,
        5,
        cv::Scalar(0, 0, 255),
        cv::FILLED
    );


    // =========================================================
    // 6. Display and save the measurement results
    // =========================================================

    // Format the centroid coordinates to one decimal place
    std::string text = cv::format(
        "Center: (%.1f, %.1f)", centerX, centerY
    );

    // Overlay the measured coordinates on the image
    cv::putText(
        displayImage,
        text,
        cv::Point(20, 30),
        cv::FONT_HERSHEY_SIMPLEX,
        0.7,
        cv::Scalar(0, 255, 0),
        2
    );

    // Save and display the annotated image
    cv::imwrite("centroid_result.png", displayImage);
    cv::imshow("Centroid Measurement", displayImage);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
