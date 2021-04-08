#include "ColorDetector.h"

void ColorDetector::setProcessingStatus(bool status)
{
    isActiveColorDetector = status;
}

ColorDetector::ColorDetector(QObject *parent) : QObject(parent)
{
    colors = new std::vector<int*>();
    brushColors = new std::vector<int*>();
}

ColorDetector::~ColorDetector()
{
    for(auto &count : *colors)
    {
        delete[] count;
    }
    delete colors;

    for(auto &count : *brushColors)
    {
        delete[] count;
    }
    delete brushColors;
}

void ColorDetector::StartProcessing()
{
    cap.open(0);

    while(isActiveColorDetector)
    {
        cap.read(img);

        cv::Scalar lower(hsvMin[0],hsvMin[1],hsvMin[2]);
        cv::Scalar upper(hsvMax[0],hsvMax[1],hsvMax[2]);

        cv::GaussianBlur(img, imgBlur, cv::Size(3,3), 3.0, 0.0);
        cv::cvtColor(imgBlur, imgHSV, cv::COLOR_BGR2HSV);
        cv::inRange(imgHSV, lower, upper, imgMask);

        cv::imshow("Image", img);
        cv::imshow("Image Mask", imgMask);
        cv::waitKey(1);
    }

    cv::destroyAllWindows();
    cap.release();
}

void ColorDetector::addColors()
{
    colors->push_back(new int[6]{hsvMin[0],hsvMin[1],hsvMin[2],hsvMax[0],hsvMax[1],hsvMax[2]});
}

void ColorDetector::addColors(int hMin, int sMin, int vMin, int hMax, int sMax, int vMax)
{
    colors->push_back(new int[6]{hMin, sMin, vMin, hMax, sMax, vMax});
}

void ColorDetector::addBrushColors()
{
    //BRG[0] - Blue, BRG[1] - Red, BRG[2] - Green
    brushColors->push_back(new int[3]{BRG[0],BRG[1],BRG[2]});
}

void ColorDetector::addBrushColors(int blue, int red, int green)
{
    brushColors->push_back(new int[3]{blue,red,green});
}

void ColorDetector::setBRG(int blue, int red, int green)
{
    BRG[0] = blue;
    BRG[1] = red;
    BRG[2] = green;
}

int& ColorDetector::getRefHsvMin(int index)
{
    return hsvMin[index];
}

int& ColorDetector::getRefHsvMax(int index)
{
    return hsvMax[index];
}

std::vector<int*>* ColorDetector::getColors()
{
    return colors;
}

std::vector<int*>* ColorDetector::getBrushColors()
{
    return brushColors;
}

bool ColorDetector::isVectorsEmpty()
{
    return (colors->size() ? true : false);
}

