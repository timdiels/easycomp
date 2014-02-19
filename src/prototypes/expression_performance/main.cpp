/*
 * Copyright (C) 2014 by Tim Diels
 *
 * This file is part of easycomp.
 *
 * easycomp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * easycomp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with easycomp.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * This prototype measures overhead of expressions
 *
 * This prototype contains dirty code, don't take it as an example
 * for good code.
 */

#include <iostream>
#include <ctime>

using namespace std;

#include "ooexpressions.h"
#include "genericexpressions.h"

///////////////////////////////////////////////////////
// OO expressions

namespace OO {

double _calc_expr(Expression* e);

// calcs the same as calc_plain, but uses oo expressions
double calc_expr(double a, double b) {
    ConstExpression c314;
    c314.val = a;

    ConstExpression c2;
    c2.val = b;

    MultiplyExpression m1;
    MultiplyExpression m2;
    AddExpression a1;
    AddExpression a2;

    m1.unused();

    // equivalent to m1(a1(m2(c2, c314), a2(c314, c2)), c2)
    m1.a = &a1;
    m1.b = &c2;
    m2.a = &c2;
    m2.b = &c314;
    a1.a = &m2;
    a1.b = &a2;
    a2.a = &c314;
    a2.b = &c2;

    return _calc_expr(&m1);
}

double _calc_expr(Expression* e) {
    return e->value();
}

}

///////////////////////////////////////////////////////
// generic expressions

namespace GENERIC {

template <class E>
double _calc_expr(E* e);

// calcs the same as calc_plain, but uses generic expressions
double calc_expr(double a, double b) {
    ConstExpression c314;
    c314.val = a;

    ConstExpression c2;
    c2.val = b;

    // Note: yes these types are crazy, I wonder how much stress that puts on the compiler. We can use make_expression to not have to type these ourselves by, analog to what make_pair is to std::pair
    MultiplyExpression<AddExpression<MultiplyExpression<ConstExpression, ConstExpression>, AddExpression<ConstExpression, ConstExpression>>, ConstExpression> m1;
    MultiplyExpression<ConstExpression, ConstExpression> m2;
    AddExpression<MultiplyExpression<ConstExpression, ConstExpression>, AddExpression<ConstExpression, ConstExpression>> a1;
    AddExpression<ConstExpression, ConstExpression> a2;

    // equivalent to m1(a1(m2(c2, c314), a2(c314, c2)), c2)
    a2.a = c314;
    a2.b = c2;
    m2.a = c2;
    m2.b = c314;
    a1.a = m2;
    a1.b = a2;
    m1.a = a1;
    m1.b = c2;

    return _calc_expr(&m1);
}

template <class E>
double _calc_expr(E* e) {
    return e->value();
}

}

///////////////////////////////////////////////////////
// plain C++ expression

double calc_plain(double c314, double c2) {
    return ((c2 * c314) + (c314 + c2)) * c2;
}

int main(int argc, char** argv) {
    double c314 = 3.14;
    double c2 = 2.0;

    double sum_oo_expr = OO::calc_expr(c314, c2);
    double sum_generic_expr = GENERIC::calc_expr(c314, c2);
    double sum_plain = calc_plain(c314, c2);

    if (sum_oo_expr != sum_generic_expr || sum_generic_expr != sum_plain) {
        cerr << sum_oo_expr << endl;
        cerr << sum_generic_expr << endl;
        cerr << sum_plain << endl;
        cerr << "wtf" << endl;
        cerr << "WRONG STUFF HAPPENED" << endl;
        return 1;
    }

    return 0;
}

