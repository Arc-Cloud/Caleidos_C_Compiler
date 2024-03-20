#ifndef AST_PRIMITIVES_HPP
#define AST_PRIMITIVES_HPP

#include <string>
#include <iostream>

#include "../ast_node.hpp"

class Variable
    : public Node
{
private:
    std::string id;

public:
    Variable(std::string _id)
        : id(_id)
    {
    }


    std::string getId() const override
    {
        return id;
    }

    void updateId(std:: string input) override{
        id = input;
    }

    std::string getType() const override
    {
        return "variable";
    }

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if(context.datatype[id] == "float"){
            std:: string variable_float = context.makeName("F");
            std:: string float_res = context.AllocReg(variable_float);
            stream << "flw " <<  float_res  << "," << context.MemoryMapping[id] << "(sp)" << std::endl;
            // if function is of return type (belongs elsewhere): int stream << "fcvt.w.s " << res << "," << float_res << ",rtz" << std::endl;
            context.dst = variable_float;
        }
        else if (context.datatype[id] == "double"){
            std:: string variable = context.makeName("D");
            std:: string res = context.AllocReg(variable);
            stream << "fld " << res << "," << context.MemoryMapping[id] << "(sp)" << std::endl;
            context.dst = variable;
        }
        else if (context.enums.count(id)){
            std:: string variable_ = context.makeName("V");
            std:: string res = context.AllocReg(variable_);
            stream << "li " << res << "," << context.enums[id] << std::endl;
            context.enums.erase(id);
            context.dst = variable_;
        }
        else{
            std:: string variable_ = context.makeName("V");
            context.AssignType(variable_, context.getDataType(id));
            std:: string res = context.AllocReg(variable_);
            stream << "lw " <<  res  << "," << context.MemoryMapping[id] << "(sp)" << std::endl;
            context.dst = variable_;
        }
    }

    void Print(std::ostream &stream) const override{};
};

class IntConstant : public Node
{
private:
    int value_;

public:
    IntConstant(int value) : value_(value){};

    void EmitRISC(std::ostream &stream, Context &context) const override{
    //    if (context.ReadInstType() == "call"){
    //         stream << "li a" << context.ParamCounter++ << "," << value_<< std::endl;
    //     }
    //     else{
        std:: string constant = context.makeName("I");
        std:: string res = context.AllocReg(constant);
        stream << "li " << res << "," << value_ << std::endl;
        context.dst = constant;
    }
    void Print(std::ostream &stream) const override{};

    std::string getType() const override
    {
        return "constant";
    }

    int getVal() const override
    {
        return value_;
    }
};

class FloatConstant : public Node
{
private:
    float value_;

public:
    FloatConstant(float value) : value_(value){};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (context.ReadInstType() == "call"){
            stream << "li a" << context.ParamCounter++ << "," << value_<< std::endl;
        }
        else{
                float val = value_;
                unsigned int ieee754 = *reinterpret_cast<unsigned int*>(&val);

                std::string label = context.makeName("LC");
                context.FloatWords[label] = ieee754;

                std:: string constant_hi = context.makeName("CH");
                std:: string float_lo = context.makeName("FL");

                std:: string high = context.AllocReg(constant_hi);
                std:: string low = context.AllocReg(float_lo);
                stream << "lui " << high << ",\%hi" << "("  << label << ")" << std::endl;
                stream << "flw " << low << ",\%lo" << "("  << label << ")" << "(" << high << ")" <<std::endl;
                context.DeallocReg(constant_hi);
                context.dst = float_lo;
        }
    }

    void Print(std::ostream &stream) const override{};

    std::string getType() const override
    {
        return "float";
    }

    int getVal() const override
    {
        return value_;
    }
};

#endif
