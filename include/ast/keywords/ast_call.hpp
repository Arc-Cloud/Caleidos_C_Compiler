#ifndef AST_CALL_HPP
#define AST_CALL_HPP

#include "../ast_node.hpp"

class Call : public Node
{
protected:
    Node *id;
    Node *args;

public:
    Call(Node *identifier_, Node *argument) : id(identifier_), args(argument){};
    virtual ~Call()
    {
        delete id;
        delete args;
    }
    std::string getType() const override
    {
        return "call";
    }

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        std::string res;
        std::string reg;
        if (args != NULL)
        {
            context.WriteInstType("call");
            args->EmitRISC(stream, context);
        }
        if (context.recurse)
        {   
            res = context.makeName("call");
            int tmp = context.savedreg++;
            reg = "s" + std::to_string(tmp);
            context.savedCounter.push_back(tmp);
            context.bindings[res] = reg;
            context.recurse = false;
            
        }
        else
        {
            res = context.makeName("call");
            reg = context.AllocReg(res);
            
        }

        stream << "call " << id->getId() << std::endl;
        stream << "mv " << reg << ",a0" << std::endl;
        context.dst = res;
        
    }

    virtual void Print(std::ostream &stream) const override{};
};

#endif
