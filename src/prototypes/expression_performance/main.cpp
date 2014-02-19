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
 * This prototype contains potentially dirty code, don't take it as an example
 * for good code.
 */

#include <iostream>
#include <ctime>

using namespace std;

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

int loop_times = 2000;

double _calc_expr(Expression* e, ConstExpression*, time_t* start_time, time_t* end_time);

// calcs the same as calc_plain, but uses expressions
double calc_expr(double a, double b, time_t* start_time, time_t* end_time) {
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

    return _calc_expr(&m1, &c2, start_time, end_time);
}

double _calc_expr(Expression* e, ConstExpression* c2, time_t* start_time, time_t* end_time) {
    double sum = 0.0;

    time(start_time);
    for (c2->val = 0.2; c2->val<loop_times; c2->val++) {
        sum += e->value();
    }
    time(end_time);

    return sum;
}

double calc_plain(double c314, double c2, time_t* start_time, time_t* end_time) {
    double sum = 0.0;

    time(start_time);
    for (double i=0.2; i<loop_times; i++) {
        sum += ((i * c314) + (c314 + i)) * i;
    }
    time(end_time);

    return sum;
}

int main(int argc, char** argv) {
    double c314 = 3.14;
    double c2 = 2.0;

    time_t start_time;
    time_t end_time;
    double sum_expr = calc_expr(c314, c2, &start_time, &end_time);

    time_t start_time2;
    time_t end_time2;
    double sum_plain = calc_plain(c314, c2, &start_time2, &end_time2);


    if (sum_expr != sum_plain) {
        cerr << "wtf" << endl;
        return 1;
    }

    time_t expr_time = end_time - start_time;
    time_t plain_time = end_time2 - start_time2;
    time_t overhead_time = expr_time - plain_time;
    cout << "Time with expressions: " << expr_time << endl
        << "Time with plain calc: " << plain_time << endl
        << "Overhead time: " << overhead_time << endl
        << "Overhead: " << overhead_time / (double) plain_time  * 100.0 << "%" << endl;

    return 0;
}

