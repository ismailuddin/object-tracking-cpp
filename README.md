# 🎥 Object tracking using optical flow
> Implementation from scratch of optical flow based object tracking algorithm
> using C++ and OpenCV

An implementation from scratch, of an optical flow based object tracking algorithm, written in C++, for use with video files.

## 📘 Algorithm details
The region of interest is captured from the first, frame and is compared to each subsequent frame within a specified region around the starting point. The region is compared in grayscale to each frame to find its location.


## 💻 Requirements
- CMake
- GCC / Clang, etc.
- OpenCV 4.X or newer

## 🚀 Usage
First, compile the program using `cmake`:

```shell
$   mkdir build && cd $_
$   cmake ..
$   cmake --build .
```

Then, run executable binary from the `build` directory:

```shell
$   ./build/ObjectTracker [video_file] [x_pos] [y_pos] [roi_padding] [region_padding]
```

### 🗣 Arguments

Argument | Description
---|---
video_file | The path to the video file
x_pos | The x coordinate of the centroid of the region of interest (ROI) to track
y_pos | The y coordinate of the centroid of the region of interest (ROI) to track
roi_padding | The padding around the coordinates to create square ROI
region_padding | The padding around the square ROI, to use as a search space for tracking the object


## 📜 Next steps
- [ ] Implement Boost Get Options library as an argument parser
