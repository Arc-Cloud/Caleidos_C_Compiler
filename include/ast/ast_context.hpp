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
        1,                            // x0         zero
        1,                            // x1         ra          return address
        1,                            // x2         sp          stack pointer
        1,                            // x3         gp          global pointer
        1,                            // x4         tp          thread pointer
        0, 0, 0,                      // x5 - x7    t0 - t2     temporary registers
        1,                            // x8         s0          frame pointer
        1,                            // x9         s1          saved register 1
        1, 1,                         // x10,x11    a0, a1      return values
        0, 0, 0, 0, 0, 0,             // x12 - x17  a2 - a7     function registers (cn use for anything really)
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // x18 - x27  s2 - s11    saved registers
        0, 0, 0, 0                    // x28 - x31  t3 - t6     temporary registers.
    };

    int FloatReg[32] = {
        1,                            // f0         zero
        1,                            // f1         ra          return address
        1,                            // f2         sp          stack pointer
        1,                            // f3         gp          global pointer
        1,                            // f4         tp          thread pointer
        0, 0, 0,                      // f5 - f7    t0 - t2     temporary registers
        1,                            // f8         s0          frame pointer
        1,                            // f9         s1          saved register 1
        1, 1,                         // f10,f11    a0, a1      return values
        0, 0, 0, 0, 0, 0,             // f12 - f17  a2 - a7     function registers (cn use for anything really)
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // f18 - f27  s2 - s11    saved registers
        0, 0, 0, 0                    // f28 - f31  t3 - t6     temporary registers.
    };

    std::string InstType; // to inform the next node the type of operation currently doing
public:
    std:: string dst;
    int ParamCounter = 0;
    Context(){}
    void WriteInstType(std::string input)
    {
        InstType = input;
    }

    std:: string ReadInstType(){
        return InstType;
    }
    ~Context(){};


     /*
        -----------------------------Register Management-------------------------------
    */
    std:: map<std:: string, std::string> bindings;
    std:: string AllocReg(std:: string var){
        for (int i = 4; i < 32; i++){
            if (Reg[i] == 0){
                Reg[i] = 1;
                bindings[var] = "x" + std::to_string(i);
                return ("x" + std::to_string(i));
            }
        }
    }

    void DeallocReg(std:: string var){
        std:: string reg = bindings[var];
        int location = reg[1] - '0';
        if (Reg[location] == 0){
            std:: cerr << "the register was never assigned";
            exit(1);
        }
        else{
            Reg[location] = 0;
            bindings.erase(var);
        }

    }





     /*
        -----------------------------MEMORY MANAGEMENT-------------------------------
    */
    //probably not enough lmao
    int default_mem = 64;
    int LastStack = 64;
    std::map<std::string, int> MemoryMapping; // to track where the value of a varibale is stored in mem.for riscv all local variables are always stored in mem


    int AllocateStack(std:: string input){
        if (LastStack == 0){
            std:: cerr << "overflow" << std::endl;
            exit(1);
        }
        else {
            LastStack -= 4;
            MemoryMapping[input] = LastStack;
            return LastStack;
        }
    }


    int memDef(){
        LastStack = default_mem;
        return default_mem;
    }
};

#endif
