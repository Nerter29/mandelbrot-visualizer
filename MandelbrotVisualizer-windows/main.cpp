#include "mandelbrotVisualizer.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main(){
    // enable color for windows 10
    DWORD originalMode;
    HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(stdoutHandle, &originalMode);
    SetConsoleMode(stdoutHandle, originalMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);



    
    mandelbrotVisualizer mandelbrotObject;

    //mandelbrotObject.setColorTables();
    mandelbrotObject.reset();
    mandelbrotObject.createMandelBrotSet();

    mandelbrotObject.reset();
    mandelbrotObject.createMandelBrotSet();

    while (true){
        std::string plusOrMinus = " ";
        if(mandelbrotObject.currentComplex[1] >= 0){
            plusOrMinus = " + ";
        }
        std::cout << "Nombre complexe au centre de l'ecran : " << mandelbrotObject.currentComplex[0] << plusOrMinus 
                  << mandelbrotObject.currentComplex[1] << "i"<< " | Nombres d'iterations par points : "
                  << mandelbrotObject.maxIterations <<std::endl; 

        

        std::cout << "\nTapez 'help' pour voir les commandes disponibles";

        while (!mandelbrotObject.hasToCreateMandelBrot){
            std::cout << "\nEntrez les coordonnees de l'ecran ou vous souhaitez zommer, sous cette forme : XXYY ";

            mandelbrotObject.hasToCreateMandelBrot = true;
            std::string input;
            std::getline(std::cin, input);

            int xMax = mandelbrotObject.screenSize[0];
            int yMax =mandelbrotObject.screenSize[1];
            if(mandelbrotObject.onPercentageMode){
                xMax = 99 + 1;
                yMax = 99 + 1;
            }
            
            if (input.length() == 4 &&
                std::all_of(input.begin(), input.end(), ::isdigit) &&
                std::stof(input.substr(0, 2)) < xMax &&
                std::stof(input.substr(2, 2)) < yMax) { 

                mandelbrotObject.middlePointCoordinate = {std::stof(input.substr(0, 2)), std::stof(input.substr(2, 2))};
            }
            else if(input == "help"){//help
                std::string percentageOrCoordinateHelp = "\n- 'percent' : met en mode pourcentage : les coordonees a rentrer seront des pourcentages de l'ecran";
                if(mandelbrotObject.onPercentageMode){
                    percentageOrCoordinateHelp = "\n- 'coord' : met en mode coordonnees : les coordonees a rentrer seront associees a chaque pixel de l'ecran";
                }
                std::cout << percentageOrCoordinateHelp
                          << "\n- entrez 'z', suivi d'un entier relatif pour regler le zoom, par exemple : 'z5'"
                          << "\n- 'reset' : reinitilise l'ensemble"
                          << "\n- 'back' : fait revenir en arriere\n";

                mandelbrotObject.hasToCreateMandelBrot = false;
            } 


            
            else if(input == "percent" && !mandelbrotObject.onPercentageMode){ //percent
                mandelbrotObject.onPercentageMode = true;
                mandelbrotObject.goBack(0);

            }
            else if(input == "coord"&& mandelbrotObject.onPercentageMode){ //coord
                mandelbrotObject.onPercentageMode = false;
                mandelbrotObject.goBack(0);

            }

            else if(input.size() > 1 && input[0] == 'z' && std::all_of(input.begin() + 1, input.end(), ::isdigit)){ //zoom positif
                mandelbrotObject.changeZoom(std::stoi(input.substr(1, input.size() - 1)));
                mandelbrotObject.hasToCreateMandelBrot = false;
            }
            else if(input.size() > 1 && input[0] == 'z' && input[1] == '-' &&
                    std::all_of(input.begin() + 2, input.end(), ::isdigit)){ //zoom négatif
                mandelbrotObject.changeZoom(-std::stoi(input.substr(2, input.size() - 1)));
                mandelbrotObject.hasToCreateMandelBrot = false;
            }

            else if(input == "reset"){ //reset
                mandelbrotObject.reset();
            }

            else if(input == "back"){ //back
                mandelbrotObject.goBack(1);
            }

            else if(input == "debug"){ //back
                std::cout << mandelbrotObject.distanceX << " " << mandelbrotObject.maxIterations;
                mandelbrotObject.hasToCreateMandelBrot = false;

            }
            else{ // inconnu
                mandelbrotObject.hasToCreateMandelBrot = false;

                std::cout << "Cette commande n'existe pas :/";
            }
        }

        mandelbrotObject.hasToCreateMandelBrot = false;
        mandelbrotObject.createMandelBrotSet();

    }

    return 0;
}
