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
    std:: string getType()const override{
        return "funcdec";
    }
    void EmitRISC(std::ostream &stream, Context &context) const override
    {       if (context.is_global != true){
            // stream << context.ReadInstType();
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
            }
            context.is_global = false;
            
        
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
        std::string type = Typespec_->getType();
        int datatype = Typespec_->getSize(); // will be useful later when we deal with numbers other than integer
        context.AssignType(var, type);
        std::string Insttype = init_->getType();
        if (Insttype == "array")
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
            if (context.inFunc){
                if (context.MemoryMapping.count(var)){
                   context.scope.push_back(context.MemoryMapping[var]);
                   context.varscope.push_back(var);
                   context.MemoryMapping.erase(var);
                   context.scopecount++;
                   context.scopeflag = true;
                   //what is this fuckery?? het at least it works
                }
            }
                context.AllocateStack(var);

        }

        if (context.ReadInstType() == "params")
        {
            if (context.getDataType(var) == "float")
            {
                stream << "fsw fa" << context.ParamCounterF++ << "," << context.MemoryMapping[var] << "(sp)" << std::endl;
            }
            else if (context.getDataType(var) == "double"){
                stream << "fsd fa" << context.ParamCounterF++ << "," << context.MemoryMapping[var] << "(sp)" << std::endl;
            }
            else
            {
                stream << "sw a" << context.ParamCounter++ << "," << context.MemoryMapping[var] << "(sp)" << std::endl;
            }
        }
        if (Insttype != "variable")
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

    void updateId(std:: string input) override{
        identifier_->updateId(input);
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
        if(context.datatype[identifier_->getId()] == "float"){
            value->EmitRISC(stream, context);
            stream << "fsw " << context.bindings[context.dst] << "," << context.MemoryMapping[identifier_->getId()] << "(sp)" << std::endl;
            context.DeallocReg(context.dst);
        }
        else{
        value->EmitRISC(stream, context);
        stream << "sw " << context.bindings[context.dst] << "," << context.MemoryMapping[identifier_->getId()] << "(sp)" << std::endl;
        context.DeallocReg(context.dst);
        }
    };

    void Print(std::ostream &stream) const override{};
};

#endif
