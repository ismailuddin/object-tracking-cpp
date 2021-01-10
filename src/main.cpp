#include <cmath>
#include <opencv2/opencv.hpp>
#include "libtracker.h"

using namespace std;
using namespace cv;


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

    Mat frame;
    int frameNumber = 0;

    VideoCapture video(filename);

    if (!video.isOpened())
    {
        cout << "Error opening video file" << endl;
        return -1;
    }

    video >> frame;
    ObjectTracker tracker = ObjectTracker(xPos, yPos, roiPadding, regionPadding);
    tracker.initialise(frame);

    namedWindow("Image", WINDOW_AUTOSIZE);

    while (true)
    {
        video >> frame;
        frameNumber++;
        if (frame.empty())
        {
            break;
        }
        tracker.setNextFrame(frame);
        tracker.trackROI();

        Point trackerUpperLeft(tracker.xTracked - roiPadding, tracker.yTracked - roiPadding);
        Point trackerBottomRight(tracker.xTracked + roiPadding, tracker.yTracked + roiPadding);
        Point textPos(tracker.xTracked - roiPadding, tracker.yTracked + roiPadding + 50);
        Point frameTextPos(25, 75);
        rectangle(
            frame,
            trackerUpperLeft,
            trackerBottomRight,
            Scalar(0, 255, 0),
            5
        );
        string trackingPosition = to_string(tracker.xTracked) + ", " + to_string(tracker.yTracked);
        string frameNumberText = "Frame " + to_string(frameNumber);
        putText(
            frame,
            trackingPosition,
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
        waitKey(1000/60);
    }
    video.release();
    destroyAllWindows();
    return 0;
}

