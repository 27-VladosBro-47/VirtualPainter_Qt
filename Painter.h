#ifndef PAINTER_H
#define PAINTER_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>

#include <QObject>
#include <QDebug>

class Painter : public QObject
{
    Q_OBJECT

private:
    cv::VideoCapture cap;
    cv::Mat img, imgBlur, imgHSV, imgMask;

    std::vector<int*> *colors = nullptr;
    std::vector<int*> *brushColors = nullptr;

    std::vector <std::vector<cv::Point>> contours;
    std::vector <cv::Vec4i> hierarchy;

    std::vector <std::vector<cv::Point>> contoursPoly;

    std::vector<int*> startPoints;
    int *startPointsFlags = nullptr;

    // int* --> int[3]
    // int[0] - X, int[1] - Y, int[2] - numb of brushColor (brushColor[int[2]])
    std::vector<int*> pointsVector;

    bool isActivePainter = false;


    int X, Y;

public slots:
    void setProcessingStatus(bool status);

public:
    Painter(QObject *parent = NULL);

    ~Painter();

    void setColorVectors(std::vector<int*> *colors, std::vector<int*> *brushColors);

    cv::Point findTopPoint(std::vector<cv::Point> &contoursPolyElement);

    void draw();

    void clearData();

    void StartProcessing();
};


#endif // PAINTER_H
