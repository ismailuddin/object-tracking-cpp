#include <vector>
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
    Mat roi, region;
    int xPos, yPos;
    int xTracked, yTracked;
    int roiPadding, regionPadding;

    ObjectTracker(
        int xPos,
        int yPos,
        int roiPadding,
        int regionPadding) : xPos(xPos),
                             yPos(yPos),
                             roiPadding(roiPadding),
                             regionPadding(regionPadding)
    {
    }

    void initialise(const Mat& frame)
    {
        cvtColor(frame, this->frame, COLOR_RGB2GRAY);
        this->roi = extractRegionFromFrame(this->frame,
                                           this->xPos,
                                           this->yPos,
                                           this->roiPadding);
        // this->nextFrame = nextFrame;
        this->xTracked = this->xPos;
        this->yTracked = this->yPos;
    }

    void setNextFrame(const Mat &frame)
    {
        cvtColor(frame, this->frame, COLOR_RGB2GRAY);
    }

    void trackRoiInRegion()
    {
        Mat proposedRegion, difference, bestRegion;
        int x, y;
        double roiMean;
        double lowestMean = 1E6;
        for (int _y = -1 * regionPadding; _y < regionPadding; _y++)
        {
            for (int _x = -1 * regionPadding; _x < regionPadding; _x++)
            {
                x = this->xTracked + _x;
                y = this->yTracked + _y;
                // Frame bounds check
                if (
                    x - this->roiPadding < 0 ||
                    y - this->roiPadding < 0 ||
                    x + this->roiPadding > this->frame.cols ||
                    y + this->roiPadding > this->frame.rows
                )
                {
                    continue;
                }
                proposedRegion = extractRegionFromFrame(this->frame,
                                                        x,
                                                        y,
                                                        this->roiPadding);
                absdiff(this->roi, proposedRegion, difference);
                roiMean = mean(difference)[0];
                if ( roiMean < lowestMean )
                {
                    lowestMean = roiMean;
                    bestRegion = proposedRegion.clone();
                    if ( abs(this->xTracked - x) < 5 )
                    {
                        this->xTracked = x;
                    }
                    if ( abs(this->yTracked - y) < 5 )
                    {
                        this->yTracked = y;
                    }
                }
            }
        }
        // this->roi = bestRegion;
    }
};

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        cout << argc << endl;
        printf("Usage:\nTrackObject <Video_path> <X_pos> <Y_pos> <RoiPadding> <RegionPadding>\n");
        return -1;
    }
    string filename = argv[1];
    int xPos = stoi(argv[2]);
    int yPos = stoi(argv[3]);
    int roiPadding = stoi(argv[4]);
    int regionPadding = stoi(argv[5]);

    VideoCapture video(filename);
    Mat frame;
    // Capture first frame
    video >> frame;
    // video >> _frame2;

    if (!video.isOpened())
    {
        cout << "Error opening video file" << endl;
        return -1;
    }
    ObjectTracker tracker = ObjectTracker(xPos, yPos, roiPadding, regionPadding);
    tracker.initialise(frame);
    int frameNumber = 0;
    namedWindow("Image", WINDOW_AUTOSIZE);
    while (true)
    {
        video >> frame;
        frameNumber++;
        // cout << ">>>>>>>>>>>>>>>>>>>>> Frame " << frameNumber << endl;
        if (frame.empty())
        {
            break;
        }
        tracker.setNextFrame(frame);
        tracker.trackRoiInRegion();
        Point pt1(tracker.xTracked - roiPadding, tracker.yTracked - roiPadding);
        Point pt2(tracker.xTracked + roiPadding, tracker.yTracked + roiPadding);
        Point textPos(tracker.xTracked - roiPadding, tracker.yTracked + roiPadding + 50);
        Point frameTextPos(25, 75);
        rectangle(
            frame,
            pt1,
            pt2,
            Scalar(0, 255, 0),
            5
        );
        string positionText = to_string(tracker.xTracked) + ", " + to_string(tracker.yTracked);
        string frameNumberText = "Frame " + to_string(frameNumber);
        putText(
            frame,
            positionText,
            textPos,
            FONT_HERSHEY_SIMPLEX,
            1,
            Scalar(0, 255, 0),
            2
        );
        putText(
            frame,
            frameNumberText,
            frameTextPos,
            FONT_HERSHEY_SIMPLEX,
            1,
            Scalar(0, 255, 0),
            2
        );
        imshow("Image", frame);
        // imshow("Image", tracker.roi);
        waitKey(1000/60);
        // waitKey(0);
        // char c = (char)waitKey(25);
        // if (c == 27)
        // {
        //     break;
        // }
    }
    video.release();
    destroyAllWindows();
    return 0;
}

Mat extractRegionFromFrame(
    const Mat &frame,
    const int &xPos,
    const int &yPos,
    const int &padding)
{
    int x, y;
    x = xPos - padding;
    y = yPos - padding;
    Rect rect = Rect(x, y, padding * 2, padding * 2);
    return frame(rect).clone();
}
