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
    {}

    virtual std::string getId() const  override{
        return id;
    }

    virtual std:: string getType() const override{
        return "variable";
    }

    virtual void EmitRISC(std::ostream &stream, Context &context) const override{
        stream << "hi";
    }

    virtual void Print(std::ostream &stream) const override{};
};


class IntConstant: public Node{
    private:
    int value_;
    public:
    IntConstant(int value): value_(value){};

    virtual void EmitRISC(std::ostream &stream, Context &context) const override{
        stream << value_;
    }
    virtual void Print(std::ostream &stream) const override{};

    virtual std::string getType() const override{
        return "constant";
    }

    virtual int getVal() const override{
        return value_;
    }
};

#endif
