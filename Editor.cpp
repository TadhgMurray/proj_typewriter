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
    commandModeDone = false;
    justUndid = true;
    std::ifstream infile(filename);
    std::string line;
    //writes file contents into editor
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    //if nothing was written into editor adds an empty line
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
    commandModeDone = false;
    justUndid = true;
    std::ifstream infile(filename);
    std::string line;   
    //writes file contents into editor
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    //if nothing was written into editor adds an empty line
    if (lines.size() == 0) {
        lines.push_back("");
    }
    u.startLogMode(logfile);
    u.render(lines, cursorCol, cursorLine);
}

//read this please, undo/redo neeeddddddd to be modular ffs please(AAAAHHHHHHH)
/*
 * name:      run
 * purpose:   changes and displays the text editor based on user input
 * arguments: none
 * returns:   nothing
 * effects:   updates cursor position and maybe elements of lines, can also
 * increase/decrease lines size
 * other:     none
 */
void Editor::run() {
    while (true) {
        commandModeDone = false;
        justUndid = true;
        int c = u.getChar();
        if (c == EOF or  c == 4 or c == -1) {
            u.render(lines, cursorCol, cursorLine);
            u.close();
            return;
        }
        else if (c == 27) {
            if (commandMode()) {
                u.render(lines, cursorCol, cursorLine);
                u.close();
                return;
            }
        }
        else {
            inputCases(c);
        }
        u.render(lines, cursorCol, cursorLine);
    }
}

/*
 * name:      inputCases
 * purpose:   changes text based on user input of printable characters/actions
 * arguments: int c to insert into/do to text
 * returns:   nothing
 * effects:   updates cursor position.elements of lines, can
 * increase/decrease lines size
 * other:     none
 */
void Editor::inputCases(int c) {
    //backspace, have to do other two for non log-mode(ASCII different when
    //not log mode)
    if (c == KEY_BACKSPACE or c == 127 or c == 8) {
        backspace();
    }
    //deals with arrows
    arrowCases(c);
    //new line
    if (c == 10) {
        newLine();
    }
    //inserts character if printable and not used for command mode
    else if (c >= 32 and c <= 126 and not commandModeDone) {
        insertCharacter(c);
    }
    //clears redo if last input changes text
    if (not justUndid) {
        redo.clear();
    }
}

/*
 * name:      commandMode
 * purpose:   allows user to input commands to save, close, undo, or redo
 * arguments: none
 * returns:   bool true/false depending on if user did close command or not
 * effects:   none
 * other:     none
 */
bool Editor::commandMode() {
    commandModeDone = true;
    int c = u.getChar();
    //save command
    if (c == 115) {
        saveFile();
    }
    //exit command
    else if (c == 120) {
        //asks to save
        if (u.savePrompt()) {
            saveFile();
        }
        //retunrs true becasue user wants to close editor
        return true;
    }
    //undo command
    else if (c == 117) {
        undoCommand();
    }
    //redo command
    else if (c == 114) {
        redoCommand();
    }
    return false;
}

/*
 * name:      saveFile
 * purpose:   saves the text editor file
 * arguments: none
 * returns:   nothing
 * effects:   none
 * other:     none
 */
void Editor::saveFile() {
    //writes current line contents into the file
    std::ofstream(outfileName, std::ios::trunc).close();
    std::ofstream outfile(outfileName);
    for (int i = 0; i < lines.size(); i++) {
        outfile << lines[i] << std::endl;
    }
    u.displaySaveMessage();
}

/*
 * name:      undoCommand
 * purpose:   undoes previous user actions
 * arguments: none
 * returns:   nothing
 * effects:   changes cursorLine, cursorCol, and the lines' contents/size, adds
 * to redo stack
 * other:     none
 */
void Editor::undoCommand() {
    if (undo.isEmpty()) {
        return;
    }
    bool firstDeleted = undo.top().deleted;
    int targetLine = undo.top().line;
    bool notJustNewLine = false;
    //checks if deletion flag/lines number changes to stop the undoing
    while (not undo.isEmpty() and undo.top().deleted == firstDeleted
            and undo.top().line == targetLine) {
        ActionStack::Action a = undo.top();
        //stops at newline boundary before inserting/deleting newline if
        //another character was entered before
        if (a.character == '\n' and notJustNewLine) {
            return;
        }
        undo.pop();
        redo.push(a);
        //false because its an undo
        undoRedoLoop(a, false);
        //stops at newline boundary
        if (a.character == '\n') {
            return;
        }
        notJustNewLine = true;
    }
}

/*
 * name:      redoCommand
 * purpose:   redoes previous undo
 * arguments: none
 * returns:   nothing
 * effects:   changes cursorLine, cursorCol, and the lines' contents/size, adds
 * to redo stack
 * other:     none
 */
void Editor::redoCommand() {
    if (redo.isEmpty()) {
        return;
    }
    bool firstDeleted = redo.top().deleted;
    int targetLine = redo.top().line;
    //checks if deletion flag/line number changes to stop the redoing
    while (not redo.isEmpty() and redo.top().deleted == firstDeleted and 
        redo.top().line == targetLine) {
        ActionStack::Action a = redo.top();
        redo.pop();
        undo.push(a);
        //true because its a redo
        undoRedoLoop(a, true);
        //stops at newline boundary
        if (a.character == '\n') {
            return;
        }
    }
}

/*
 * name:      undoLoop
 * purpose:   loops through the undo stack, undoing actions
 * arguments: none
 * returns:   nothing
 * effects:   changes cursorLine, cursorCol, and the lines' contents/size, adds
 * to redo stack
 * other:     none
 */
void Editor::undoRedoLoop(ActionStack::Action a, bool redoCommand) {
    //undo an original insertion, or redo an original deletion
    if ((not a.deleted and not redoCommand) or (a.deleted and redoCommand)) {
        undoRedoInsertionDeletion(a);
    } 
    //undo an original deletion, or redo an original insertion
    else {
        undoRedoDeletionInsertion(a);
    }
}

/*
 * name:      undoRedoInsertionDeletion
 * purpose:   erases previous text change
 * arguments: ActionStack::Action a to remove from text
 * returns:   nothing
 * effects:   moves cursorLine/cursorCol, reduces lines' contents/size
 * other:     none
 */
void Editor::undoRedoInsertionDeletion(ActionStack::Action a) {
    //if character is a newline copies that new line back to previous line
    //then deletes that new line and resets line and col
    if (a.character == '\n') {
        if (a.line + 1 < lines.size()) {
            lines[a.line] += lines[a.line + 1];
            lines.erase(lines.begin() + a.line + 1);
            cursorLine = a.line;
            cursorCol = a.column;
        }
    }
    //otherwise deletes character, reseting line and col
    else {
        lines[a.line].erase(a.column, 1);
        cursorLine = a.line;
        cursorCol = a.column;
    }
}

/*
 * name:      undoRedoDeletionInsertion
 * purpose:   inserts previous text change
 * arguments: ActionStack::Action a to add to text
 * returns:   nothing
 * effects:   moves cursorLine/cursorCol, increases lines' contents/size
 * other:     none
 */
void Editor::undoRedoDeletionInsertion(ActionStack::Action a) {
    //if character is a newline adds a new line to lines and takes rest of
    //lines contents after the newline to the new line
    if (a.character == '\n') {
        std::string tail = lines[a.line].substr(a.column);
        lines[a.line].erase(a.column);
        lines.insert(lines.begin() + a.line + 1, tail);
        cursorLine = a.line + 1;
        cursorCol = 0;
    }
    //otherwise inserts character, updates line/column
    else {
        lines[a.line].insert(a.column, 1, a.character);
        cursorLine = a.line;
        cursorCol = a.column + 1;
    }
}

/*
 * name:      backspace
 * purpose:   updates the text by backspacing
 * arguments: none
 * returns:   nothing
 * effects:   updates cursor position and elements of lines, can also
 * decrease lines size, pushes onto undo stack
 * other:     none
 */
void Editor::backspace() {
    //if not at start of line, erase and hold in undo
    if (cursorCol > 0) {
        cursorCol--;
        undo.push(lines[cursorLine].at(cursorCol), true, cursorLine, 
                cursorCol); 
        lines[cursorLine].erase(cursorCol, 1);
    }
    //if at start of line and not the first line, switch lines, add
    //line to previous line, and then delete line
    else if (cursorLine != 0) {
        int previousLine = cursorLine - 1;
        int previousLength = lines[previousLine].size();
        //save newline deletion as '\n'
        undo.push('\n', true, previousLine, previousLength);
        lines[previousLine] += lines[cursorLine];
        lines.erase(lines.begin() + cursorLine);
        cursorLine = previousLine;
        cursorCol = previousLength;
    }
    justUndid = false;
}

/*
 * name:      arrowCases
 * purpose:   checks if the user wants to update cursor position using arrow
 * keys
 * arguments: int c ASCII index the user pressed
 * returns:   nothing
 * effects:   updates cursor position
 * other:     none
 */
void Editor::arrowCases(int c) {
    //arrow left
    if (c == KEY_LEFT) {
        arrowLeft();
    }
    //arrow right
    else if (c == KEY_RIGHT) {
        arrowRight();
    }
    //arrow down
    else if (c == KEY_DOWN) {
        arrowDown();
    }
    //arrow up
    else if (c == KEY_UP) {
        arrowUp();
    }
}

/*
 * name:      arrowUp
 * purpose:   moves user's cursor up
 * arguments: none
 * returns:   nothing
 * effects:   updates cursor position, decrement one line(or wraps)
 * other:     none
 */
void Editor::arrowUp() {
    //line wraps, so moves column up one line
    if (cursorCol >= u.getTerminalWidth()) {
        cursorCol -= u.getTerminalWidth();
    }
    //decrements cursorLine as long as not at top
    else if (cursorLine != 0) {
        cursorLine--;
        //if previous visual line was a wrap, sets to correct wrapped column
        int lastRows =  lines[cursorLine].size()/u.getTerminalWidth();
        //offset from terminal width - column of current line 
        int offset = u.getTerminalWidth() - cursorCol;
        //sets to final column of the terminal width of final wrap
        cursorCol = u.getTerminalWidth() * (lastRows + 1);
        //minuses the offset from the line it was on previously
        cursorCol -= offset;
        //if line has less characters than cursorCol size, moves cursorCol to
        //end
        if (cursorCol > lines[cursorLine].size()) {
            cursorCol = lines[cursorLine].size();
        }
    }
}

/*
 * name:      arrowDown
 * purpose:   moves user's cursor down
 * arguments: none
 * returns:   nothing
 * effects:   updates cursor position, increment one line(or wraps)
 * other:     none
 */
void Editor::arrowDown() {
    int width = u.getTerminalWidth();
    int lineLength = lines[cursorLine].size();
    int offsetInWrap = cursorCol % width;
    int currentWrap = cursorCol / width;
    int nextWrapStart = (currentWrap + 1) * width;
    //another wrap exists in this line
    if (nextWrapStart < lineLength) {
        cursorCol += width;
        //if this next wrap is not the full visual line, sets column to end
        //of visual line
        if (cursorCol > lineLength) {
            cursorCol = lineLength;
        }
    }
    //move to next actual line
    else if (cursorLine + 1 < lines.size()) {
        //goes to next line, sets it to previous visual column
        cursorLine++;
        //if previous wrapped (like its 87, 7 greater than width), sets to 7
        cursorCol = offsetInWrap;
        //if this column is greater than column = line size
        if (cursorCol > lines[cursorLine].size()) {
            cursorCol = lines[cursorLine].size();
        }
    }
}

/*
 * name:      arrowLeft
 * purpose:   moves user's cursor to the left
 * arguments: none
 * returns:   nothing
 * effects:   updates cursor position, decrement column by one(or line)
 * other:     none
 */
void Editor::arrowLeft() {
    if (cursorCol != 0) {
        cursorCol--;
    }
    //if at start of line moves to end of previous line
    else {
        if (cursorLine != 0) {
            cursorLine--;
            cursorCol = lines[cursorLine].size();
        }
    }
}

/*
 * name:      arrowLeft
 * purpose:   moves user's cursor to the right
 * arguments: none
 * returns:   nothing
 * effects:   updates cursor position, increment column by one(or line)
 * other:     none
 */
void Editor::arrowRight() {
    if (lines[cursorLine].size() > cursorCol) {
        cursorCol++;
    }
    //if at end of line moves to start of previous line
    else {
        if (lines.size() > cursorLine + 1) {
            cursorLine++;
            cursorCol = 0;
        }
    }
}

/*
 * name:      newLine
 * purpose:   adds a new line 
 * arguments: none
 * returns:   nothing
 * effects:   updates cursor position, increment line by one, column = 0
 * other:     none
 */
void Editor::newLine() {
    std::string s = lines[cursorLine].substr(cursorCol);
    undo.push('\n', false, cursorLine, cursorCol);
    lines[cursorLine].erase(cursorCol);
    cursorLine++;
    while (cursorLine > lines.size()) {
        lines.push_back("");
    }
    lines.insert(lines.begin() + cursorLine, s);
    cursorCol = 0;
    justUndid = false;
}

/*
 * name:      insertCharacter
 * purpose:   inserts character into the text
 * arguments: int c to insert into text
 * returns:   nothing
 * effects:   updates cursor position, increments column by 1
 * other:     none
 */
void Editor::insertCharacter(int c) {
    undo.push(c, false, cursorLine, cursorCol);
    lines[cursorLine].insert(cursorCol, 1, c);
    cursorCol++;
    justUndid = false;
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