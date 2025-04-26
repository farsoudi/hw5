#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <vector>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

typedef std::vector<char> cvec;


// Add prototypes of helper functions here
namespace Helpers {
    //fn for beginning convert - to empty spots
    cvec inToArray(const std::string& in);
    //object function that has internal reference to dict and applies to return object if valid
    struct Applier {
        public:
            Applier(const std::set<std::string>& dict) : dict(dict) {};
            bool operator()(cvec word);
            std::set<std::string> operator*() {return ret;}

        private:
            const std::set<std::string>& dict;
            std::set<std::string> ret;

    };
    //fn to get next empty index
    size_t next(size_t i, const cvec& original);
    //Struct that has pointers to common data to avoid re-passing data over and over on the stack for helper functions in global scope.
    struct Recursor {
        public:
            Recursor(const cvec& yellows, Helpers::Applier& apply) : apply(apply), yellows(yellows) {}
            void operator()(
                cvec last_word,
                size_t index,
                cvec remaining_yellows
            );
        private:
        const cvec& yellows;
        Helpers::Applier& apply;
    };
};


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    //Holds the original vector to work off
    const cvec original(Helpers::inToArray(in));
    //applier obj
    Helpers::Applier apply(dict);
    //Holds vector of characters we must use, and a obj to know where it is to recurse over it.
    const cvec yellows(floating.begin(), floating.end());
    Helpers::Recursor recurse(yellows, apply);

    if (in.empty())
        return std::set<std::string> ();
    
    //recursive step
    recurse(
        original,
        Helpers::next(0, original),
        yellows
    );

    return *apply;
}

// Define any helper functions here
void Helpers::Recursor::operator()(
    cvec last_word,
    size_t index,
    cvec remaining_yellows
) {
    // Implementation goes here
    if(!remaining_yellows.empty()) { //meaning we havent applied all the yellows 
        char currentYellow = remaining_yellows.front();
        cvec updated_yellows = remaining_yellows;
        updated_yellows.erase(updated_yellows.begin());

        for (size_t column = index ; column < last_word.size() ; column = next(column+1, last_word)) {
            //next word
            cvec next = last_word;
            next[column] = currentYellow;

            //recursive
            (*this)(
                next,
                Helpers::next(index, next),
                updated_yellows
            );
        }
    } else { //done adding yellows

        if (index < last_word.size()) { //still recursing to do, but without yellows.
            for (char c = 'a'; c <= 'z'; ++c) {
                cvec next = last_word;
                next[index] = c;

                // Recurse to the next empty spot
                (*this)(
                    next, 
                    Helpers::next(index, next), 
                    remaining_yellows
                );
            }

        } else { //full word
            apply(last_word);
        }
    }

}


std::vector<char> Helpers::inToArray(const std::string& in) {
    std::vector<char> ret;
    for (char c : in) {
        ret.push_back(c == '-' ? '\0' : c);
    }
    return ret;
}

bool Helpers::Applier::operator()(std::vector<char> word) {
    std::string temp(word.begin(), word.end()); //convs back to string
    std::set<std::string>::iterator it = dict.find(temp);
    if (it == dict.end()) return false;
    ret.insert(temp);
    return true;
}

size_t Helpers::next(size_t i,  const std::vector<char>& original) {
    while (i < original.size() && original[i] != '\0') ++i;
    return i;
}

