#ifndef AST_UNARYOP_HPP
#define AST_UNARYOP_HPP

#include "../ast_node.hpp"


class UnaryMinusOp : public Node
{
private:
    Node *Operand;

public:
    UnaryMinusOp(Node *oper) : Operand(oper){};

    virtual ~UnaryMinusOp()
    {
        delete Operand;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (Operand->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = -Operand->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            Operand->EmitRISC(stream, context);
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "neg " << res << "," << context.bindings[context.dst] << std::endl;
            context.DeallocReg(context.dst);
            context.dst = op;
        }
    }

};



class UnaryBitwiseNotOp : public Node
{
private:
    Node *Operand;

public:
    UnaryBitwiseNotOp(Node *oper) : Operand(oper){};

    virtual ~UnaryBitwiseNotOp()
    {
        delete Operand;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (Operand->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = ~Operand->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            Operand->EmitRISC(stream, context);
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "not " << res << "," << context.bindings[context.dst] << std::endl;
            context.DeallocReg(context.dst);
            context.dst = op;
        }
    }

};


class UnaryLogicNotOp : public Node
{
private:
    Node *Operand;

public:
    UnaryLogicNotOp(Node *oper) : Operand(oper){};

    virtual ~UnaryLogicNotOp()
    {
        delete Operand;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (Operand->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = !Operand->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            Operand->EmitRISC(stream, context);
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "seqz " << res << "," << context.bindings[context.dst] << std::endl;
            context.DeallocReg(context.dst);
            context.dst = op;
        }
    }

};


class UnarySizeOfOp : public Node
{
private:
    Node *Operand;

public:
    UnarySizeOfOp(Node *oper) : Operand(oper){};

    virtual ~UnarySizeOfOp()
    {
        delete Operand;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {   

        if (context.datatype[Operand -> getId()] == "struct"){
            std::string resultReg = context.AllocReg("result");
            int result = context.structsize; // wouldnt work with more than two struct;
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else if(Operand->getType() == "variable"){
            std::string resultReg = context.AllocReg("result");
            int result = context.data_size(context.datatype[Operand->getId()]);
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else{
            std::string resultReg = context.AllocReg("result");
            int result = context.data_size(Operand->getType());
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }

    }

};



class UnaryIncrOp : public Node
{
private:
    Node *Operand;

public:
    UnaryIncrOp(Node *oper) : Operand(oper){};

    virtual ~UnaryIncrOp()
    {
        delete Operand;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        Operand->EmitRISC(stream, context);
        std:: string mid = context.dst;
        std:: string op = context.makeName("O");
        std:: string res = context.AllocReg(op);
        stream << "addi " << res << "," << context.bindings[mid] << "," << 1 << std::endl;
        stream << "sw " << res << "," << context.MemoryMapping[Operand->getId()] << "(sp)" << std:: endl;
        //context.DeallocReg(res);
        context.dst = mid;
    }

};




class UnaryDecrOp : public Node
{
private:
    Node *Operand;

public:
    UnaryDecrOp(Node *oper) : Operand(oper){};

    virtual ~UnaryDecrOp()
    {
        delete Operand;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        Operand->EmitRISC(stream, context);
        std:: string mid = context.dst;
        std:: string op = context.makeName("O");
        std:: string res = context.AllocReg(op);
        stream << "addi " << res << "," << context.bindings[mid] << "," << -1 << std::endl;
        stream << "sw " << res << "," << context.MemoryMapping[Operand->getId()] << "(sp)" << std:: endl;
        //context.DeallocReg(res);
        context.dst = mid;
    }

};



#endif
