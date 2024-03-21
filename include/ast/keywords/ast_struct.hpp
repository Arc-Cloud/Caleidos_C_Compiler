#ifndef AST_STRUCT_HPP
#define AST_STRUCT_HPP

#include "../ast_node.hpp"

class StructSpec: public Node {
    protected:
        Node*  id;
        NodeList* structs;
    public:
        StructSpec(Node* id_, NodeList* structs_): id(id_), structs(structs_){};
        ~StructSpec(){
            delete id;
            delete structs;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{


            context.currentStructName = id->getId();
            structs->EmitRISC(stream,context);

        }

        virtual void Print(std::ostream &stream) const override{};

        std::string getType() const override{
        return "struc build";
        }


};


class StructBuildMap: public Node {
    protected:
        NodeList* typeSpecifiers;
        NodeList* declarators;

    public:
        StructBuildMap(NodeList* typeSpecifiers_, NodeList* declarators_): typeSpecifiers(typeSpecifiers_), declarators(declarators_){};
        ~StructBuildMap(){
            delete typeSpecifiers;
            delete declarators;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{


            std::string type;

            if (auto typeSpecNode = typeSpecifiers->getFirstNode()) {
                type = typeSpecNode->getType();
            }

            for (auto& declarator : *declarators) {
                std::string name = declarator->getId();
                context.structMap[context.currentStructName][name] = std::pair(type, context.CurrentOffset);
                context.CurrentOffset += 4;
                context.StructMem[name] = context.default_mem - context.CurrentOffset;
            }



        }

        virtual void Print(std::ostream &stream) const override{};

        std::string getType() const override{
        return "struct_map_build";
        }

};



class StructMemberAccess : public Node {
protected:
    Node* struct_instance;
    std::string name;

public:
    StructMemberAccess(Node* struct_instance_, std::string name_): struct_instance(struct_instance_), name(name_){}

    virtual void EmitRISC(std::ostream &stream, Context &context) const override {

        struct_instance->EmitRISC(stream, context);

        auto& memberInfo = context.structMap[struct_instance->getId()][name];
        std::string type = memberInfo.first;
        int offset = memberInfo.second;

        /*std:: string variable_ = context.makeName("V");
        context.AssignType(variable_, context.getDataType(name));
        std:: string res = context.AllocReg(variable_);
        stream << "lw " <<  res  << "," << context.StructMem[name] << "(sp)" << std::endl;
        context.dst = variable_;*/

    }

    virtual void Print(std::ostream &stream) const override {
    }

    std::string getType() const override{
        return "struct";
        }

    std::string getId() const override{
        return name;
        }
};


#endif
