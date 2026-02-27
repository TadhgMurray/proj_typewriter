/*
 *  Main.cpp
 *  Tadhg Murray (tmurra01)
 *  2/23/26
 *
 *  CS 15 Project 2 typewriter
 *
 *  This file is the main file for the program. It reads in the filenames
 *  from the console and creates an Editor object to edit those files.
 *
 */

#include "Editor.h"
#include "TextUI.h"

#include <iostream>

/*
 * name:      main
 * purpose:   Runs the text editor, inputting the filenames, exits if
 * too many arguments provided
 * arguments: argc that stores the number of command line arguments
 * char *argv[] that stores those command line arguments
 * returns:   returnts int 0 once program is finished running
 * effects:   none
 * other:     none
 */
int main(int argc, char *argv[]) {
    if (argc == 3) {
        Editor e(argv[1], argv[2]);
        e.run();
    }
    else if(argc == 2){
        Editor e(argv[1]);
        e.run();
    }
    else {
        std::cerr << "Usage: ./typewriter textFile [logFile]";
        exit(1);
    }
    return 0;
}