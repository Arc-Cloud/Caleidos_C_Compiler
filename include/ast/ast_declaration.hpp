#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast_node.hpp"

class Declaration: public Node{
    protected:
    Node* _type;
    Node* id;
    public:
    Declaration(Node* type, Node* variable): _type(type), id(variable){};
    ~Declaration(){
        delete _type;
        delete id;
    }
    virtual void Print(std::ostream &stream) const override {};
    virtual void EmitRISC(std::ostream &stream, Context &context) const override {

    };
};

#endif
