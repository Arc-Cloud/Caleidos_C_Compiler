#ifndef AST_ARGUMENT_HPP
#define AST_ARGUMENT_HPP


#include "../ast_node.hpp"

class Argument: public NodeList {
    private:
    std:: vector <Node*> args;
    public:
    Argument(Node* first_): args({first_}){};
    virtual ~Argument(){
        for (auto node : args)
        {
            delete node;
        }
    }

    void Print(std::ostream &stream) const override{};

    void PushBack(Node *item) override
    {
        args.push_back(item);
    }

     virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        for (auto node : args)
        {
            if (node == nullptr)
            {
                continue;
            }
            node -> EmitRISC(stream,context);
            stream << "mv a" << context.ParamCounter++ << "," << context.bindings[context.dst] << std::endl;
            context.DeallocReg(context.dst);
        }
    }

};
    
#endif
