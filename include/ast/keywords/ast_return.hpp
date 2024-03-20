#ifndef AST_RETURN_HPP
#define AST_RETURN_HPP

#include "../ast_node.hpp"

class Return : public Node
{
protected:
    Node *ret;

public:
    Return(Node *expr) : ret(expr){};
    virtual ~Return()
    {
        delete ret;
    }

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        // if (ret->getType() == "constant")
        // {
        //     stream << "li a0, " << ret->getVal() << std::endl;

        // }
        // else if (ret->getType() == "variable"){
        //     std:: string var = ret->getId();
        //     context.AllocReg(var);
        //     stream << "lw " << context.bindings[var] << "," << context.MemoryMapping[var] << "(sp)" << std::endl;
        //     stream << "mv a0," << context.bindings[var] << std::endl;
        //     context.DeallocReg(var);

        // }
        // else if (ret -> getType() == "operator"){
        //     ret -> EmitRISC(stream, context);
        //     stream << "mv a0," << context.bindings[context.dst] << std::endl;
        //     context.DeallocReg(context.dst);
        // }
        // else if (ret -> getType() == "call"){
        //     ret -> EmitRISC(stream, context);
        // }


        // if (context.ReadInstType() == "float"){
        //     ret->EmitRISC(stream, context);
        //     stream << "fmv.s fa0," << context.bindingsFloat[context.dst] << std::endl;
        //     context.DeallocFloatReg(context.dst);
        //     context.return_ = true;
        //     stream << "j " << context.EndLabel << std::endl;
        // }
        // else{
            ret->EmitRISC(stream, context);
            std:: string holder = context.dst;
            if (holder[1] == 'F'){
                stream << "fmv.s fa0," << context.bindings[holder] << std::endl;
            }
            else if (holder[1] == 'D'){
                stream << "fmv.d fa0," << context.bindings[holder] << std::endl;
            }
            else{
            stream << "mv a0," << context.bindings[holder] << std::endl;
            }
            context.DeallocReg(holder);
            context.return_ = true;
            stream << "j " << context.EndLabel << std::endl;

        // }


    }
    virtual void Print(std::ostream &stream) const override{};
};

#endif
