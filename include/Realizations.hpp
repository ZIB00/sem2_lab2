#pragma once
#include "Interfaces.hpp"

class IntRing : public IRing<int> {
public:
    int Add(int a, int b) const override { return a + b; }
    int Zero() const override { return 0; }
    int Inverse(int a) const override { return -a; }
    
    int Multiply(int a, int b) const override { return a * b; }
    int One() const override { return 1; }
};

class DoubleRing : public IRing<double> {
public:
    double Add(double a, double b) const override { return a + b; }
    double Zero() const override { return 0.0; }
    double Inverse(double a) const override { return -a; }
    
    double Multiply(double a, double b) const override { return a * b; }
    double One() const override { return 1.0; }
};