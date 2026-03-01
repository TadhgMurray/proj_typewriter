/*
 * Editor.h
 *
 * CS15 Project 2: Typewriter
 * 
 * The Editor class is a class that creates an interactable text editor using a
 * vector of strings. The class handles user input so it can edit
 * this vector in multiple ways, insertion/deletion of printable characters,
 * the moving of the cursor using arrow keys and the other text-changing
 * operations, an undo operation, a redo operations, a save operation and a
 * close operation. These last four operations are part of the command mode
 * case. It holds the potential undos and redos in two ActionStack
 * instances. It can render the cursor position and the vector of string
 * into a text editor inteface using the TextUI class.
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
        //other input cases helper
        void inputCases(int c);
        //command mode helper
        bool commandMode();
        //save helper
        void saveFile();
        //undo/redo helper functions
        void processUndoRedo(ActionStack &source, ActionStack &dest, 
                            bool isRedo);
        void undoRedoLoop(ActionStack::Action a, bool redoCommand);
        void undoRedoInsertionDeletion(ActionStack::Action a);
        void undoRedoDeletionInsertion(ActionStack::Action a);
        //arrow helper functions
        void arrowCases(int c);
        void arrowLeft();
        void arrowRight();
        void arrowDown();
        void arrowUp();
        //backspace function
        void backspace();
        //character/newline insertion functions
        void newLine();
        void insertCharacter(int c);
        //bool flags for clearing redo/entered command mode
        bool commandModeDone;
        bool justUndid;
        //ActionStacks to hold potential undo and redo operations
        ActionStack undo;
        ActionStack redo;
};


#endif