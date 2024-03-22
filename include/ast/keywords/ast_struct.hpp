#ifndef AST_STRUCT_HPP
#define AST_STRUCT_HPP

#include "../ast_node.hpp"

class DefineSpecial: public Node{
    private:
    std:: string id;
    Node* statement;
    public:
    DefineSpecial(std:: string _id, Node* _statement): id(_id), statement(_statement){};
    virtual ~DefineSpecial(){
        delete statement;
    }

    std:: string getType()const override{
            return "struct";
    }


    virtual void EmitRISC(std::ostream &stream, Context &context) const override{
        statement -> EmitRISC(stream, context);
        context.StructMapping[id] = context.StructMem;
    }

    virtual void Print(std::ostream &stream) const override{};
};



class StructDeclare: public Node{
    private:
    Node* Typespec;
    Node* init;
    public:
    StructDeclare(Node*_typespec, Node* _init): Typespec(_typespec), init(_init){};
    virtual ~StructDeclare(){
        delete Typespec;
        delete init;
    }

    std:: string getType()const override{
            return "struct";
    }


    virtual void EmitRISC(std::ostream &stream, Context &context) const override{
        std:: string var = init -> getId();
        int datatype = Typespec->getSize();
        context.structsize += datatype * 4;
        context.StructMem[var] = context.structsize;
    }

    virtual void Print(std::ostream &stream) const override{};

};

class Structinit: public Node{
    private:
    std:: string id;
    public:
    Structinit(std:: string _id): id(_id){};
    virtual ~Structinit(){}
    std:: string getType()const override{
        return "struct";
    }
    std:: string getId() const override{
        return id; 
    }


    virtual void EmitRISC(std::ostream &stream, Context &context) const override{
    }

    virtual void Print(std::ostream &stream) const override{};

};

class StructExpression: public Node{
    private:
    Node* StructId;
    std:: string id;
    public:
    StructExpression(Node* _StructId, std:: string _id): StructId(_StructId), id(_id){};
    virtual ~StructExpression(){
        delete StructId;
    }
    std:: string getType()const override{
        return "struct";
    }
    std:: string getId() const override{
        return id;
    }

    // this is so fucked but it works
    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (context.ReadInstType() == "assign")
        {   
            std::map<std::string, int> mem = context.StructMapping[context.StructMap[StructId->getId()]];
            stream << "sw " << context.bindings[context.dst] << "," << mem[id] << "(sp)" << std::endl;
            context.DeallocReg(context.dst);
        }
        else{
            std:: string res = context.makeName("Z");
            std:: string reg = context.AllocReg(res);
            std::map<std::string, int> mem = context.StructMapping[context.StructMap[StructId->getId()]];
            stream << "lw " << reg << "," << mem[id] << "(sp)" << std::endl;
            context.dst = res;
        }
    }

    virtual void Print(std::ostream &stream) const override{};

};


#endif
