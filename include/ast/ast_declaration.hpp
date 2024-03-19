#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast_node.hpp"
#include <string>

class FunctionDeclarator : public Node
{
private:
    Node *identifier_;
    NodeList *parameter;

public:
    FunctionDeclarator(Node *identifier, NodeList *param) : identifier_(identifier), parameter(param){};
    virtual ~FunctionDeclarator()
    {
        delete identifier_;
        delete parameter;
    };
    void EmitRISC(std::ostream &stream, Context &context) const override
    {   
        stream << ".globl " << identifier_->getId() << std::endl;
        stream << identifier_->getId() << ":" << std::endl;
        stream << "addi sp,sp,-" << context.memDef() << std::endl;
        stream << "sw ra," << std::to_string(context.AllocateStack("ra")) << "(sp)" << std::endl;
        stream << "sw s0," << std::to_string(context.AllocateStack("s0")) << "(sp)" << std::endl;
        stream << "addi s0,sp," << context.default_mem << std::endl;
        if (parameter != NULL && parameter->getSize() < 9)
        {
            context.WriteInstType("params");
            parameter->EmitRISC(stream, context);
            context.ParamCounter = 0; // pay attention to this;
        }
    };
    void Print(std::ostream &stream) const override{};
};

class Declaration : public Node
{
private:
    Node *Typespec_;
    Node *init_;

public:
    Declaration(Node *type, Node *init) : Typespec_(type), init_(init){};
    virtual ~Declaration()
    {
        delete Typespec_;
        delete init_;
    }

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        std::string var = init_->getId();
        // context.AllocReg(var);
        int datatype = Typespec_->getSize(); // will be useful later when we deal with numbers other than integer
        std::string type = init_->getType();
        if (type == "array")
        {
            int size = init_->getSize();
            for (int i = 0; i < size; i++)
            {
                std::string mem = var + std::to_string(i);
                context.AllocateStack(mem);
            }
        }
        else
        {
            context.AllocateStack(var);
        }
        if (context.ReadInstType() == "params")
        {
            stream << "sw a" << context.ParamCounter++ << "," << context.MemoryMapping[var] << "(sp)" << std::endl;
        }
        if (type != "variable")
        {
            init_->EmitRISC(stream, context);
        }
    };

    void Print(std::ostream &stream) const override{};
};

class InitDeclarator : public Node
{
private:
    Node *identifier_;
    Node *value;

public:
    InitDeclarator(Node *id, Node *val) : identifier_(id), value(val){};
    virtual ~InitDeclarator()
    {
        delete identifier_;
        delete value;
    }
    std::string getType() const override
    {
        return "InitDeclarator";
    }

    std::string getId() const override
    {
        return identifier_->getId();
    }

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        // if (value->getType() == "constant")
        // {
        //     std::string dst = context.AllocReg(identifier_->getId());
        //     context.DeallocReg(identifier_->getId());
        //     stream << "li " << dst << "," << value->getVal() << std::endl;
        //     stream << "sw " << dst << "," << context.MemoryMapping[identifier_->getId()] << "(sp)" << std::endl;
        // }
        // else if (value->getType() == "operator")
        // {
        //     value->EmitRISC(stream, context);
        //     stream << "sw " << context.bindings[context.dst] << "," << context.MemoryMapping[identifier_->getId()] << "(sp)" << std::endl;
        //     context.DeallocReg(context.dst);
        // }

        value->EmitRISC(stream, context);
        stream << "sw " << context.bindings[context.dst] << "," << context.MemoryMapping[identifier_->getId()] << "(sp)" << std::endl;
        context.DeallocReg(context.dst);
    };

    void Print(std::ostream &stream) const override{};
};

#endif
