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

void asIsEmptyTest() {
    ActionStack a;
    assert(a.isEmpty() == true);
}