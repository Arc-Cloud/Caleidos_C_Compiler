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

    std:: string getId() const override{
        return id -> getId();
    }

    std:: string getType() const override{
        return "array";
    }

    int getSize() const override{
        return index -> getVal();
    }
    virtual void Print(std::ostream &stream) const override{};

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (index -> getType() == "constant"){
            std:: string bind = context.makeName("A");
            std:: string reg = context.AllocReg(bind);
            std:: string mem = id ->getId() + std:: to_string(index->getVal());
            stream << "lw " << reg << "," << context.MemoryMapping[mem] << "(sp)" << std::endl;
            context.dst = bind;
        }
    };
};

#endif
