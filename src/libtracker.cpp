#include <cmath>
#include <opencv2/opencv.hpp>
#include "libtracker.h"

using namespace std;
using namespace cv;

Mat extractRegionFromFrame(
    const Mat &frame,
    const int &xPos,
    const int &yPos,
    const int &padding
);


ObjectTracker::ObjectTracker(int xPos,
                             int yPos,
                             int roiPadding,
                             int regionPadding)
{
    this->xPos = xPos;
    this->yPos = yPos;
    this->roiPadding = roiPadding;
    this->regionPadding = regionPadding;
}

void ObjectTracker::initialise(const Mat& frame)
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

void ObjectTracker::setNextFrame(const Mat& frame)
{
    cvtColor(frame, this->frame, COLOR_RGB2GRAY);
}

void ObjectTracker::trackROI()
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
