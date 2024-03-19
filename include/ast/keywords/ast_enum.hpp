#ifndef AST_ENUM_HPP
#define AST_ENUM_HPP

#include "../ast_node.hpp"

class EnumDeclerator: public Node {
    protected:
        Node*  id;
        NodeList* enumerators;
    public:
        EnumDeclerator(Node* id_, NodeList* enumerators_): id(id_), enumerators(enumerators_){};
        ~EnumDeclerator(){
            delete id;
            delete enumerators;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{

            enumerators->EmitRISC(stream,context);
            context.enumcounter = 0;

        }

        virtual void Print(std::ostream &stream) const override{};

        std::string getType() const override{
        return "enumerator";
        }


};


class Enumerator: public Node {
    protected:
        Node* id;
        Node* value;
    public:
        Enumerator(Node* id_, Node* value_): id(id_), value(value_){};
        ~Enumerator(){
            delete id;
            delete value;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{
            //value->EmitRISC(stream,context); need to implement to handle operations e.g a = 4 * 5
            //std::string val = context.dst;
            if (value!=NULL){
            context.enums[id->getId()] = value->getVal();
            }
            else{
                context.enums[id->getId()] = context.enumcounter++;
            }
        }

        virtual void Print(std::ostream &stream) const override{};

};

#endif
