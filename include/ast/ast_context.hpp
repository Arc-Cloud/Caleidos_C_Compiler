#ifndef AST_CONTEXT_HPP
#define AST_CONTEXT_HPP
#include <string>
#include <unordered_map>
#include <map>
// An object of class Context is passed between AST nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).
#include <set>

//this thing is so messy but hey it works hehe would probably clean it up if we had time
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
    ///recursion
    bool recurse = false;
    int savedreg = 1; //lets hope this doesnt get bigger than 7
    std:: vector <int> savedCounter;

    /// string
    std:: unordered_map<std:: string, std::string> StringsHolder;
    std:: set<std::string> StringVar;

    /// Float
    std::map<std::string, int> FloatWords;
    std::map<std::string, std::string> datatype;

    std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> DoubleWords;
    ///

    /// Struct
    // this is so stupid i love it
    std:: string currStruct;
    std:: map <std:: string , int> StructMem;
    std:: map <std:: string, std:: map <std::string, int>> StructMapping;
    std:: map <std:: string, std:: string> StructMap;
    int structsize = 0;
    // int StructSizeCounter;

    ///

    /// string
    std::unordered_map<std::string, std::string> Strings;
    ///

    ///pointer
    std:: set <std:: string> pointerlist;


    /// frame
    int ParamCounter = 0;
    std:: string EndLabel;
    int ParamCounterF = 0;
    bool inFunc = false;
    bool scopeflag = false;
    int scopecount = 0;
    std:: vector <std:: string> varscope;
    std:: vector <int> scope;
    std:: vector <std:: map<std:: string, std::string>> frame_bind;
    std:: vector <std:: map<std:: string, int>> frame_mems;
    std:: vector <std:: map<std:: string, std::string>> frame_data;

    //register holder
    std:: string dst;

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
    int default_mem = 128;
    int LastStack = 128;
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

 /*
        -----------------------------Data size-------------------------------
    */

   int data_size(std::string type){
    if(type == "void"){
        return 1;
    }
    else if(type == "char"){
        return 1;
    }
    else if(type == "int"){
        return 4;
    }
    else if(type == "float"){
        return 4;
    }
    else if(type == "double"){
        return 8;
    }
    else if(type == "unsigned"){
        return 4;
    }
    else if(type == "signed"){
        return 4;
    }
    else{
        return 0;
    }

   }

};

#endif
