#ifndef AST_ASSIGN_HPP
#define AST_ASSIGN_HPP

#include "../ast_node.hpp"

class SimpleAssignment : public Node{
    private:
    Node* _expr;
    Node* _val;
    public:
    SimpleAssignment(Node* expr, Node* val) : _expr(expr), _val(val){};
    ~SimpleAssignment(){
        delete _expr;
        delete _val;
    }
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
