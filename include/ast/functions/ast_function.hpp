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

        std:: string getType()const override{
            return "function";
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{
                context.newFrame(context);
                if (compound_statement_ != NULL){
                    context.is_global = false;
                }
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
                for (int i = 0; i < context.savedCounter.size(); i++){
                    std:: string reg = "s" + std::to_string(context.savedCounter[i]); // this is a reall stupid implementation i know
                    stream << "lw " << reg << "," << context.MemoryMapping[reg] << "(sp)" << std::endl;
                }
                stream << "lw ra," << std:: to_string(context.MemoryMapping["ra"]) << "(sp)" << std::endl;
                stream << "addi sp,sp," << context.default_mem << std::endl;

                stream << "jr ra" << std::endl;

                for (const auto& pair : context.FloatWords) {
                    stream << pair.first << ":" << std::endl;
                    stream << ".word " << pair.second << std::endl;
                }
                context.FloatWords.clear();
                for (const auto& [label, parts] : context.DoubleWords) {
                    stream << label << ":" << std::endl;
                    stream << ".word   " << parts.second << std::endl;
                    stream << ".word   " << parts.first << std::endl;
                }
                context.DoubleWords.clear();
                for (const auto& [label, parts]: context.StringsHolder){
                    stream << label << ":" << std::endl;
                    stream <<".string " << parts << std::endl; 
                }
                context.StringsHolder.clear();
                context.ExitFrame(context);
        }

        virtual void Print(std::ostream &stream) const override{};
};

class CompoundStat : public Node
{
private:
    Node *declarations;
    Node *statements;

public:
    CompoundStat(Node *declare, Node *state) : declarations(declare), statements(state){};
    virtual ~CompoundStat()
    {
        delete declarations;
        delete statements;
    }

    void Print(std::ostream &stream) const override{};
    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        context.WriteInstType(" ");
        if (declarations != NULL)
        {
            // int decnum = declarations ->getSize(); this code can be exploited for memory management but im lazy now
            context.inFunc = true;
            declarations->EmitRISC(stream, context);
            context.inFunc = false;
        }

        if (statements != NULL)
        {
            statements->EmitRISC(stream, context);
        }

        if (context.scopeflag){
            context.scopeflag = false;
            int count = context.scopecount - 1;
            context.MemoryMapping[context.varscope[count]] = context.scope[count];
            //this is so fucked lol but it works
        }
    }
};

#endif
