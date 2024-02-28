#ifndef AST_ifelse_HPP
#define AST_ifelse_HPP

#include "../ast_node.hpp"


class ifNoElse : public Node{
    private:
    Node* expression_;
    Node* statement_;
    public:
    ifNoElse(Node *expression, Node * statement) : expression_(expression), statement_(statement){};
    ~ifNoElse(){
        delete expression_;
        delete statement_;
    }

    void EmitRISC(std::ostream &stream, Context &context) const override;
    // void Print(std::ostream &stream) const override;
};


class ifElse : public Node{
    private:
    Node* expression_;
    Node* statement_;
    Node* alternative_;
    public:
    ifElse ( Node* expression, Node* statement1, Node* statement2): expression_(expression), statement_(statement1), alternative_(statement2){};
    ~ifElse(){
        delete expression_;
        delete statement_;
        delete alternative_;
    }

    void EmitRISC(std::ostream &stream, Context &context) const override;
};


#endif