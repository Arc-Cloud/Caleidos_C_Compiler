#ifndef AST_POINTER_HPP
#define AST_POINTER_HPP

#include "ast/ast_node.hpp"


class PointerDeclarator: public Node{
    private:
    Node*  point;
    public:
    PointerDeclarator(Node* _point): point(_point){};
    virtual ~PointerDeclarator(){
        delete point;
    }
    std:: string getId() const override{
        return point ->getId();
    }

    virtual void Print(std::ostream &stream) const override{};

    std:: string getType() const override{
        return "pointer";
    }

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
    };
};

#endif
