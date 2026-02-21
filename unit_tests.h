/*
 *  unit_tests.h
 *  Tadhg Murray(tmurra01)
 *  2/20/26
 *
 *  CS 15 Project 2 TypeWriter
 *
 *  Uses Matt Russell's unit_test framework to test the ActionStack class.
 *
 */

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

#include "ActionStack.h"

//tests ActionStack isEmpty() on an empty ActionStack
void asIsEmptyTest() {
    ActionStack as;
    assert(as.isEmpty() == true);
}

//tests ActionStack isEmpty() on a one element ActionStack
void asIsEmpty1ElementTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    as.push(a);
    assert(as.isEmpty() == false);
    
}

//tests ActionStack isEmpty() on a 100 element ActionStack
void asIsEmpty100ElementTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 100; i++) {
        as.push(a);
    }
    assert(as.isEmpty() == false);
}

//tests ActionStack size() on an empty ActionStack
void asEmptySizeTest() {
    ActionStack as;
    assert(as.size() == 0);
}

//tests ActionStack size() on a one element ActionStack
void asSize1Test() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    as.push(a);
    assert(as.size() == 1);
}

//tests ActionStack isEmpty() on a 100 element ActionStack
void asSize100Test() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 100; i++) {
        as.push(a);
    }
    assert(as.isEmpty() == false);
}

//tests ActionStack top() on an empty ActionStack
void asTopEmptyTest() {
    ActionStack as;
    bool runtimeErrorThrown = false;
    std::string errorMessage = "";
    try {
        as.top();
    } 
    catch (const std::runtime_error &e) {
        runtimeErrorThrown = true;
        errorMessage = e.what();
    }
    assert(runtimeErrorThrown);
    assert(errorMessage == "empty_stack");
}

//tests ActionStack top() on a 1 element ActionStack
void asTop1ElementTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    as.push(a);
    assert(as.top().character == a.character);
    assert(as.top().deleted == a.deleted);
    assert(as.top().column == a.column);
    assert(as.top().line == a.line);
}

//tests ActionStack top() on a 100 element ActionStack
void asTop100ElementTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 100; i++) {
        as.push(a);
    }
    assert(as.top().character == a.character);
    assert(as.top().deleted == a.deleted);
    assert(as.top().column == a.column);
    assert(as.top().line == a.line);
}

//tests ActionStack top() on a 100 element stack with a unique top
void asTop100ElementTopDifferentTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 99; i++) {
        as.push(a);
    }
    ActionStack::Action a2;
    a2.character = 'b';
    a2.deleted = false;
    a2.column = 1;
    a2.line = 1;
    as.push(a2);
    assert(as.top().character == a2.character);
    assert(as.top().deleted == a2.deleted);
    assert(as.top().column == a2.column);
    assert(as.top().line == a2.line);
}

//tests ActionStack push() 1 time
void asPushEmpyTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    as.push(a);
    assert(as.top().character == a.character);
    assert(as.top().deleted == a.deleted);
    assert(as.top().column == a.column);
    assert(as.top().line == a.line);
    assert(as.size() == 1);
}

//tests ActionStack push() 100 times
void asPush100ElementsTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 100; i++) {
        as.push(a);
    }
    assert(as.top().character == a.character);
    assert(as.top().deleted == a.deleted);
    assert(as.top().column == a.column);
    assert(as.top().line == a.line);
    assert(as.size() == 100);
}

//tests ActionStack push() 100 times with a unique element at the top
void asPush100ElementsFinalUniqueTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 99; i++) {
        as.push(a);
    }
    ActionStack::Action a2;
    a2.character = 'b';
    a2.deleted = false;
    a2.column = 1;
    a2.line = 1;
    as.push(a2);
    assert(as.top().character == a2.character);
    assert(as.top().deleted == a2.deleted);
    assert(as.top().column == a2.column);
    assert(as.top().line == a2.line);
    assert(as.size() == 100);
}

//tests ActionStack alternative push() 1 time
void asAltPushEmpyTest() {
    ActionStack as;
    as.push('a', true, 0, 0);
    assert(as.top().character == 'a');
    assert(as.top().deleted == true);
    assert(as.top().column == 0);
    assert(as.top().line == 0);
    assert(as.size() == 1);
}

//tests ActionStack alternative push() 100 times
void asAltPush100ElementsTest() {
    ActionStack as;
    for (int i = 0; i < 100; i++) {
        as.push('a', true, 0, 0);
    }
    assert(as.top().character == 'a');
    assert(as.top().deleted == true);
    assert(as.top().column == 0);
    assert(as.top().line == 0);
    assert(as.size() == 100);
}

//tests ActionStack alternative push() 100 times with a unique element 
//at the top
void asAltPush100ElementsFinalUniqueTest() {
    ActionStack as;
    for (int i = 0; i < 99; i++) {
        as.push('a', true, 0, 0);
    }
    as.push('b', false, 1, 1);
    assert(as.top().character == 'b');
    assert(as.top().deleted == false);
    assert(as.top().column == 1);
    assert(as.top().line == 1);
    assert(as.size() == 100);
}

//tests ActionStack pop() on an empty ActionStack
void asPopEmptyTest() {
    ActionStack as;
    bool runtimeErrorThrown = false;
    std::string errorMessage = "";
    try {
        as.top();
    } 
    catch (const std::runtime_error &e) {
        runtimeErrorThrown = true;
        errorMessage = e.what();
    }
    assert(runtimeErrorThrown);
    assert(errorMessage == "empty_stack");
}

//tests ActionStack pop() on a 1 element ActionStack
void asPopOneElementTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    as.push(a);
    as.pop();
    assert(as.size() == 0);
    assert(as.isEmpty() == true);
}

//tests ActionStack pop() on a 100 element ActionStack
void asPop100ElementsTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 100; i++) {
        as.push(a);
    }
    as.pop();
    assert(as.top().character == a.character);
    assert(as.top().deleted == a.deleted);
    assert(as.top().column == a.column);
    assert(as.top().line == a.line);
    assert(as.size() == 99);
    assert(as.isEmpty() == false);
}

//tests ActionStack pop() on an empty ActionStack
void asPop100ElementsTopUniqueTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 99; i++) {
        as.push(a);
    }
    ActionStack::Action a2;
    a2.character = 'b';
    a2.deleted = false;
    a2.column = 1;
    a2.line = 1;
    as.push(a2);
    as.pop();
    assert(as.top().character == a.character);
    assert(as.top().deleted == a.deleted);
    assert(as.top().column == a.column);
    assert(as.top().line == a.line);
    assert(as.size() == 99);
    assert(as.isEmpty() == false);
}

//tests ActionStack pop() to clear ActionStack
void asPopAllElements() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 100; i++) {
        as.push(a);
    }
    for (int i = 0; i < 100; i++) {
        as.pop();
    }
    assert(as.size() == 0);
    assert(as.isEmpty() == true);
}

//tests ActionStack clear() on an empty stack
void asClearEmptyTest() {
    ActionStack as;
    as.clear();
    assert(as.size() == 0);
    assert(as.isEmpty() == true);
}

//tests ActionStack clear() on a one element stack
void asClear1ElementTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    as.push(a);
    as.clear();
    assert(as.size() == 0);
    assert(as.isEmpty() == true);
}

//tests ActionStack clear() on a 100 element stack
void asClear100ElelmentsTest() {
    ActionStack as;
    ActionStack::Action a;
    a.character = 'a';
    a.deleted = true;
    a.column = 0;
    a.line = 0;
    for (int i = 0; i < 100; i++) {
        as.push(a);
    }
    as.clear();
    assert(as.size() == 0);
    assert(as.isEmpty() == true);
}