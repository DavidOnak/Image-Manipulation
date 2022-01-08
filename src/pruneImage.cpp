#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string>

#include "twoDtree.h"
#include "imageUtil/PNG.h"
using namespace std;

// prototype for clean function
void clean(void);

int pruneImage() {
    cout << "This simple program can take an image and prune it based on input tolerance. To work YOU MUST" << endl;
    cout << "HAVE A DIRECTORY CALLED inputImages where this executable is located with the images you want to prune. " << endl;

    int prune = 0;
    while (prune == 0) {
        cout << "Enter name of PNG you want to prune with .png, avoid using images over 4MP as this program can run out of memory." << endl;

        /*get input name and check for error*/
        string name;
        imageUtil::PNG png, out;

        std::getline(std::cin, name);

        //cout << name << endl;
        while (!png.readFromFile("inputImages/" + name)) {
            std::getline(std::cin, name);
        }

        printf("Got png of size %d by %d , will now make the tree.\n", png.width(), png.height());
        twoDtree tree = twoDtree(png);

        // obtain tolerance to do prune
        int tol;
        cout << "Enter the tolerance for prune (normally around 50 to 5000), please be patient!" << endl;

        /*get input name and check for error*/
        while (!scanf("%i", &tol)) {
            cout << "Not a valid tolerance!\n" << endl;
        }

        tree.prune(tol);
        out = tree.render();
        out.writeToFile("outputImages/pruned-" + name);
        printf("pruned-%s as been added to outputImages.", name.c_str());

        cout << "Type 'p' to prune again otherwise press anything else to exit" << endl;
        char input;
        clean();
        scanf("%c", &input);
        clean();
        system("CLS");

        // if 'p' is not entered then kick out of loop
        if (input != 'p') {
            prune++;
        }
    }
    return 0;
}

// cleans buffer of extra characters
void clean(void)
{
    char dummy;

    do
    {
        /* read each characters seprate */
        scanf("%c", &dummy);
    } while (dummy != '\n');  /* if the user enters this function will stop */
}