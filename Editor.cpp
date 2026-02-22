/*
 *  Editor.cpp
 *  Tadhg Murray(tmurra01)
 *  2/21/26
 *
 *  CS 15 Project 2 TypeWriter
 *
 *  This file contains an implementation of the Editor class.
 *
 */

#include "Editor.h"

#include <string>
#include <iostream>
#include <fstream>

/*
 * name:      Editor
 * purpose:   creates an Editor object, initalizes variables
 * arguments: std::string filename of file to edit text
 * returns:   nothing
 * effects:   assigns a value to cursorCol and cursorLine
 * other:     none
 */
Editor::Editor(std::string filename) {
    cursorCol = 0;
    cursorLine = 0;
    outfileName = filename;
    std::ifstream infile(filename);
    std::string line;
    //this makes it so that if file has something in it it gets fucked up i think, keep for now
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    if (lines.size() == 0) {
        lines.push_back("");
    }
    u.render(lines, cursorCol, cursorLine);
}

/*
 * name:      Editor
 * purpose:   creates an Editor object, initalizes variables
 * arguments: std::string filename of file to edit text, st::string logfile
 * to log user inputs into.
 * returns:   nothing
 * effects:   assigns a value to cursorCol and cursorLine
 * other:     none
 */
Editor::Editor(std::string filename, std::string logfile) {
    cursorCol = 0;
    cursorLine = 0;
    outfileName = filename;
    std::ifstream infile(filename);
    std::string line;   
    //this makes it so that if file has something in it it gets fucked up i think, keep for now
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    if (lines.size() == 0) {
        lines.push_back("");
    }
    u.startLogMode(logfile);
    u.render(lines, cursorCol, cursorLine);
}

/*
 * name:      run
 * purpose:   changes and displays the text editor based on user input
 * arguments: none
 * returns:   nothing
 * effects:   updates cursor position and maybe elements of lines, can also
 * increase/decrese lines size
 * other:     none
 */
void Editor::run() {
    int c = u.getChar();
    //command mode case
    if (c == 27) {
        c = u.getChar();
        if (c == 115) {
            saveFile();
        }
        else if (c == 120) {
            bool save = u.savePrompt();
            if (save) {
                saveFile();
            }
            u.close();
        }
    }
    else if (c == KEY_BACKSPACE) {
        if (cursorCol > 0) {
            cursorCol--;
            lines[cursorLine].erase(cursorCol, 1);
        }
        else {
            if (cursorLine != 0) {
                cursorLine--;
                cursorCol = lines[cursorLine].size();
                lines[cursorLine] += lines[cursorLine + 1];
                lines.erase(lines.begin() + (cursorLine + 1));
            }
        }
    }
    //good
    else if (c == KEY_LEFT) {
        if (cursorCol != 0) {
            cursorCol--;
        }
        else {
            if (cursorLine != 0) {
                cursorLine--;
                cursorCol = lines[cursorLine].size();
            }
        }
    }
    //good
    else if (c == KEY_RIGHT) {
        if (lines[cursorLine].size() > cursorCol) {
            cursorCol++;
        }
        else {
            if (lines.size() > cursorLine + 1) {
                cursorLine++;
                cursorCol = 0;
            }
        }
    }
    //good
    else if (c == KEY_DOWN) {
        if (lines[cursorLine].size() > (cursorCol + u.getTerminalWidth())) {
            cursorCol += u.getTerminalWidth();
        }
        else if (lines.size() > cursorLine + 1) {
            cursorLine++;
            if (lines[cursorLine].size() < cursorCol) {
                cursorCol = lines[cursorLine].size();
            }
        }
    }
    //good
    else if (c == KEY_UP) {
        if (cursorLine != 0) {
            if (cursorCol > u.getTerminalWidth()) {
                cursorCol -= u.getTerminalWidth();
            }
            else {
                cursorLine--;
                if (cursorCol > lines[cursorLine].size()) {
                    cursorCol = lines[cursorLine].size();
                }
            }
        }
    }
    else if (c == 10) {
        std::string s = lines[cursorLine].substr(cursorCol);
        lines[cursorLine].erase(cursorCol);
        cursorLine++;
        while (cursorLine >= lines.size()) {
            lines.push_back("");
        }
        lines.insert(lines.begin() + cursorLine, s);
        cursorCol = 0;
    }
    else {
        lines[cursorLine].insert(cursorCol, 1, c);
        cursorCol++;
    }
    u.render(lines, cursorCol, cursorLine);
    
}

void Editor::saveFile() {
    std::ofstream(outfileName, std::ios::trunc).close();
    std::ofstream outfile(outfileName);
    for (int i = 0; i < lines.size(); i++) {
        outfile << lines[i] << std::endl;
    }
    u.displaySaveMessage();
}


/*
 * name:      ~Editor
 * purpose:   destructor for the Editor class
 * returns:   nothing
 * effects:   none
 * other:     none
 */
Editor::~Editor() {

}