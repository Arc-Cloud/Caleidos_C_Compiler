#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP


#include "../ast_node.hpp"

class Function: public Node {
    protected:
        Node* type;
        Node* id;
        // NodeList* parameters;
        Node* compound_statement_;
    public:
        Function(Node* typespec, Node* name, Node* statement): type(typespec), id(name), compound_statement_(statement){};
        ~Function(){
            delete type;
            delete compound_statement_;
            // delete parameters;
            delete id;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{
                id ->EmitRISC(stream, context);
                std:: string end = context.makeName("end");
                context.EndLabel = end;
                if (compound_statement_ != nullptr){
                compound_statement_ -> EmitRISC(stream,context);
                }
                if (context.return_){
                stream << end << ":" << std::endl;
                }
                stream << "lw s0,"<< std::to_string(context.MemoryMapping["s0"])<< "(sp)" << std:: endl;
                stream << "lw ra," << std:: to_string(context.MemoryMapping["ra"]) << "(sp)" << std::endl;
                stream << "addi sp,sp," << context.default_mem << std::endl;

                stream << "ret" << std::endl;

                for (const auto& pair : context.FloatWords) {
                    stream << pair.first << ":" << std::endl;
                    stream << ".word " << pair.second << std::endl;
                }
                context.FloatWords.clear();
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
            context.WriteInstType(" ");
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
