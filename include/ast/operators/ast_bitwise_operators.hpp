#ifndef AST_BITWISEOP_HPP
#define AST_BITWISEOP_HPP

#include "../ast_node.hpp"


class BitwiseAndOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    BitwiseAndOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~BitwiseAndOp()
    {
        delete leftOperand_;
        delete rightOperand_;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() & rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            leftOperand_->EmitRISC(stream, context);
            std:: string left = context.dst;
            rightOperand_->EmitRISC(stream, context);
            std:: string right= context.dst;
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "and " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};


class BitwiseOrOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    BitwiseOrOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~BitwiseOrOp()
    {
        delete leftOperand_;
        delete rightOperand_;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() | rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            leftOperand_->EmitRISC(stream, context);
            std:: string left = context.dst;
            rightOperand_->EmitRISC(stream, context);
            std:: string right= context.dst;
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "or " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};



class BitwiseXorOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    BitwiseXorOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~BitwiseXorOp()
    {
        delete leftOperand_;
        delete rightOperand_;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() ^ rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            leftOperand_->EmitRISC(stream, context);
            std:: string left = context.dst;
            rightOperand_->EmitRISC(stream, context);
            std:: string right= context.dst;
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "xor " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};



class ShiftLeftOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    ShiftLeftOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~ShiftLeftOp()
    {
        delete leftOperand_;
        delete rightOperand_;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() << rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            leftOperand_->EmitRISC(stream, context);
            std:: string left = context.dst;
            rightOperand_->EmitRISC(stream, context);
            std:: string right= context.dst;
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "sll " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};

class ShiftRightOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    ShiftRightOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~ShiftRightOp()
    {
        delete leftOperand_;
        delete rightOperand_;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() >> rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            leftOperand_->EmitRISC(stream, context);
            std:: string left = context.dst;
            rightOperand_->EmitRISC(stream, context);
            std:: string right= context.dst;
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "sra " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};

#endif
