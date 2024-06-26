#ifndef AST_ASSIGNMENTOP_HPP
#define AST_ASSIGNMENTOP_HPP

#include "../ast_node.hpp"

class Assign : public Node
{
private:
    Node *identifier_;
    Node *value_;

public:
    Assign(Node *unary, Node *assign) : identifier_(unary), value_(assign){};
    virtual ~Assign()
    {
        delete identifier_;
        delete value_;
    }
    virtual void Print(std::ostream &stream) const override{};

    std::string getType() const override
    {
        return "operator";
    }

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        std::string mem = identifier_->getId();

        value_->EmitRISC(stream, context);
        std::string name = context.dst;
        /*if(context.datatype[identifier_->getId()] == "float"){
            context.WriteInstType("AssignFloat");
            value_->EmitRISC(stream,context);
            context.WriteInstType(" ");
        }
        else{
            value_->EmitRISC(stream, context);
        }*/

        if (identifier_->getType() == "array1")
        {
            mem = identifier_->getId() + std::to_string(identifier_->getSize());
        }

        if (identifier_->getType() == "array")
        {

            context.WriteInstType("AssignArray");
            identifier_->EmitRISC(stream, context);
            context.WriteInstType("None");
        }
        else if (identifier_->getType() == "struct"){
            context.WriteInstType("assign");
            identifier_ -> EmitRISC(stream,context); // should hv done it like this for all the assignment instead of doing it here lmao
            context.WriteInstType("None");
        }
        else if (identifier_->getType() == "pointer"){
            std:: string res = context.makeName("P");
            std:: string reg = context.AllocReg(res);
            stream << "lw " << reg << "," << context.MemoryMapping[mem] << "(sp)" << std::endl;
            stream << "sw " << context.bindings[context.dst] << ",0" << "(" << reg <<")" << std::endl;
            context.DeallocReg(res);
            context.DeallocReg(context.dst);
        }
        else if ((name[1] == 'F'))
        {
            stream << "fsw " << context.bindings[context.dst] << "," << context.MemoryMapping[mem] << "(sp)" << std::endl;
            context.DeallocReg(context.dst);
        }
        else if (name[1] == 'D'){
            stream << "fsd " << context.bindings[context.dst] << "," << context.MemoryMapping[mem] << "(sp)" << std::endl;
            context.DeallocReg(context.dst);
        }
        else
        {
            stream << "sw " << context.bindings[name] << "," << context.MemoryMapping[mem] << "(sp)" << std::endl;
            context.DeallocReg(context.dst);
        }
    };
    // this can be simplified
};

#endif
