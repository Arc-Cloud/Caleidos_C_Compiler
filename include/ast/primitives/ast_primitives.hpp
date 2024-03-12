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

    std::string getType() const override
    {
        return "variable";
    }

    void EmitRISC(std::ostream &stream, Context &context) const override
    {   
        std:: string variable_ = context.makeName("V");
        std:: string res = context.AllocReg(variable_);
        stream << "lw " <<  res  << "," << context.MemoryMapping[id] << "(sp)" << std::endl;
        context.dst = variable_;
    }

    void Print(std::ostream &stream) const override{};
};

class IntConstant : public Node
{
private:
    int value_;

public:
    IntConstant(int value) : value_(value){};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {   if (context.ReadInstType() == "call"){
            stream << "li a" << context.ParamCounter++ << "," << value_<< std::endl;
        }
        else{
        std:: string constant = context.makeName("I");
        std:: string res = context.AllocReg(constant);
        stream << "li " << res << "," << value_ << std::endl;
        context.dst = constant;
        }
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

#endif
