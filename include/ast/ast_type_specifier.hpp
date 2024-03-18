#ifndef AST_TYPE_SPECIFIER
#define AST_TYPE_SPECIFIER

#include "ast_node.hpp"

enum _Types
{
    _int,
    _float
};
class TypeSpecifier : public Node
{
private:
    _Types id;

public:
    TypeSpecifier(_Types TypeId) : id(TypeId){};
    ~TypeSpecifier(){};
    void EmitRISC(std::ostream &stream, Context &context) const override{};
    void Print(std::ostream &stream) const override{};
    std::string getType() const override
    {
        switch (id)
        {
        case _int:
            return "int";
            break;
        case _float:
            return "float";
            break;

        default:
            std::cerr << "data Type not implemented" << std::endl;
            break;
        }
    };
    int getSize() const override
    {
        switch (id)
        {
        case _int:
            return 1; //(requires 4 bytes or "1" word)
            break;
        case _float:
            return 1; // 4 bytes
            break;

        default:
            std::cerr << "data Type not implemented" << std::endl;
            break;
        }
    }
};

#endif
