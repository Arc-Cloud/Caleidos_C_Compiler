#ifndef AST_GLOBAL_HPP
#define AST_GLOBAL_HPP

#include "../ast_node.hpp"


class Global: public Node{
    private:
    Node* globl;
    public:
    Global(Node* glob): globl(glob){};
    virtual ~Global(){
        delete globl;
    }
    void Print(std::ostream &stream) const override{};
     void EmitRISC(std::ostream &stream, Context &context) const override
    {   

        context.is_global = true;
        // stream << globl -> getType();

        globl->EmitRISC(stream,context);
    }

};


#endif
