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

    void setProcessingStatus(bool status)
    {
        isActiveColorDetector = status;
    }

public:
    ColorDetector(QObject *parent = NULL) : QObject(parent)
    {
        colors = new std::vector<int*>();
        brushColors = new std::vector<int*>();
    }

    ~ColorDetector()
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

    void StartProcessing()
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

    void addColors()
    {
        colors->push_back(new int[6]{hsvMin[0],hsvMin[1],hsvMin[2],hsvMax[0],hsvMax[1],hsvMax[2]});
    }

    void addColors(int hMin, int sMin, int vMin, int hMax, int sMax, int vMax)
    {
        colors->push_back(new int[6]{hMin, sMin, vMin, hMax, sMax, vMax});
    }

    void addBrushColors()
    {
        //BRG[0] - Blue, BRG[1] - Red, BRG[2] - Green
        brushColors->push_back(new int[3]{BRG[0],BRG[1],BRG[2]});
    }

    void addBrushColors(int blue, int red, int green)
    {
        brushColors->push_back(new int[3]{blue,red,green});
    }

    void setBRG(int blue, int red, int green)
    {
        BRG[0] = blue;
        BRG[1] = red;
        BRG[2] = green;
    }

    int& getRefHsvMin(int index)
    {
        return hsvMin[index];
    }

    int& getRefHsvMax(int index)
    {
        return hsvMax[index];
    }

    std::vector<int*>* getColors()
    {
        return colors;
    }

    std::vector<int*>* getBrushColors()
    {
        return brushColors;
    }

    bool isVectorsEmpty()
    {
        return (colors->size() ? true : false);
    }
};


#endif // COLORDETECTOR_H
