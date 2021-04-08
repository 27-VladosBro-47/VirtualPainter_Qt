#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>

#include <QObject>
#include <QDebug>

class ColorDetector : public QObject
{
    Q_OBJECT

private:
    std::vector<int*> *colors = nullptr;
    std::vector<int*> *brushColors = nullptr;

    int hsvMin[3] = {0,0,0};
    int hsvMax[3] = {179, 255, 255};

    int BRG[3];

    cv::VideoCapture cap;
    cv::Mat img, imgBlur, imgHSV, imgMask;

    bool isActiveColorDetector = false;

public slots:

    void setProcessingStatus(bool status);

public:
    ColorDetector(QObject *parent = NULL);

    ~ColorDetector();

    void StartProcessing();

    void addColors();

    void addColors(int hMin, int sMin, int vMin, int hMax, int sMax, int vMax);

    void addBrushColors();

    void addBrushColors(int blue, int red, int green);

    void setBRG(int blue, int red, int green);

    int& getRefHsvMin(int index);

    int& getRefHsvMax(int index);

    std::vector<int*>* getColors();

    std::vector<int*>* getBrushColors();

    bool isVectorsEmpty();
};


#endif // COLORDETECTOR_H
