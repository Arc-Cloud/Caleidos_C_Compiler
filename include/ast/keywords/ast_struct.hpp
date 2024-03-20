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

            structs->EmitRISC(stream,context);

        }

        virtual void Print(std::ostream &stream) const override{};

        std::string getType() const override{
        return "struct";
        }


};


class StructMemberDeclaration: public Node {
    protected:
        Node* typeSpecifier;
        NodeList* declarators;

    public:
        StructMemberDeclaration(Node* typeSpecifier_, NodeList* declarators_): typeSpecifier(typeSpecifier_), declarators(declarators_){};
        ~StructMemberDeclaration(){
            delete typeSpecifier;
            delete declarators;
        }

        virtual void EmitRISC(std::ostream &stream, Context &context) const override{

            for (auto& declarator : *declarators) {

                std::string name = declarator->getId();
                std::string type = typeSpecifier->getType();

                context.structMap[name] = std::pair(type, context.CurrentOffset);

                context.CurrentOffset += 4;
            }


        }

        virtual void Print(std::ostream &stream) const override{};

};

class StructMemberAccess : public Node {
protected:
    Node* struct_instance;
    std::string name;

public:
    StructMemberAccess(Node* struct_instance_, const std::string& name_): struct_instance(struct_instance_), name(name_){}

    virtual void EmitRISC(std::ostream &stream, Context &context) const override {
        // Assuming `instance` is an expression that evaluates to a struct instance,
        // and `member` is the name of the struct member being accessed.

        // Calculate the base address of the struct instance into a register
        instance->EmitRISC(stream, context);

        // Retrieve the offset and type of the member from the context's structMap
        auto memberInfo = context.structMap[member];
        std::string type = memberInfo.first;
        int offset = memberInfo.second;

        // Generate the code to access the struct member using the base address and offset
        // This is highly dependent on the architecture and the type of the member.
        // Here is a placeholder for illustrative purposes:
        stream << "// Load the address of the struct instance into a register (done in instance->EmitRISC)\n";
        stream << "// Add the offset of the member (" << offset << ") to the base address\n";
        stream << "// Depending on the type (" << type << "), use the appropriate load instruction\n";
    }

    virtual void Print(std::ostream &stream) const override {
        // Implement as needed for debugging
    }
};


#endif
