#ifndef AST_FOR_HPP
#define AST_FOR_HPP

#include "../ast_node.hpp"

class For: public Node {
private:
    Node* initExpr; // Initialization expression
    Node* condExpr; // Condition expression
    Node* incrExpr; // Increment expression, which might be null
    Node* statement; // Statement to execute in the loop

public:
    For(Node* initExpr_, Node* condExpr_, Node* incrExpr_, Node* statement_)
        : initExpr(initExpr_), condExpr(condExpr_), incrExpr(incrExpr_), statement(statement_) {};

    virtual ~For() {
        delete initExpr;
        delete condExpr;
        delete incrExpr; // how do we handle deletion if incrExpr is nullptr?
        delete statement;
    }

    void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (initExpr != nullptr) {
        initExpr->EmitRISC(stream, context);
        }




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
        //     stream << "ble " << dst << ",zero," << label2 << std::endl;
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
        //     stream << "ble " << context.bindings[context.dst] << ",zero," << label2 << std::endl;
        //     context.DeallocReg(context.dst);
        // }

    }

};



#endif
