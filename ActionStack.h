/*
 * ActionStack.h
 *
 * CS15 Proj 2: Typewriter
 * 
 * The ActionStack class is a stack of Action instances, where each Action
 * captures an action performed by the user. The interface enables the client
 * to perform standard stack operations.
 *
 * Author: Milod Kazerounian (Sept 2025)
 */

#ifndef __ACTIONSTACK_H__
#define __ACTIONSTACK_H__

#include <vector>
class ActionStack {
    public:
        /* The ActionStack will store Action instances, which capture actions
        * performed by the user. An Action comprises of: a `character`,
        * a bool denoting whether or not that character was `deleted`, and
        * a `line` and `column` for the character.
        */ 
        struct Action {
            char character;
            bool deleted;
            size_t column;
            size_t line;
        };

        /* Constructor, destructor, and member functions.
        * See the spec for the expected behavior of each of these.
        */
        ActionStack();
        ~ActionStack();

        bool isEmpty() const;
        int size() const;
        Action top() const;
        void pop();
        void push(Action elem);
        void push(char c, bool was_delete, std::size_t line, 
                    std::size_t column);
        void clear();

    private:
        // Add any private member variables or functions as you see fit.
        std::vector <Action> stack;
};





#endif
