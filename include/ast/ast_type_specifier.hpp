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
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
    std::string getType() const override;
};

#endif
