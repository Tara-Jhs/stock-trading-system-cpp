#include"Company.hpp"
#include<string>

Company ::Company(string n, int p)
{
    name = n;
    first_price = p;
    momentary_price = p;
}

string Company ::get_name() const
{
    return name;
}

int Company ::get_price() const
{
    return momentary_price;
}

void Company ::set_price(int p)
{
    momentary_price = p;
}