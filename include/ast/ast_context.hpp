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
        0,                            // x4         tp          thread pointer
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
    int makeNameUnq = 0;
public:
    std:: string dst;
    std:: string EndLabel;
    int ParamCounter = 0;
    int ParamCounterF = 0;
    bool return_ = false;
    Context(){}
    void WriteInstType(std::string input)
    {
        InstType = input;
    }

    std:: string ReadInstType(){
        return InstType;
    }
    ~Context(){};

    std::string makeName(std::string base)
    {
    return "." + base + std::to_string(makeNameUnq++);
    }

    /// switch
    std::vector<std::string> labels;
    std::vector<int> case_num;
    std::string switch_end_label;
    ///

    ///global
    bool is_global = false;

    /// Enum
    std::unordered_map<std::string, int> enums;
    int enumcounter = 0;
    ///

    /// For
    std::string contLabel;
    ///

    /// Float
    std::map<std::string, int> FloatWords;
    std::map<std::string, std::string> datatype;
    ///

    /// string
    std::unordered_map<std::string, std::string> Strings;


    /// frame
    bool inFunc = false;
    bool scopeflag = false;
    int scopecount = 0;
    std:: vector <std:: string> varscope;
    std:: vector <int> scope;
    std:: vector <std:: map<std:: string, std::string>> frame_bind;
    std:: vector <std:: map<std:: string, int>> frame_mems;
    std:: vector <std:: map<std:: string, std::string>> frame_data;

     /*
        -----------------------------Register Management-------------------------------
    */
    std:: map<std:: string, std::string> bindings ;
    // std:: map<std:: string, std::string> bindingsFloat ;

    std:: string getDataType(std:: string input){
        return datatype[input];
    }
    void AssignType(std:: string input, std:: string type){
        datatype[input] = type;
    }
    void deleteType(std:: string input){
        datatype.erase(input);
    }

    std::string AllocReg(std::string var)
    {
        if (var[1] == 'F'|| var[1] == 'D')
        {
            for (int i = 4; i < 32; i++)
            {
                if (FloatReg[i] == 0)
                {
                    FloatReg[i] = 1;
                    bindings[var] = "f" + std::to_string(i);
                    return ("f" + std::to_string(i));
                }
            }
        }
        else
        {
            for (int i = 4; i < 32; i++)
            {
                if (Reg[i] == 0)
                {
                    Reg[i] = 1;
                    bindings[var] = "x" + std::to_string(i);
                    return ("x" + std::to_string(i));
                }
            }
        }
    }

    void DeallocReg(std::string var)
    {
        std::string reg = bindings[var].substr(1);
        int location = stoi(reg);
        if (var[1] == 'F' || var[1] == 'D')
        {
            if (FloatReg[location] == 0)
            {
                std::cerr << "the register was never assigned";
                exit(1);
            }
            else
            {
                FloatReg[location] = 0;
                bindings.erase(var);
            }
        }
        else
        {
            if (Reg[location] == 0)
            {
                std::cerr << "the register was never assigned";
                exit(1);
            }
            else
            {
                Reg[location] = 0;
                bindings.erase(var);
            }
        }
    }
    // void DeallocFloatReg(std:: string float_var){
    //     std:: string float_reg = bindingsFloat[float_var].substr(1);
    //     int location = stoi(float_reg);
    //     if (FloatReg[location] == 0){
    //         std:: cerr << "the register was never assigned";
    //         exit(1);
    //     }
    //     else{
    //         FloatReg[location] = 0;
    //         bindingsFloat.erase(float_var);
    //     }

    // }
    // std::string AllocFloatReg(std::string float_var)
    // {
    //     for (int i = 4; i < 32; i++)
    //     {
    //         if (FloatReg[i] == 0)
    //         {
    //             FloatReg[i] = 1;
    //             bindingsFloat[float_var] = "f" + std::to_string(i);
    //             return ("f" + std::to_string(i));
    //         }
    //     }
    // }

    /*
        -----------------------------FRAME MANAGEMENT-------------------------------

    */  

    int framecount = 0;
    void newFrame(Context &current){
         std::map<std::string, std::string> new_datatype;
         std:: map<std:: string, std::string> new_bindings;
         std::map<std::string, int> new_MemoryMapping;
         frame_bind.push_back(current.bindings);
         frame_mems.push_back(current.MemoryMapping);
         frame_data.push_back(current.datatype);
         current.bindings = new_bindings;
         current.MemoryMapping = new_MemoryMapping;
         current.datatype = new_datatype;
         framecount++;
        
    }

    void ExitFrame(Context &current){
        framecount--;
        current.bindings = frame_bind[framecount];
        current.MemoryMapping = frame_mems[framecount];
        current.datatype = frame_data[framecount];
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
