#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat extractRegionFromFrame(
    const Mat &frame,
    const int &xPos,
    const int &yPos,
    const int &padding
);


class ObjectTracker
{

public:
    Mat frame;
    Mat roi;
    int xPos, yPos;
    int xTracked, yTracked;
    int roiPadding, regionPadding;

    ObjectTracker(int xPos,
                  int yPos,
                  int roiPadding,
                  int regionPadding);
    void initialise(const Mat& frame);
    void setNextFrame(const Mat& frame);
    void trackROI();
};
