#pragma once

#include<string>
#include<iostream>

using namespace std;

class Company
{
private:
    string name;
    int first_price;
    int momentary_price;

public:
    Company(string n, int p);
    string get_name() const;
    int get_price() const;
    void set_price(int p);
};
