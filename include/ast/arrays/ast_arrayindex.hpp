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
            std::string regs = context.dst;
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
            if (context.pointerlist.count(id->getId()))
            {
                if (context.StringVar.count(id->getId()))
                {
                    id->EmitRISC(stream, context);
                    std::string reg = context.dst;
                    // stream << "lw " << context.bindings[reg] << "," << index->getVal() << "(" << context.bindings[reg] << ")" << std::endl;
                    stream << "addi " << context.bindings[reg] << "," << context.bindings[reg] << "," << index -> getVal() << std::endl;
                    stream << "lbu " << context.bindings[reg] << ",0(" << context.bindings[reg] << ")" << std::endl;
                    context.dst = reg; 
                }
                else
                {
                    if (index->getType() == "constant")
                    {
                        id->EmitRISC(stream, context);
                        std::string reg = context.dst;
                        stream << "lw " << context.bindings[reg] << "," << (index->getVal() * 4) << "(" << context.bindings[reg] << ")" << std::endl;
                        context.dst = reg;
                    }
                    else
                    {
                        id->EmitRISC(stream, context);
                        std::string _id = context.dst;
                        std::string reg1 = context.bindings[_id];
                        index->EmitRISC(stream, context);
                        std::string _index = context.dst;
                        std::string reg2 = context.bindings[_index];
                        std::string res = context.makeName("PA");
                        stream << "slli " << reg2 << "," << reg2 << ",2" << std::endl;
                        stream << "add " << reg1 << "," << reg1 << "," << reg2 << std::endl;
                        context.DeallocReg(_index);
                        std::string reg = context.AllocReg(res);
                        stream << "lw " << reg << ",0(" << reg1 << ")" << std::endl;
                        context.DeallocReg(_id);
                        context.dst = res;
                    }
                }
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
        }
    };
};

#endif
