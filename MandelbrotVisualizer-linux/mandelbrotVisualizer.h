#ifndef MANDELBROTVISUALIZER_H
#define MANDELBROTVISUALIZER_H

#include <vector>
#include <string>
#include <complex>

//windows
//#include <windows.h>

class mandelbrotVisualizer{
private:




public:

    long double maxIterations = 1000;
    long double zoomMultiplier = 2;
    std::vector<std::vector<std::string>> screenLists;
    std::vector<long double> pointsIterations = {};
    long double iterationAverage;
    long double notOnSetNumber;

    long double maxIterationNotOnSet;
    long double minIterationNotOnSet;
    bool allPointsOnSet = true;

    std::vector<int> percentagesList = {0, 25, 50, 75, 99};
    std::vector<long double> screenSize = {81, 41};

    long double distanceX;
    long double distanceY;
    
    std::vector<long double> currentComplex = {0, 0};

    std::vector<std::vector<long double>> baseSetOfNumber = {{-2.0, 2},{-1, 1}};
    std::vector<std::vector<long double>> currentSetOfNumber = baseSetOfNumber;
    std::vector<std::vector<std::vector<long double>>> allSetsOfNumbers;

    std::vector<long double> baseMiddlePointCoordinate = {40, 20};
    std::vector<long double> middlePointCoordinate = baseMiddlePointCoordinate;

    bool playing = true;
    bool hasToCreateMandelBrot;
    bool firstIteration = true;
    bool hasToGoBack = false;
    bool onPercentageMode = false;


    void displayScreen();
    void createMandelBrotSet();
    bool isPartOfMandelBrot(std::complex<long double> c);

    void setDistances(std::vector<std::vector<long double>> setOfNumber);
    void setScreenSize();
    void changeMaxIteration();
    int countLeadingZeros(long double num);
    std::vector<long double> getDimensionSet(long double firstCoordinate, long double secondCoordinate, long double screenProportion, long double distance);

    void reset();
    void changeZoom(int zoom);
    
    void goBack(int iterationNumber);
    

};



#endif 
