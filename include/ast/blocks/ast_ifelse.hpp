#ifndef AST_IFELSE_HPP
#define AST_IFELSE_JPP


#include "../ast_node.hpp"

class IfElse: public Node{
    private:
    Node* expr;
    Node* statement;
    Node* statement1;
    public:
    IfElse(Node* expr_, Node* statement_, Node* statement1_): expr(expr_), statement(statement_), statement1(statement1_){};
    virtual ~IfElse(){
        delete expr;
        delete statement;
        delete statement1;
    }
    void Print(std::ostream &stream) const override{};
    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (expr ->getType() == "constant"){
            if (expr ->getVal() != 0){
                statement ->EmitRISC(stream, context);
            }
            else{
                statement1 ->EmitRISC(stream,context);
            }
        }

    }

};

// need addition for the different cases of input





#endif
