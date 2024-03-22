#ifndef AST_TYPEDEF_HPP
#define AST_TYPEDEF_HPP

#include "../ast_node.hpp"

class TypeDef: public Node{
    private:
    Node* typespecfier;
    public:
    TypeDef(Node* Type): typespecfier(Type){};
    virtual ~TypeDef(){
        delete typespecfier;
    }

    std:: string getId() const override{
        return "typedef";
    }
    std:: string getType() const override{
        return typespecfier -> getType();
    }

    void EmitRISC(std::ostream &stream, Context &context) const override{};
    void Print(std::ostream &stream) const override{};
};

#endif