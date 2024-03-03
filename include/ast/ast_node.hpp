#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <iostream>
#include <vector>

#include "ast_context.hpp"

class Node
{

public:
    Node(){};
    virtual void EmitRISC(std::ostream &stream, Context &context) const = 0;
    virtual void Print(std::ostream &stream) const = 0;
    virtual ~Node(){};

    // helper fuction for child functions
    virtual std::string getType() const {};
    virtual std::string getId() const {};
    virtual int getSize() const {};
    virtual int getVal() const{};
};

// Represents a list of nodes.
class NodeList : public Node
{
private:
    std::vector<Node *> nodes_;

public:
    NodeList(Node *first_node) : nodes_({first_node}){};
    NodeList(){};
    ~NodeList()
    {
        for (auto node : nodes_)
        {
            delete node;
        }
    }
    virtual void Print(std::ostream &stream) const override{};

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        for (int i = 0; i < nodes_.size(); i++)
        {
            if (nodes_[i]== nullptr)
            {
                continue;
            }
            nodes_[i]->EmitRISC(stream, context);
        }
    }
    void PushBack(Node *item)
    {
        nodes_.push_back(item);
    }
};

#endif
