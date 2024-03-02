#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP


#include "../ast_node.hpp"

class Function: public Node {
    protected:
        Node* type;
        std::string id;
        NodeList* parameters;
        Node* compound_statement_;
    public:
        Function(Node* typespec, std:: string name, NodeList* params, Node* statement): type(typespec), id(name), parameters(params), compound_statement_(statement){};
        ~Function(){
            delete type;
            delete compound_statement_;
            delete parameters;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{
            if (compound_statement_ != nullptr){
                stream << ".globl " << id << std::endl;
                stream << id << ":" << std::endl;

                compound_statement_ -> EmitRISC(stream,context);

                stream << "ret" << std::endl;
            }
        }

        virtual void Print(std::ostream &stream) const override{};
};

#endif
