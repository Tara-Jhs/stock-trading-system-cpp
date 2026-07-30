#pragma once

#include<string>
#include<map>
#include<iostream>

using namespace std;

class User
{
private:
    string username;
    long long credit;
    long long lock_credit;
    map<string, pair<int, int>> shares;

public:
    User(string u, int c);
    string get_user_name() const;
    void addShare(const string &companyName, int count);
    friend class TradingSystem;
};