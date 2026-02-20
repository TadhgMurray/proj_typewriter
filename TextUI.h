/* 
 * TextUI.h
 *
 * CS15 Proj 2: Typewriter
 * 
 * Interface for the TextUI class, which creates the 
 * text-based user interface for typewriter.
 *
 * Author: Milod Kazerounian (Sept 2025)
 */

#ifndef __TEXTUI_H__
#define __TEXTUI_H__

#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>

class TextUI {
public:
    // Constructor and destructor.
    TextUI();
    ~TextUI();


    /* Member function for putting UI in log mode, so that program state is
     * continuously dumped to the given logfile. Call this function when
     * the user provides an extra argument on the command line naming the
     * log file 
     */
    void startLogMode(std::string logfile);

    /* The `render` function updates the interface with the given arguments.
     * `lines` is a vector of the lines in the document.
     * `cursorCol` is the column that the cursor is located 
     * (like its `x` coordinate)
     * `cursorLine` is the line index within `lines` that the cursor is located.
     * (like its `y` coordinate)
     */
    void render(const std::vector<std::string> &lines, const int &cursorCol, const int &cursorLine);

    /* Listens for a character press from the user, and returns the character
     * (usually expressed as its ASCII number) that was pressed.
     */
    int getChar();

    // Ends the user interface.
    void close();

    /* Getter functions for determining the width and height of the user's
     * terminal.
     */
    int getTerminalWidth();
    int getTerminalHeight();

    // Function for displaying a message "Saved!" to the user.
    void displaySaveMessage();

    /* Function for asking user if they wish to save.
     * returns true if yes, false if no.
     */
    bool savePrompt();

private:
    /* NOTE: The private members below concern the innerworkings of the class.
     * Clients of the class can happily ignore these details.
     */
    
    // width and height of terminal
    int termW, termH;


    // struct to capture index info about a line
    struct LineIDX {
        // line's number within lines vector
        size_t lineno;
        // if the line wraps, wrapno captures the which "wrap" number we're on
        size_t wrapno;
    };
        
    // position of the last vertical offset we printed from
    LineIDX lastOffsetIDX;
    
    // last position of the cursor
    LineIDX lastCursorIDX;
    
    // position of last offset of *bottom* of screen
    LineIDX lastBottomIDX;

    /* Keeps track of whether cursor was at the top or bottom the *last* time
     * that `render` was called. Useful for implementing scrolling. 
     */
    bool prevAtTop, prevAtBottom;

    // comparison functions for lines
    bool lineLT(const LineIDX &l1, const LineIDX &l2);
    bool lineGT(const LineIDX &l1, const LineIDX &l2);

    /* returns incremented copy of l
     * by incrementing either l's lineno (when at end of line), 
     * or wrapno (when at end of wrap) 
     */
    LineIDX incrementLineIDX(const LineIDX &l, const std::vector<std::string> &lines);

    // keep track of when we're in log mode
    bool log_mode;

    // output stream for log mode
    std::ofstream *dumpstream;

    // maintain the last dump that was made to avoid redundant states
    std::string lastDump;

    // when in log mode, used to dump the current program state
    void dumpState(const std::vector<std::string> &lines,
                   const int &cursorX, const int &cursorY);
};


#endif
