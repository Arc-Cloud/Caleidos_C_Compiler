#include "../include/ast/functions/ast_translation.hpp"
#include <vector>


void Translation:: EmitRISC(std::ostream &stream, Context &context) const{
    stream << ".text" << std:: endl;
    stream << ".globl f" << std:: endl;

    for (auto node : functions_){
        if(node == nullptr){
            continue;
        }
        node ->EmitRISC(stream,context);
    }
}

void Translation::PushBack(Node* item){
    functions_.push_back(item);
}

void Translation::Print(std::ostream &stream) const {
      for (auto node : functions_)
    {
        if (node == nullptr)
        {
            continue;
        }
        node->Print(stream);
    }
}

