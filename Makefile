# Makefile for Project 2: TypeWriter
# Tadhg Murray(tmurra01)
# 
# This Makefile will build unit_test for the unit_testing 
# framework, and compile the program

CXX=clang++
CXXFLAGS=-g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS=-g3


typewriter: main.o TextUI.o Editor.o ActionStack.o
	$(CXX) $(CXXFLAGS) $^ -lncurses -o typewriter

#this rule builds Main.o
main.o: main.cpp  Editor.o ActionStack.o
	$(CXX) $(CXXFLAGS) -c main.cpp

ActionStack.o: ActionStack.cpp ActionStack.h
	${CXX} ${CXXFLAGS} -c ActionStack.cpp

Editor.o: Editor.cpp Editor.h
	${CXX} ${CXXFLAGS} -c Editor.cpp

TypeWriter: Main.o Editor.o ActionStack.o
	$(CXX) $(CXXFLAGS) $^ -o typewriter

unit_test: unit_test_driver.o ActionStack.o
	${CXX} ${CXXFLAGS} unit_test_driver.o ActionStack.o

clean:
	rm -f main.o Editor.o ActionStack.o typewriter