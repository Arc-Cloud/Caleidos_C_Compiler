#ifndef AST_ARRAY_HPP
#define AST_ARRAY_HPP

#include "../ast_node.hpp"

class DeclareArray : public Node
{
private:
    Node *id;
    Node *length;

public:
    DeclareArray(Node *identifier_, Node *length_) : id(identifier_), length(length_){};
    virtual ~DeclareArray()
    {
        delete id;
        delete length;
    }

    std:: string getId() const override{
        return id -> getId();
    }

    std:: string getType() const override{
        return "array";
    }

    int getSize() const override{
        return length -> getVal();
    }
    virtual void Print(std::ostream &stream) const override{};

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
    };
};

#endif
