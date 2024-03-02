#ifndef AST_CONTEXT_HPP
#define AST_CONTEXT_HPP
#include <string>
#include <unordered_map>
#include <map>

// An object of class Context is passed between AST nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).
class Context
{
protected:
    int Reg[32] = {
        1,                            // x0 zero
        1,                            // x1 ra return address
        1,                            // x2 sp stack pointer
        1,                            // x3 gp global pointer
        1,                            // x4 tp thread pointer
        0, 0, 0,                      // x5, x6, x7 temporary registers
        1,                            // x8 s0 frame pointer
        1,                            // x9 s1 saved register 1
        1, 1,                         // x10,x11 a0, a1 return values
        0, 0, 0, 0, 0, 0,             // a2 -a6   function registers (cn use for anything really)
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // s2 - s11 saved registers
        0, 0, 0, 0                    // t3 - t6 temporary registers.
    };

    std::string InstType;
    std::unordered_map<std::string, std::string> bindings;
    std::map<std::string, int> MemoryMapping;
public:

    //allocate memory manually :) hope doesnt get larger than this
    int memDefault = 32;
    int lastmemused = 0;

    void WriteInstType(std::string input)
    {
        InstType = input;
    }

    std:: string ReadInstType(){
        return InstType;
    }
    ~Context(){};
};

#endif
