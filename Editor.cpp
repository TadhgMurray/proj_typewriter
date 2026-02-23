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
    if (c == -1) {
        u.render(lines, cursorCol, cursorLine);
        u.close();
        exit(0);
    }
    //command mode case
    if (c == 27) {
        c = u.getChar();
        //save command
        if (c == 115) {
            saveFile();
        }
        //exit command
        else if (c == 120) {
            bool save = u.savePrompt();
            if (save) {
                saveFile();
            }
            u.close();
            exit(0);
        }
        //undo command
        else if (c == 117) {
            if (undo.isEmpty()) {
                return;
            }
            bool firstDeleted = undo.top().deleted;
            int targetLine = undo.top().line;
            while (not undo.isEmpty() and undo.top().deleted == firstDeleted
                    and undo.top().line == targetLine) {
                ActionStack::Action a = undo.top();
                undo.pop();
                redo.push(a);
                if (!a.deleted) {  // undo insert
                    if (a.character == '\n') {
                        lines[a.line] += lines[a.line + 1];
                    }
                    else {
                        lines[a.line].erase(a.column, 1);
                    }
                } 
                else {  // undo delete
                    if (a.character == '\n') {
                        std::string tail =
                            lines[a.line].substr(a.column);
                        lines[a.line].erase(a.column);
                        lines.insert(lines.begin() + a.line + 1, tail);
                    }
                    else {
                        lines[a.line].insert(a.column, 1, a.character);
                    }
                }
                cursorLine = a.line;
                cursorCol = a.column;
            }
        }
        //redo command
        else if (c == 114) {
            if (redo.isEmpty()) {
                return;
            }
            bool firstDeleted = redo.top().deleted;
            while (not redo.isEmpty() and redo.top().deleted == firstDeleted) {
                ActionStack::Action a = redo.top();
                redo.pop();
                undo.push(a);
                if (!a.deleted) {
                    //original action was insert, redo = insert
                    lines[a.line].insert(a.column, 1, a.character);
                }
                else {
                    // original action was delete, redo = delete
                    if (a.character == '\n') {
                        // merge lines again
                        lines[a.line] += lines[a.line + 1];
                        lines.erase(lines.begin() + a.line + 1);
                    }
                    else {
                        lines[a.line].erase(a.column, 1);
                    }
                }
                cursorLine = a.line;
                cursorCol = a.column;
            }
        }
    }
    //for some reason when not used in log mode it doesn't update the visual
    //cursor position right, but the functionality still works. To make it
    //update i had to do these two other ors. I have no clue why. I think
    //it's because when not in log mode it used the insertion else at the end
    //which retained the functionality but not the visuals?
    else if (c == KEY_BACKSPACE or c == 127 or c == 8) {
        if (cursorCol > 0) {
            cursorCol--;
            undo.push(lines[cursorLine].at(cursorCol), true, cursorLine, 
                cursorCol);
            lines[cursorLine].erase(cursorCol, 1);
        }
        else if (cursorLine != 0){
            int previousLine = cursorLine - 1;
            int previousLength = lines[previousLine].size();
            //save newline deletion as '\n'
            undo.push('\n', true, previousLine, previousLength);
            lines[previousLine] += lines[cursorLine];
            lines.erase(lines.begin() + cursorLine);
            cursorLine = previousLine;
            cursorCol = previousLength;
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
        int width = u.getTerminalWidth();
        int lineLength = lines[cursorLine].size();
        int offsetInWrap = cursorCol % width;
        int currentWrap = cursorCol / width;
        int nextWrapStart = (currentWrap + 1) * width;
        //another wrap exists in this line
        if (nextWrapStart < lineLength) {
            cursorCol = nextWrapStart + offsetInWrap;
            if (cursorCol > lineLength) {
                cursorCol = lineLength;
            }
        }
        //move to next actual line
        else if (cursorLine + 1 < lines.size()) {
            cursorLine++;
            if (cursorCol > lines[cursorLine].size()) {
                cursorCol = lines[cursorLine].size();
            }
        }
    }
    //good
    else if (c == KEY_UP) {
        if (cursorCol >= u.getTerminalWidth()) {
            cursorCol -= u.getTerminalWidth();
        }
        else if (cursorLine != 0){
                cursorLine--;
                if (cursorCol > lines[cursorLine].size()) {
                    cursorCol = lines[cursorLine].size();
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
        undo.push(c, false, cursorLine, cursorCol);
        lines[cursorLine].insert(cursorCol, 1, c);
        cursorCol++;
    }
    if (c != 117 and c != 114) {
        redo.clear();
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