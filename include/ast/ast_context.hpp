#ifndef AST_CONTEXT_HPP
#define AST_CONTEXT_HPP
#include <string>

// An object of class Context is passed between AST nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).
class Context
{
protected:
    std::string InstType;

public:
    void WriteInstType(std::string input)
    {
        InstType = input;
    };

    std:: string ReadInstType (){
        return InstType;
    }
};

#endif
