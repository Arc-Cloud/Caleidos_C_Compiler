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

        else if (expr -> getType() == "variable"){
            std:: string dst = context.AllocReg(expr->getId());
            std:: string label1 = context.makeName("L");
            std::string label2 = context.makeName("L");
            stream << "lw " << dst << "," << context.MemoryMapping[expr->getId()] << "(sp)" << std::endl;
            stream << "beq "<< dst << ",zero," << label1 << std::endl;
            context.DeallocReg(expr->getId());
            statement ->EmitRISC(stream,context);
            stream << "j " << label2 << std::endl;
            stream << label1 << ":" << std::endl;
            statement1 ->EmitRISC(stream,context);
            stream << label2 << ":" << std::endl;

        }
        else if (expr -> getType() == "operator"){
            expr -> EmitRISC(stream, context);
            std:: string dst = context.bindings[context.dst];
            std:: string label1 = context.makeName("L");
            std::string label2 = context.makeName("L");
            stream << "beq " << dst << ",zero," << label1 << std::endl;
            context.DeallocReg(context.dst);
            statement -> EmitRISC(stream,context);
            stream << "j " << label2 <<  std::endl;
            stream << label1 <<":" << std::endl;
            statement1 -> EmitRISC(stream, context);
            stream << label2 << ":" << std::endl;
        } 

    }

};

class IfNoElse: public Node{
    private:
    Node* expr;
    Node* statement;
    public:
    IfNoElse(Node* expr_, Node* statement_): expr(expr_), statement(statement_){};
    virtual ~IfNoElse(){
        delete expr;
        delete statement;
    }
    void Print(std::ostream &stream) const override{};
    void EmitRISC(std::ostream &stream, Context &context) const override
    {   
        std:: string label = context.makeName("L");
        expr -> EmitRISC(stream, context);
        stream << "beq " << context.bindings[context.dst] << ",zero," << label << std::endl;
        statement -> EmitRISC(stream, context);
        stream << label << ":" << std::endl;

    }

};

// need addition for the different cases of input





#endif
