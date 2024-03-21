#ifndef AST_POINTERDEC_HPP
#define AST_POINTERDEC_HPP

#include "ast/ast_node.hpp"


class Dereference: public Node{
    private:
    Node*  id;
    public:
    Dereference(Node* _point): id(_point){};
    virtual ~Dereference(){
        delete id;
    }

    virtual void Print(std::ostream &stream) const override{};

    std:: string getType() const override{
        return "dereference";
    }

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        std:: string res = context.makeName("P");
        std:: string reg = context.AllocReg(res);
        stream << "addi " << reg << "," << "sp" << "," << context.MemoryMapping[id ->getId()] << std::endl;
        context.dst = res;
    };
};
#endif