#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast_node.hpp"
#include <string>

class DirectInit: public Node{
    private:
    Node* type;
    std::string id;
    Node* value;
    public:
    DirectInit(Node* _type,  const std::string  &name, Node* _value): type(_type), id(name), value(_value){};
    ~DirectInit(){
        delete type;
        delete value;
    }

    virtual void Print(std::ostream &stream) const override{};

    virtual void EmitRISC(std::ostream &stream, Context &context) const override{
                stream << id;

        }
};

#endif
