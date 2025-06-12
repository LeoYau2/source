#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat image;
    image = imread("lena.jpg", IMREAD_COLOR);

    imshow("view", image);
    waitKey(0);
    return 0;
}
