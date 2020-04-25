#include <iostream>
#include <cstdlib>

#include "twoDtree.h"
#include "imageUtil/PNG.h"

int pruneImage() {
	cout << "This simple program can take an image and prune it based on input tolerance. \n"
		+ "To work YOU MUST HAVE A DIRECTORY CALLED inputImages where this executable is located\n"
		+ "with the images you want to prune. Please type in the name of the PNG you want to use (with file extention)."<< endl;

    /*get input name and check for error*/
	string name;
    scanf("%s", &name);
    while (!std::ifstream infile(name))
    {
        cout << " Image not found!\n" << endl;
		scanf("%s", &name);
    }
	
	imageUtil::PNG png, result;
	png.readFromFile(name);
	twoDtree tree = twoDtree(png);
	
	// obtain tolerance to do prune
	int tol;
	cout << "Enter the tolerance for prune (normally around ****). \n" << endl;

    /*get input name and check for error*/
    while (!scanf("%i", &tol);)
    {
        cout << "Not a valid tolerance!\n" << endl;
    }
	
	result = tree.prune(tol);
	result.writeToFile("pruned-"+name);
}