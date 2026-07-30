#include"User.hpp"
#include<string>

User ::User(string u, int c)
{
    username = u;
    credit = c;
    lock_credit = 0;
}

string User ::get_user_name() const
{
    return username;
}

void User ::addShare(const string &companyName, int count)
{
    shares[companyName] = {count, 0};
}