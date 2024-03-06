#ifndef AST_ASSIGN_HPP
#define AST_ASSIGN_HPP

#include "../ast_node.hpp"

class Assign : public Node{
    private:
    Node* identifier_;
    Node* value_;
    public:
    Assign(Node* unary, Node* assign): identifier_(unary), value_(assign){};
    virtual ~Assign(){
        delete identifier_;
        delete value_;
    }
    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (value_ ->getType() == "constant"){
            context.AllocReg(identifier_->getId());
            std:: string dst = context.bindings[identifier_->getId()];
            context.DeallocReg(identifier_->getId());
            stream << "li " << dst << "," << value_ ->getVal() << std:: endl;
            stream << "sw " << dst << "," << context.MemoryMapping[identifier_->getId()] << "(sp)" << std::endl;
        }
        else if (value_ -> getType() == "variable"){
            context.AllocReg(identifier_->getId());
            std:: string dst = context.bindings[identifier_->getId()];
            context.DeallocReg(identifier_->getId());
            stream << "lw " << dst << "," << context.MemoryMapping[value_ ->getId()] << "(sp)" << std::endl;
            stream << "sw " << dst << "," << context.MemoryMapping[identifier_ ->getId()] << "(sp)" << std::endl;
        }
    };
};


#endif
