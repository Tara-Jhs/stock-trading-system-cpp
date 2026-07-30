#pragma once 

#include<string>
#include<iostream>

using namespace std;

const string REGISTER = "register";
const string SELL_ORDER = "sell_order";
const string BUY_ORDER = "buy_order";
const string CANCEL_ORDER = "cancel_order";
const string REPORT_PORTFOLIO = "report_portfolio";
const string REPORT_COMPANY = "report_company";

struct Order
{
    int id = -1;
    string username;
    string company;
    int count;
    int price;
    bool is_buy;
};