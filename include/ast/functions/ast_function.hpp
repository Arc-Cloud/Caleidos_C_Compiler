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
                stream << ".globl " << id << std::endl;
                stream << id << ":" << std::endl;
                stream << "addi sp,sp," << (-context.default_mem) << std::endl;
                stream << "sw ra," << std::to_string(context.AllocateStack()) <<"(sp)" << std::endl;
                stream <<  "sw s0," << std::to_string(context.AllocateStack()) <<"(sp)" << std:: endl;
                stream << "addi s0,sp," << context.default_mem << std:: endl;
                if (compound_statement_ != nullptr){
                compound_statement_ -> EmitRISC(stream,context);
                }
                context.memDealloc();
                stream << "lw s0,"<< std::to_string(context.DeallocStack())<< "(sp)" << std:: endl;
                stream << "lw ra," << std:: to_string(context.DeallocStack()) << "(sp)" << std::endl;
                stream << "addi sp,sp," << context.default_mem << std::endl;

                stream << "ret" << std::endl;

        }

        virtual void Print(std::ostream &stream) const override{};
};

#endif
