#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP


#include "../ast_node.hpp"

class Function: public Node {
    protected:
        Node* type;
        Node* id;
        NodeList* parameters;
        Node* compound_statement_;
    public:
        Function(Node* typespec, Node* name, NodeList* params, Node* statement): type(typespec), id(name), parameters(params), compound_statement_(statement){};
        ~Function(){
            delete type;
            delete compound_statement_;
            delete parameters;
            delete id;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{
                id ->EmitRISC(stream, context);
                stream << "addi sp,sp,-" << context.memDef() << std::endl;
                stream << "sw ra," << std::to_string(context.AllocateStack("ra")) <<"(sp)" << std::endl;
                stream <<  "sw s0," << std::to_string(context.AllocateStack("s0")) <<"(sp)" << std:: endl;
                stream << "addi s0,sp," << context.default_mem << std:: endl;
                if (compound_statement_ != nullptr){
                compound_statement_ -> EmitRISC(stream,context);
                }
                stream << "lw s0,"<< std::to_string(context.MemoryMapping["s0"])<< "(sp)" << std:: endl;
                stream << "lw ra," << std:: to_string(context.MemoryMapping["ra"]) << "(sp)" << std::endl;
                stream << "addi sp,sp," << context.default_mem << std::endl;

                stream << "ret" << std::endl;

        }

        virtual void Print(std::ostream &stream) const override{};
};

class CompoundStat : public Node {
    private:
    Node* declarations;
    Node* statements;
    public:
    CompoundStat(Node* declare, Node* state): declarations(declare), statements(state){};
    virtual ~CompoundStat(){
        delete declarations;
        delete statements;
    }

    void Print(std::ostream &stream) const override{};
    virtual void EmitRISC(std::ostream &stream, Context &context) const override{
            if (declarations != NULL){
                // int decnum = declarations ->getSize(); this code can be exploited for memory management but im lazy now
                declarations -> EmitRISC(stream, context);
            }

            if (statements != NULL){
                statements -> EmitRISC(stream, context);
            }

    }
};

#endif
