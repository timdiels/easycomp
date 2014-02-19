// Note: can't inline virtual functions unless exact type is known at compile time (which with our expressions is not the case)

namespace OO {

class Expression
{
public:
    virtual double value() = 0;
    virtual void unused() {} // probably not needed, but wouldn't want to take the risk

    Expression* a;
    Expression* b;
};

// concrete classes
class AddExpression : public Expression
{
public:
    virtual double value() {
        return a->value() + b->value();
    }
};

class MultiplyExpression : public Expression
{
public:
    virtual double value() {
        return a->value() * b->value();
    }
};

class ConstExpression : public Expression
{
public:
    virtual double value() {
        return val;
    }

    double val;
};

}
