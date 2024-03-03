#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast_node.hpp"

class Declaration : public Node
{
protected:
    Node *_type;
    std::string id;

public:
    Declaration(Node *type, std::string variable) : _type(type), id(variable){};
    ~Declaration()
    {
        delete _type;
    }
    virtual void Print(std::ostream &stream) const override{};
    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        auto find = context.MemoryMapping.find(id);
        if (find == context.MemoryMapping.end())
        {
            for (int i = 0; i < _type->getSize(); i++)
            {
                context.MemoryMapping[id][i] = context.memAlloc();
            }
        }
    }
};

class Assignment : public Node
{
protected:
    std::string id;
    Node *constant;

public:
    Assignment(std::string _identifier, Node *value) : id(_identifier), constant(value){};
    ~Assignment()
    {
        delete constant;
    }
    virtual void Print(std::ostream &stream) const override{};
    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        stream << "li a5," << constant->getVal() << std::endl;
        stream << "sw a5," << context.MemoryMapping[id][0] << "(s0)" << std::endl;
        // need to change implementation for better scaling
    }
};

#endif
