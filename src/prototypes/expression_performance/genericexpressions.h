
namespace GENERIC {

// concrete classes
template <class E1, class E2>
class AddExpression
{
public:
    inline double value() {
        return a.value() + b.value();
    }

    E1 a;
    E2 b;
};

template <class E1, class E2>
class MultiplyExpression
{
public:
    inline double value() {
        return a.value() * b.value();
    }

    E1 a;
    E2 b;
};

class ConstExpression
{
public:
    inline double value() {
        return val;
    }

    double val;
};

}
