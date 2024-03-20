#ifndef AST_RELATIONALOP_HPP
#define AST_RELATIONALOP_HPP

#include "../ast_node.hpp"


class EqualOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    EqualOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~EqualOp()
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
            int result = leftOperand_->getVal() == rightOperand_->getVal();
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
            stream << "sub " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            stream << "seqz " << res << "," << res << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};


class NotEqualOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    NotEqualOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~NotEqualOp()
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
            int result = leftOperand_->getVal() != rightOperand_->getVal();
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
            stream << "sub " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            stream << "snez " << res << "," << res << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};


class LessThanOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    LessThanOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~LessThanOp()
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
            int result = leftOperand_->getVal() < rightOperand_->getVal();
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
            stream << "slt " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            stream << context.bindings[res] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};


class LessThanEqualOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    LessThanEqualOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~LessThanEqualOp()
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
            int result = leftOperand_->getVal() <= rightOperand_->getVal();
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
            stream << "sgt " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            stream << "xori " << res << "," << res << "," << 1 << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};


class GreaterThanOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    GreaterThanOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~GreaterThanOp()
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
            int result = leftOperand_->getVal() > rightOperand_->getVal();
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
            stream << "sgt " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};


class GreaterThanEqualOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    GreaterThanEqualOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~GreaterThanEqualOp()
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
            int result = leftOperand_->getVal() >= rightOperand_->getVal();
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
            stream << "slt " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            stream << "xori " << res << "," << res << "," << 1 << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};


#endif
