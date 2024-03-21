#ifndef AST_ARITHMETICOP_HPP
#define AST_ARITHMETICOP_HPP

#include "../ast_node.hpp"

class AddOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    AddOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~AddOp()
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
            int result = leftOperand_->getVal() + rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            std::string left;
            std::string right;
            if (rightOperand_->getType() == "call")
            {
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
            }
            else
            {
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
            }

            if (left[1] == 'F' && right[1] == 'F')
            {
                std::string op = context.makeName("F");
                std::string res = context.AllocReg(op);
                stream << "fadd.s " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }
            else if (left[1] == 'D' && right[1] == 'D'){
                std:: string op = context.makeName("D");
                std:: string res = context.AllocReg(op);
                stream << "fadd.d " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }
            else if (left[1] == 'P' || right[1] == 'P')
            {
                if (left[1] == 'P')
                {
                    std::string op = context.makeName("P");
                    std::string res = context.AllocReg(op);
                    stream << "slli " << context.bindings[right] << "," << context.bindings[right] << ",2" << std::endl;
                    stream << "add " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                    context.DeallocReg(left);
                    context.DeallocReg(right);
                    context.dst = op;
                }
                else{
                    std::string op = context.makeName("P");
                    std::string res = context.AllocReg(op);
                    stream << "slli " << context.bindings[left] << "," << context.bindings[left] << ",2" << std::endl;
                    stream << "add " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                    context.DeallocReg(left);
                    context.DeallocReg(right);
                    context.dst = op;
                }
            }
            else
            {
                std::string op = context.makeName("O");
                std::string res = context.AllocReg(op);
                stream << "add " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }

        }
    }
};

class SubOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    SubOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~SubOp()
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
            int result = leftOperand_->getVal() - rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            std::string left;
            std::string right;
            if (rightOperand_->getType() == "call")
            {
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
            }
            else
            {
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
            }
            if (left[1] == 'F' && right[1] == 'F')
            {
                std::string op = context.makeName("F");
                std::string res = context.AllocReg(op);
                stream << "fsub.s " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }
            else if (left[1] == 'D' && right[1] == 'D'){
                std::string op = context.makeName("F");
                std::string res = context.AllocReg(op);
                stream << "fsub.d " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }
            else if (left[1] == 'P' || right[1] == 'P')
            {
                if (left[1] == 'P')
                {
                    std::string op = context.makeName("P");
                    std::string res = context.AllocReg(op);
                    stream << "slli " << context.bindings[right] << "," << context.bindings[right] << ",2" << std::endl;
                    stream << "sub " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                    context.DeallocReg(left);
                    context.DeallocReg(right);
                    context.dst = op;
                }
                else{
                    std::string op = context.makeName("P");
                    std::string res = context.AllocReg(op);
                    stream << "slli " << context.bindings[left] << "," << context.bindings[left] << ",2" << std::endl;
                    stream << "sub " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                    context.DeallocReg(left);
                    context.DeallocReg(right);
                    context.dst = op;
                }
            }
            else
            {
            std::string op = context.makeName("O");
            std::string res = context.AllocReg(op);
            stream << "sub " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
            }
        }
    }
};

class MulOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    MulOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~MulOp()
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
            int result = leftOperand_->getVal() * rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            std::string left;
            std::string right;
            if (rightOperand_->getType() == "call")
            {
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
            }
            else
            {
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
            }
            if (left[1] == 'F' && right[1] == 'F')
            {
                std::string op = context.makeName("F");
                std::string res = context.AllocReg(op);
                stream << "fmul.s " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }
            else if (left[1] == 'D' && right[1] == 'D'){
                std::string op = context.makeName("D");
                std::string res = context.AllocReg(op);
                stream << "fmul.d " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }
            else
            {
                std::string op = context.makeName("O");
                std::string res = context.AllocReg(op);
                stream << "mul " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }
        }
    }
};

class DivOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    DivOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~DivOp()
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
            int result = leftOperand_->getVal() / rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            std::string left;
            std::string right;
            if (rightOperand_->getType() == "call")
            {
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
            }
            else
            {
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
            }
            if (left[1] == 'F' && right[1] == 'F')
            {
                std::string op = context.makeName("F");
                std::string res = context.AllocReg(op);
                stream << "fdiv.s " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }
            else if (left[1] == 'F' && right[1] == 'F'){
                std::string op = context.makeName("D");
                std::string res = context.AllocReg(op);
                stream << "fdiv.d " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
                context.DeallocReg(left);
                context.DeallocReg(right);
                context.dst = op;
            }
            else
            {
            std::string op = context.makeName("O");
            std::string res = context.AllocReg(op);
            stream << "div " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
            }
        }
    }
};

class ModOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    ModOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~ModOp()
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
            int result = leftOperand_->getVal() % rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            std::string left;
            std::string right;
            if (rightOperand_->getType() == "call")
            {
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
            }
            else
            {
                leftOperand_->EmitRISC(stream, context);
                left = context.dst;
                rightOperand_->EmitRISC(stream, context);
                right = context.dst;
            }
            std::string op = context.makeName("O");
            std::string res = context.AllocReg(op);
            stream << "rem " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }
};

#endif
