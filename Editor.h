/*
 * Editor.h
 *
 * CS15 Projext 2: Typewriter
 * 
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
        void inputCases(int c);
        bool commandMode();
        void saveFile();
        void undoCommand();
        void undoLoop();
        void undoInsertion(ActionStack::Action a);
        void undoDelete(ActionStack::Action a);
        void redoCommand();
        bool redoOriginalInsertion(ActionStack::Action a);
        bool redoOriginalDeletion(ActionStack::Action a);
        void arrowCases(int c);
        void arrowLeft();
        void arrowRight();
        void arrowDown();
        void arrowUp();
        void backspace();
        void newLine();
        void insertCharacter(int c);
        bool commandModeDone;
        bool justUndid;
        ActionStack undo;
        ActionStack redo;
};


#endif