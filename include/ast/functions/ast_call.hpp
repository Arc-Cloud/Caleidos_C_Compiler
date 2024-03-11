#ifndef AST_CALL_HPP
#define AST_CALL_HPP

#include "../ast_node.hpp"

class Call : public Node
{
protected:
    Node *id;

public:
    Call(Node *identifier_) : id(identifier_){};
    virtual ~Call()
    {
        delete id;
    }
    std:: string getType() const override{
        return "call";
    }

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        stream << "call " << id ->getId() << std::endl;
    }

    virtual void Print(std::ostream &stream) const override{};
};

#endif
