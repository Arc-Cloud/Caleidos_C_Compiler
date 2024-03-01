#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast_node.hpp"

// the declaration of a variable in the code should allocate memory space onto the memory, we do it manually for now
class declaration : public Node {
    private:
    Node* type_spec;
    Node* identifier_; // might be changed to vector if we are dealing with multiple declaration at the same time
    public:
    declaration(Node* type, Node* identifier) : type_spec(type), identifier_(identifier) {};
    ~declaration(){
        delete type_spec;
        delete identifier_;
    };
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
