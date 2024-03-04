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
        stream << "hi";
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
    {
        stream << value_;
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
