#ifndef AST_CALL_HPP
#define AST_CALL_HPP

#include "../ast_node.hpp"

class Enumerator {
    protected:
        Node*  id;
        NodeList* enumerators;
    public:
        Enumerator(Node* id_, NodeList* enumerators_): id(id_), enumerators(enumerators_){};
        ~Enumerator(){
            delete id;
            delete enumerators;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{
            for (const auto& pair : context.enums) {
                    stream << pair.first << ":" << std::endl;
                    stream << ".word " << pair.second << std::endl;
                }
                context.enums.clear();
        }

        virtual void Print(std::ostream &stream) const override{};
};


class EnumDeclerator: public Node {
    protected:
        Node* id;
        Node* value;
    public:
        EnumDeclerator(Node* id_, Node* value_): id(id_), value(value_){};
        ~EnumDeclerator(){
            delete id;
            delete value;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{
            context.enums[id->getId()] = value->getValue();
        }

        virtual void Print(std::ostream &stream) const override{};

};

#endif
