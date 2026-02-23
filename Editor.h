/*
 * Editor.h
 *
 * CS15 Proj 2: Typewriter
 * 

 */

#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "ActionStack.h"
#include "TextUI.h"

#include <vector>
#include <string>

class Editor {
    public:
        Editor(std::string filename);
        Editor(std::string filename, std::string logfile);
        ~Editor();
        void run();
    private:
        std::vector<std::string> lines;
        int cursorCol;
        int cursorLine;
        std::string outfileName;
        TextUI u;
        void saveFile();
        ActionStack undo;
        ActionStack redo;
};

#endif