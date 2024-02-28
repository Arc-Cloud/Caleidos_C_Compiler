#ifndef AST_TRANSLATION_HPP
#define AST_TRANSLATION_HPP
#include "../ast_node.hpp"

// this is the top most branch i think?
class Translation : public NodeList{
    private:
    std:: vector <Node*> functions_;
    public:
    Translation(Node *first_node) : functions_({first_node}) {};
    ~Translation(){
        for (auto node : functions_){
            delete node;
        }
    }

    void PushBack (Node* item);
    virtual void EmitRISC(std::ostream &stream, Context &context) const override;
    virtual void Print(std::ostream &stream) const override;
};

#endif
