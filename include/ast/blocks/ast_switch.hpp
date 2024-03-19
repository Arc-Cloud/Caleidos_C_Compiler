#ifndef AST_SWITCH_HPP
#define AST_SWITCH_JPP


#include "../ast_node.hpp"

class Switch: public Node{
    private:
    Node *expr;
    Node *statement;
    public:
    Switch(Node *expr_, Node *statement_): expr(expr_), statement(statement_){};
    virtual ~Switch(){
        delete expr;
        delete statement;
    }
    void Print(std::ostream &stream) const override{};
    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        context.WriteInstType("load");

        statement->EmitRISC(stream, context);

        int num_cases = context.case_num.size();

        for(int i = 0; i < num_cases; i++){
            std::string label = context.makeName("L");
            context.labels.push_back(label);
        }
        std::string default_label = context.makeName("L");
        std::string endlabel = context.makeName("L");
        context.switch_end_label = endlabel;

        for(int i = num_cases; i > 0; i--){
            expr -> EmitRISC(stream, context);
            std:: string switchVal = context.dst;
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            int case_num = context.case_num.back();
            stream << "li " << res << "," << case_num << std::endl;
            stream << "beq " << context.bindings[switchVal] << "," << res << "," << context.labels[i-1] << std::endl;
            context.DeallocReg(op);
            context.DeallocReg(switchVal);
            context.case_num.pop_back();
        }

        stream << "j " << default_label << std::endl;

        context.WriteInstType("emit");

        statement->EmitRISC(stream, context);

        context.WriteInstType("done");

        stream << default_label << ":" << std::endl;
        statement->EmitRISC(stream, context);

        stream << endlabel << ":" << std::endl;
    }


};


class Case: public Node{
    private:
    Node *expr;
    Node *statement;
    public:
    Case(Node *expr_, Node *statement_): expr(expr_), statement(statement_){};
    virtual ~Case(){
        delete expr;
        delete statement;
    }
    void Print(std::ostream &stream) const override{};
    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (context.ReadInstType() == "load"){
            context.case_num.push_back(expr->getVal());
        }
        else if(context.ReadInstType() == "emit") {
        std::string label = context.labels.front();
        stream << label << ":" << std::endl;
        statement->EmitRISC(stream, context);
        context.labels.erase(context.labels.begin());
        }

    }

};

class Break : public Node
{
public:

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if(context.ReadInstType() == "emit") {
            stream << "j " << context.switch_end_label << std::endl;
        }
    }
    virtual void Print(std::ostream &stream) const override{};
};

class Default : public Node
{
protected:
    Node *statement;

public:
    Default(Node *statement_): statement(statement_){};
    virtual ~Default()
    {
        delete statement;
    }
    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if(context.ReadInstType() == "done") {

            statement->EmitRISC(stream, context);

        }
    }
    virtual void Print(std::ostream &stream) const override{};
};

#endif

