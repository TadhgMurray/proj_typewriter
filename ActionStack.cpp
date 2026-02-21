/*
 *  ActionStack.cpp
 *  Tadhg Murray(tmurra01)
 *  2/20/26
 *
 *  CS 15 Project 2 TypeWriter
 *
 *  This file contains an implementation of the ActionStack class.
 *
 */

#include "ActionStack.h"

#include <stdexcept>

/*
 * name:      ActionStack
 * purpose:   creates an ActionStack object
 * arguments: none
 * returns:   nothing
 * effects:   none
 * other:     none
 */
ActionStack::ActionStack() {
}

/*
 * name:      size const
 * purpose:   returns the size of the stack
 * arguments: none
 * returns:   returns int size of the stack
 * effects:   none
 * other:     none
 */
int ActionStack::size() const {
    return stack.size();
}

/*
 * name:      isEmpty const
 * purpose:   returns if the stack is empty or not
 * arguments: none
 * returns:   returns bool true/false depending on if stack is empty or not
 * effects:   none
 * other:     none
 */
bool ActionStack::isEmpty() const {
    if (stack.size() == 0) {
        return true;
    }
    return false;
}

/*
 * name:      top const
 * purpose:   returns the top Action on the stack
 * arguments: none
 * returns:   returns the top Action on the stack
 * effects:   none
 * other:     if stack is empty returns nothing
 */
ActionStack::Action ActionStack::top() const {
    if (isEmpty()) {
        throw std::runtime_error("empty_stack");
    }
    return stack.back();
}

/*
 * name:      pop
 * purpose:   removes the top element of the stack
 * arguments: none
 * returns:   nothing
 * effects:   decreases size of stack by 1
 * other:     if stack is empty does nothing
 */
void ActionStack::pop() {
    if (isEmpty()) {
        throw std::runtime_error("empty_stack");
    }
    stack.pop_back();
}

/*
 * name:      push
 * purpose:   adds an Action element to the top of the stack
 * arguments: Action elem to add to stack
 * returns:   nothing
 * effects:   increases size of stack by 1
 * other:     none
 */
void ActionStack::push(Action elem) {
    stack.push_back(elem);
}

/*
 * name:      push
 * purpose:   adds an Action element to the top of the stack
 * arguments: Char c, bool was_delete, std::size_t line, and std::size_t column
 * to create an Action and then put it onto the stack
 * returns:   nothing
 * effects:   increases size of stack by 1
 * other:     none
 */
void ActionStack::push(char c, bool was_delete, size_t line, size_t column) {
    struct Action elem;
    elem.character = c;
    elem.deleted = was_delete;
    elem.column = column;
    elem.line = line;
    stack.push(elem);
}

/*
 * name:      clear
 * purpose:   clears the entire stack
 * arguments: none
 * returns:   nothing
 * effects:   change size of stack to 0
 * other:     none
 */
void ActionStack::clear() {
    while (not isEmpty()) {
        pop();
    }
}

/*
 * name:      ~ActionStack
 * purpose:   destructor for the ActionStack class
 * arguments: none
 * returns:   nothing
 * effects:   none
 * other:     none
 */
ActionStack::~ActionStack() {
}
