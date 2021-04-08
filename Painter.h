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
    void setProcessingStatus(bool status)
    {
        isActivePainter = status;
    }

public:
    Painter(QObject *parent = NULL) : QObject(parent)
    {

    }

    ~Painter()
    {

    }

    void setColorVectors(std::vector<int*> *colors, std::vector<int*> *brushColors)
    {
        this->colors = colors;
        this->brushColors = brushColors;
    }

    cv::Point findTopPoint(std::vector<cv::Point> &contoursPolyElement)
    {
        if(contoursPolyElement.size()>1)
        {
            cv::Point topPoint = contoursPolyElement[0];

            for(int i = 1; i < contoursPolyElement.size(); i++)
            {
                if(contoursPolyElement[i].y < topPoint.y)
                {
                    topPoint = contoursPolyElement[i];
                }
            }

            return topPoint;
        }
        else
        {
            return contoursPolyElement[0];
        }
    }

    void draw()
    {
        std::vector<int*> buffPoints;

        for(int i = 0; i < brushColors->size(); i++)
        {
            buffPoints.push_back(new int[2]{startPoints[i][0],startPoints[i][1]});
        }

        int colorNumb;

        for(int i = 0; i < pointsVector.size(); i++)
        {
            colorNumb = pointsVector[i][2];
            cv::circle(img, cv::Point(pointsVector[i][0],pointsVector[i][1]), 8, cv::Scalar((*brushColors)[colorNumb][0],(*brushColors)[colorNumb][1],(*brushColors)[colorNumb][2]), cv::FILLED);

            cv::line(img, cv::Point(buffPoints[colorNumb][0],buffPoints[colorNumb][1]), cv::Point(pointsVector[i][0],pointsVector[i][1]), cv::Scalar((*brushColors)[colorNumb][0],(*brushColors)[colorNumb][1],(*brushColors)[colorNumb][2]), 15);

            buffPoints[colorNumb][0] = pointsVector[i][0];
            buffPoints[colorNumb][1] = pointsVector[i][1];

        }


        for(auto &count : buffPoints)
        {
            delete[] count;
        }
        buffPoints.clear();

    }

    void clearData()
    {
        for(auto &element : pointsVector)
        {
            delete[] element;
        }

        pointsVector.clear();

        delete[] startPointsFlags;
        for(auto &count : startPoints)
        {
            delete[] count;
        }
        startPoints.clear();


        startPointsFlags = new int[brushColors->size()]{0};

        for(int i = 0; i < brushColors->size(); i++)
        {
            startPointsFlags[i] = 0;
            startPoints.push_back(new int[2]{0,0});
        }

    }

    void StartProcessing()
    {
        clearData();

        cap.open(0);

        while(isActivePainter)
        {
            cap.read(img);
            cv::GaussianBlur(img, imgBlur, cv::Size(3,3), 3.0, 0.0);
            cv::cvtColor(imgBlur, imgHSV, cv::COLOR_BGR2HSV);

            for(int i = 0; i < colors->size(); i++)
            {
                cv::Scalar lower((*colors)[i][0], (*colors)[i][1], (*colors)[i][2]);
                cv::Scalar upper((*colors)[i][3], (*colors)[i][4], (*colors)[i][5]);

                cv::inRange(imgHSV, lower, upper, imgMask);
                cv::findContours(imgMask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

                contoursPoly.resize(contours.size());

                for(int numbOfContours = 0; numbOfContours < contours.size(); numbOfContours++)
                {
                    int area = cv::contourArea(contours[numbOfContours]);
                    if(area > 1200)
                    {
                        double perimeter = cv::arcLength(contours[numbOfContours], true);
                        cv::approxPolyDP(contours[numbOfContours], contoursPoly[numbOfContours], perimeter*0.03, true);

                        cv::Point point = findTopPoint(contoursPoly[numbOfContours]);
                        if(startPointsFlags[i]==0)
                        {
                            X = startPoints[i][0] = point.x;
                            Y = startPoints[i][1] = point.y;
                            startPointsFlags[i] = 1;
                        }
                        else
                        {
                            pointsVector.push_back(new int[3]{point.x, point.y, i});
                        }

                    }
                }
            }

            draw();
            cv::imshow("Image", img);
            cv::waitKey(1);
        }

        cv::destroyAllWindows();
        cap.release();
    }
};


#endif // PAINTER_H
