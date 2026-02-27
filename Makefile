# Makefile for Project 2: TypeWriter
# Tadhg Murray(tmurra01)
# 
# This Makefile will build unit_test for the unit_testing 
# framework, and build typewriter to compile/run the program

CXX=clang++
CXXFLAGS=-g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS=-g3

#this rule builds the executable
typewriter: main.o TextUI.o Editor.o ActionStack.o
	$(CXX) $(CXXFLAGS) $^ -lncurses -o typewriter

#this rule builds main.o
main.o: main.cpp  Editor.o ActionStack.o
	$(CXX) $(CXXFLAGS) -c main.cpp

#this rule builds ActonStack.o
ActionStack.o: ActionStack.cpp ActionStack.h
	${CXX} ${CXXFLAGS} -c ActionStack.cpp

#this rule builds Editor.o
Editor.o: Editor.cpp Editor.h ActionStack.h
	${CXX} ${CXXFLAGS} -c Editor.cpp

#this rule builds the unit tests
unit_test: unit_test_driver.o ActionStack.o
	${CXX} ${CXXFLAGS} unit_test_driver.o ActionStack.o

#this rule clears the .o files(except for TextUI) and exectutable
clean:
	rm -f main.o Editor.o ActionStack.o typewriter