#ifndef AST_FOR_HPP
#define AST_FOR_HPP

#include "../ast_node.hpp"

class For: public Node {
private:
    Node *initExpr; // Initialization expression
    Node *condExpr; // Condition expression
    Node *incrExpr; // Increment expression, which might be null
    Node *statement; // Statement to execute in the loop

public:
    For(Node *initExpr_, Node *condExpr_, Node *incrExpr_, Node *statement_)
        : initExpr(initExpr_), condExpr(condExpr_), incrExpr(incrExpr_), statement(statement_) {};

    virtual ~For() {
        delete initExpr;
        delete condExpr;
        if (incrExpr != nullptr) {
        delete incrExpr;
        }
        delete statement;
    }

    void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        std::string label1 = context.makeName("L");
        std::string label2 = context.makeName("L");

        initExpr->EmitRISC(stream, context);



        stream << "j " << label2 << std::endl;
        stream << label1 << ":" << std::endl;

        statement->EmitRISC(stream, context);

        if (incrExpr != nullptr) {
        incrExpr->EmitRISC(stream, context);
        }

        stream << label2 << ":" << std::endl;
        condExpr->EmitRISC(stream, context);
        stream << "bne " << context.bindings[context.dst] << ",zero," << label1 << std::endl;
        context.DeallocReg(context.dst);

    }

};



#endif
