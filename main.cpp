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


int main(int argc, char *argv[]) {
    if (argc == 3) {
        Editor e(argv[1], argv[2]);
        while (true) {
            e.run();
        }
    }
    else if(argc == 2){
        Editor e(argv[1]);
        while (true) {
            e.run();
        }
    }
    else {
        std::cerr << "Usage: ./typewriter textFile [logFile]";
        exit(1);
    }
}