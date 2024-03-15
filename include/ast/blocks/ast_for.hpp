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

        std::string label1 = context.makeName("L");
        std::string label2 = context.makeName("L");

        stream << label1 << ":" << std::endl;

        if (condExpr != nullptr) {
        condExpr->EmitRISC(stream, context);
        stream << "beqz " << context.bindings[context.dst] << ", " << label2 << std::endl;
        }

        if (statement != nullptr) {
        statement->EmitRISC(stream, context);
        }

        if (incrExpr != nullptr) {
        incrExpr->EmitRISC(stream, context);
        }

        stream << "j " << label1 << std::endl;

        stream << label2 << ":" << std::endl;

    }

};



#endif
