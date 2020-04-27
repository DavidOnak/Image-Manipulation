#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <stdbool.h>

#include "ImageEffects.h"
#include "imageUtil/hslaPNG.h"
using namespace std;

void clean(void);

int ImageManipulation() {
    cout << "This simple program can take an image and apply a simple modification to it. To work YOU MUST" << endl;
	cout << "HAVE A DIRECTORY CALLED inputImages where this executable is located with the images you want to edit. " << endl;

	int modify = 0;
	while(modify == 0){
        cout << "Enter name of PNG you want to manipulate with .png extension." << endl;

        /*get input name and check for error*/
        string name;
        imageUtil::hslaPNG png, out;

        std::getline(std::cin, name);

        //cout << name << endl;
        while (!png.readFromFile("inputImages/"+name)) {
            std::getline(std::cin, name);
        }

        char command;
        int width = png.width()-1;
        int height = png.height()-1;
        string newName;
        cout << "Enter 'g' to gray scale, 's' to add spotlight, 'u' to ubcify, or 'w' to add a water mark." << endl;

        bool valid = false;

        /*get input type and check for error*/
        while (!valid){
            scanf("%c", &command);
            clean();

            switch(command) {
                case 'g': {
                    out = grayscale(png);
                    newName = "grayScaled";
                    valid = true;
                    break;
                }
                case 's': {
                    printf("Enter an x coordinate from 0 to %d\n", width);
                    int x;
                    scanf("%i", &x);
                    while (x < 0 || x > width) {
                        cout << "Not a valid entry!" << endl;
                        scanf("%i", &x);
                    }
                    clean();
                    printf("Enter an y coordinate from 0 to %d\n", height);
                    int y;
                    scanf("%i", &y);
                    while (y < 0 || y > height) {
                        cout << "Not a valid entry!" << endl;
                        scanf("%i", &y);
                    }
                    clean();
                    out = createSpotlight(png, x, y);
                    newName = "spotlight";
                    valid = true;
                    break;
                }
                case 'u': {
                    out = ubcify(png);
                    newName = "ubcify";
                    valid = true;
                    break;
                }
                case 'w': {
                    cout << "Enter png image to watermark onto selected image. (with .png extension)" << endl;
                    clean();
                    string waterM;
                    imageUtil::hslaPNG second;
                    std::getline(std::cin, waterM);

                    //cout << name << endl;
                    while (!second.readFromFile("inputImages/"+waterM)) {
                        std::getline(std::cin, waterM);
                    }
                    out = watermark(png, second);
                    newName = "watermark";
                    valid = true;
                    break;
                }
                default: {
                    cout << "Not a valid entry!" << endl;
                }
            }
        }

        out.writeToFile("outputImages/"+newName+"-"+name);
        printf("%s-%s as been added to outputImages.\n", newName.c_str(), name.c_str());

        cout << "Type '1' to edit again otherwise press anything else to exit." << endl;
        char input;
        scanf("%c", &input);
        clean();
        system ("CLS");

        // if 'p' is not entered then kick out of loop
        if (input != '1') {
            modify++;
        }
	}
	return 0;
}

// cleans buffer of extra characters
void clean (void)
{
    char dummy;

    while(dummy != '\n') {  /* if the user enters this function will stop */
        scanf("%c",&dummy);
    }
}

