#ifndef AST_TYPE_SPECIFIER
#define AST_TYPE_SPECIFIER

#include "ast_node.hpp"

enum _Types{
    _int,
};
class TypeSpecifier : public Node
{
private:
    _Types id;
public:
    TypeSpecifier(_Types TypeId) : id(TypeId){};
    ~TypeSpecifier(){};
    virtual void EmitRISC(std::ostream &stream, Context &context) const override {};
    virtual void Print(std::ostream &stream) const override {};
    virtual std::string getType() const override{
        return "not implemented";
    };
    virtual int getSize() const override{
        switch (id)
        {
        case _int: return 4;
            break;

        default: std:: cerr << "data Type not implemented" << std::endl;
            break;
        }
    }
};

#endif
