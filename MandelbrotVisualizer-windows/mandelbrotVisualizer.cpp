#include "mandelbrotVisualizer.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <cmath> 
#include <list>
#include <algorithm>    
#include <complex>


void mandelbrotVisualizer::displayScreen(){
    iterationAverage /= notOnSetNumber;

    int l = 0;

    for (int i = 0; i < screenLists.size(); i++) {
        std::vector<std::string> partList = screenLists[i];

        //display y screen coordinates
        int currentNumber = screenSize[1] - 1 - i;
        if(!onPercentageMode){
            if(currentNumber <= 9){
                std::cout << "0" << currentNumber;
            }
            else{
                std::cout << currentNumber;
            }            
        }
        
        else{
            if(round((currentNumber * 100) / screenSize[1]) <= percentagesList[percentagesList.size() - 1 - l]){
                if (percentagesList[percentagesList.size() - 1 - l] == 0){
                    std::cout << "0";
                }
                std::cout << percentagesList[percentagesList.size() - 1 - l];
                l++;
            }
            else{
                std::cout << "  ";
            }
        }

        //dislay every pixel on screen
        for (int j = 0; j < partList.size(); j++) {
            long double currentIteration = pointsIterations[i * screenSize[0] + j];

            std::string rByte = "50";
            std::string gByte;
            std::string bByte;

            if(minIterationNotOnSet < 30){
                gByte = std::to_string(int(std::min((currentIteration / (iterationAverage * 1)) * 255, 255.0L)));
                bByte = std::to_string(int(std::min((currentIteration / (iterationAverage * 2)) * 255, 255.0L)));


            }
            else if (!allPointsOnSet){

                currentIteration = std::min(currentIteration, maxIterationNotOnSet);
                long double IterationDifference = maxIterationNotOnSet - minIterationNotOnSet;
                long double currentIterationDifference = maxIterationNotOnSet - currentIteration;
                gByte = std::to_string(int(std::min(((IterationDifference - currentIterationDifference) /
                                    (currentIterationDifference)) * 255, 255.0L)));
                bByte = std::to_string(int(std::min(((IterationDifference - currentIterationDifference) /
                                (currentIterationDifference)) * 255, 255.0L)));
            }
            else{
                gByte = "255";
                bByte = "255";
            }
            if(std::stof(gByte) < 0.0L || std::stof(bByte) < 0.0L ){
                gByte = "0";
                bByte = "0";
            }
            if(std::stof(gByte) > 255.0L || std::stof(bByte) > 255.0L){
                gByte = "255";
                bByte = "255";
            }
        
            std::stringstream string; 
            string << "\033[38;2;"<<rByte << ";" << gByte << ";" << bByte << "m"<< partList[j] << "\033[0m";
            std::cout << string.str() << " "; 
        }
        std::cout << std::endl;
    }


    //display x screen coordinates
    int m = 0;
    for (int k = 0; k < 2; k++){

        std::cout << "  ";
        for (int j = 0; j < screenSize[0];j++){
            if(!onPercentageMode){
                if(k == 0){
                    std::cout << std::floor(j / 10) << " ";
                }
                else{
                    std::cout << std::to_string(j).back() << " ";
                }
            }
            else{
                if(m < percentagesList.size()){
                    if(std::round(((j + 1) * 100) / screenSize[0]) >= percentagesList[m]){
                        if ( percentagesList[m] == 0){
                            std::cout << "0";
                        }
                        std::cout << percentagesList[m];
                        m++;
                        }
                    else{
                        std::cout << "  ";
                    }
                }
              
            }
        }
        std::cout << std::endl;
    }

    screenLists.clear();
    pointsIterations.clear();
    iterationAverage = 0;
    notOnSetNumber = 0;
    allPointsOnSet = true;
}



void mandelbrotVisualizer::createMandelBrotSet(){
    std::cout << "\nCette visualistion vous est proposee par Nerter : https://nerter.fr" << std::endl;

    changeMaxIteration();

    setScreenSize();
    std::vector<std::string> partList;

    
    if(!firstIteration && !hasToGoBack){
        allSetsOfNumbers.push_back(currentSetOfNumber);

        if(onPercentageMode){
            middlePointCoordinate[0] = middlePointCoordinate[0] * screenSize[0] / 100;
            middlePointCoordinate[1] = middlePointCoordinate[1] * screenSize[1] / 100;

        }
        long double screenProportionX = (middlePointCoordinate[0] / screenSize[0]);
        long double screenProportionY = (middlePointCoordinate[1] / screenSize[1]);

        setDistances(currentSetOfNumber);
        currentSetOfNumber = {getDimensionSet(currentSetOfNumber[0][0], currentSetOfNumber[0][1], screenProportionX, distanceX) ,
                            getDimensionSet(currentSetOfNumber[1][0], currentSetOfNumber[1][1], screenProportionY, distanceY)};
    }
    
    if(hasToGoBack){
        hasToGoBack = false;
    }
    
    maxIterationNotOnSet = 0;
    minIterationNotOnSet = maxIterations;

    for (long double i = 0; i < screenSize[1]; i++){ //y
        for (long double j = 0; j < screenSize[0]; j++){ //x

            setDistances(currentSetOfNumber);
            
            std::complex<long double> c((j / screenSize[0]) * distanceX + currentSetOfNumber[0][0],
                                   (1 - (i / screenSize[1])) * distanceY + currentSetOfNumber[1][0]);

            if(isPartOfMandelBrot(c)){
                partList.push_back("#");
            }
            else{
                partList.push_back(".");
            }

            if(i == std::round(screenSize[1] / 2) && j == std::round(screenSize[0] / 2)){
                currentComplex[0] = c.real();
                currentComplex[1] = c.imag();

            }
        }
        screenLists.push_back(partList);
        partList.clear();
    }
    displayScreen();
    firstIteration = false;
}

bool mandelbrotVisualizer::isPartOfMandelBrot(std::complex<long double> c){
    std::complex<long double> z = 0;

    for (int i = 0; i < maxIterations; i++){
        z = std::pow(z, 2) + c;

        if(std::abs(z) > 2){
            pointsIterations.push_back(i);
            iterationAverage += i;
            notOnSetNumber ++;

            maxIterationNotOnSet = std::max(maxIterationNotOnSet, (long double)(i));
            minIterationNotOnSet = std::min(minIterationNotOnSet, (long double)(i));

            allPointsOnSet = false;

            return false;
        }
    }
    pointsIterations.push_back(maxIterations);
    return true;
}

void mandelbrotVisualizer::setDistances(std::vector<std::vector<long double>> setOfNumber){
    distanceX = std::abs(setOfNumber[0][1] - setOfNumber[0][0]);
    distanceY = std::abs(setOfNumber[1][1] - setOfNumber[1][0]);
}

void mandelbrotVisualizer::setScreenSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width, height;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = std::round((csbi.srWindow.Right - csbi.srWindow.Left + 1) * 0.48); // < 0.5
        height = std::round((csbi.srWindow.Bottom - csbi.srWindow.Top + 1) * 0.92) - 3;

        if(height % 2 == 1){ //height has to bo even
            height --;
        }

        long double baseWidthDistance = (std::abs(baseSetOfNumber[0][0] - baseSetOfNumber[0][1]));
        long double baseHeightDistance = (std::abs(baseSetOfNumber[1][0] - baseSetOfNumber[1][1]));

        int adjustedWidth = height *  (baseWidthDistance / baseHeightDistance); // Ratio between width and height of screen to match Mandelbrot ratio
        int adjustedHeight = width * (baseHeightDistance / baseWidthDistance); 

        if(adjustedHeight > height){
            screenSize[0] = adjustedWidth;
            screenSize[1] = height;
        }
        else{
            screenSize[0] = width;
            screenSize[1] = adjustedHeight;
        }
    }
}

void mandelbrotVisualizer::changeMaxIteration(){
    maxIterations = 1000 + countLeadingZeros(distanceX) * 500;
}

int mandelbrotVisualizer::countLeadingZeros(long double num) {
    int count = 0;
    long double numCopy = num;
    while (numCopy < 1){
        numCopy *= 10;
        count ++;
    }
    return count;
}




std::vector<long double> mandelbrotVisualizer::getDimensionSet(long double firstCoordinate, long double secondCoordinate,
                                                         long double screenProportion, long double distance){
    long double zoomAddition;
    if(zoomMultiplier >= 0){
        zoomAddition = (distance * (0.5L - (0.5L / zoomMultiplier)));
    }
    else{
        zoomAddition = distance * (zoomMultiplier / 4);
    }
    return {firstCoordinate + ((screenProportion - 0.5L) * distance) + zoomAddition,
            secondCoordinate + ((screenProportion - 0.5L) * distance) - zoomAddition};
}


void mandelbrotVisualizer::reset()
{
    currentSetOfNumber = baseSetOfNumber;
    middlePointCoordinate = baseMiddlePointCoordinate;
    firstIteration = true;
    allSetsOfNumbers.clear();  
}

void mandelbrotVisualizer::changeZoom(int zoom)
{
    zoomMultiplier = zoom;
    if(zoom == 0){
        zoomMultiplier = 1;
    }
    std::cout << "Le zoom " << zoom << " a été pris en compte" << std::endl;
}

void mandelbrotVisualizer::goBack(int iterationToGoBack)
{
    hasToGoBack = true;
    if(int(allSetsOfNumbers.size() - (1 + iterationToGoBack)) >= 0){
        setDistances(allSetsOfNumbers[allSetsOfNumbers.size() - (iterationToGoBack + 1)]);
        if(iterationToGoBack != 0){
            currentSetOfNumber = allSetsOfNumbers[allSetsOfNumbers.size() - 1];
            allSetsOfNumbers.pop_back();
        }
        
    }
    else{
        setDistances(baseSetOfNumber);
        currentSetOfNumber = baseSetOfNumber;
    }
}
