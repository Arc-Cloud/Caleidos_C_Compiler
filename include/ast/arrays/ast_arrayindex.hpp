#ifndef AST_ARRAYINDEX_HPP
#define AST_ARRAYINDEX_HPP

#include "../ast_node.hpp"

class ArrayIndex : public Node
{
private:
    Node *id;
    Node *index;

public:
    ArrayIndex(Node *identifier_, Node *length_) : id(identifier_), index(length_){};
    virtual ~ArrayIndex()
    {
        delete id;
        delete index;
    }

    std::string getId() const override
    {
        return id->getId();
    }

    std::string getType() const override
    {
        if (index->getType() == "constant")
        {
            return "array1";
        }
        else
        {
            return "array";
        }
    }

    int getSize() const override
    {
        return index->getVal();
    }
    virtual void Print(std::ostream &stream) const override{};

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (context.ReadInstType() == "AssignArray")
        {       
                std:: string regs = context.dst;
                std::string res = context.makeName("A");
                std::string mem = id->getId() + "0";
                index->EmitRISC(stream, context);
                std::string index_ = context.bindings[context.dst];
                std::string reg = context.dst;
                stream << "slli " << index_ << "," << index_ << ",2" << std::endl; // since each word is 4
                stream << "neg " << index_ << "," << index_ << std::endl;
                stream << "add " << index_ << "," << index_ << ",sp" << std::endl;
                stream << "sw " << context.bindings[regs] << "," << context.MemoryMapping[mem] << "(" << index_ << ")" << std::endl;
                context.DeallocReg(regs);
                context.DeallocReg(reg);
                context.dst = res;
        }
        else
        {
            if (index->getType() == "constant")
            {
                std::string bind = context.makeName("A");
                std::string reg = context.AllocReg(bind);
                std::string mem = id->getId() + std::to_string(index->getVal());
                stream << "lw " << reg << "," << context.MemoryMapping[mem] << "(sp)" << std::endl;
                context.dst = bind;
            }
            else
            {
                std::string res = context.makeName("A");
                std::string regs = context.AllocReg(res);
                std::string mem = id->getId() + "0";
                index->EmitRISC(stream, context);
                std::string index_ = context.bindings[context.dst];
                std::string reg = context.dst;
                stream << "slli " << index_ << "," << index_ << ",2" << std::endl; // since each word is 4
                stream << "neg " << index_ << "," << index_ << std::endl;
                stream << "add " << index_ << "," << index_ << ",sp" << std::endl;
                stream << "lw " << regs << "," << context.MemoryMapping[mem] << "(" << index_ << ")" << std::endl;
                context.DeallocReg(reg);
                context.dst = res;
            }
        }
    };
};

#endif
