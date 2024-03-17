#ifndef AST_ASSIGNMENTOP_HPP
#define AST_ASSIGNMENTOP_HPP

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

    std:: string getType() const override{
        return "operator";
    }

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        std:: string mem = identifier_->getId();
       
        value_-> EmitRISC(stream,context);

        if (identifier_ -> getType() == "array"){
             mem = identifier_->getId() + std::to_string(identifier_->getSize());
        }



        stream << "sw " << context.bindings[context.dst] << "," << context.MemoryMapping[mem] << "(sp)" << std:: endl;
        context.DeallocReg(context.dst);
    };
};


#endif
