#ifndef AST_WHILE_HPP
#define AST_WHILE_HPP

#include "../ast_node.hpp"
class While: public Node{
    private:
    Node* expr;
    Node* statement;
    public:

    While(Node* expr_, Node* statement_): expr(expr_), statement(statement_){};
    virtual ~While(){
        delete expr;
        delete statement;
    }

    void Print(std::ostream &stream) const override{};
    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        // if (expr ->getType() == "constant"){
        //     while (expr -> getVal()){
        //         statement -> EmitRISC(stream, context);
        //     }
        // }
        // else if (expr ->getType() == "variable"){
        //     std:: string label1 = context.makeName("L");
        //     std::string label2 = context.makeName("L");
        //     stream << "j " << label1 << std::endl;
        //     stream << label2 << ":" << std::endl;
        //     statement -> EmitRISC(stream, context);
        //     stream << label1 << ":" << std::endl;
        //     std:: string dst = context.AllocReg(expr->getId());
        //     stream << "lw " << dst << "," << context.MemoryMapping[expr->getId()] << "(sp)" << std::endl;
        //     stream << "bne " << dst << ",zero," << label2 << std::endl;
        //     context.DeallocReg(expr->getId());
        //     // theres a mistake here with the dealloc
        // }
        // else if (expr -> getType() == "operator"){
        //     std:: string label1 = context.makeName("L");
        //     std::string label2 = context.makeName("L");
        //     stream << "j " << label1 << std::endl;
        //     stream << label2 << ":" << std::endl;
        //     statement -> EmitRISC(stream, context);
        //     stream << label1 << ":" << std:: endl;
        //     expr -> EmitRISC(stream, context);
        //     stream << "bne " << context.bindings[context.dst] << ",zero," << label2 << std::endl;
        //     context.DeallocReg(context.dst);
        // }

        std:: string label1 = context.makeName("L");
        std:: string label2 = context.makeName("L");
        stream << "j " << label1 << std::endl;
        stream << label2 << ":" << std::endl;
        statement -> EmitRISC(stream,context);
        stream << label1 << ":" << std::endl;
        expr -> EmitRISC(stream,context);
        stream << "bne " << context.bindings[context.dst] << ",zero," << label2 << std::endl;
        context.DeallocReg(context.dst);

    }

};
















#endif
