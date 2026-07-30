#include"TradingSystem.hpp"
#include<iostream>
#include<string>

void run_program(TradingSystem &system, string command)
{
    if (command == REGISTER)
    {
        string username;
        int credit;
        cin >> username >> credit;
        if (system.find_user(username))
        {
            system.add_user_error(username);
        }
        else
        {
            system.add_user(username, credit);
        }
    }
    else if (command == SELL_ORDER)
    {
        string username, company;
        int count, price;
        cin >> username >> count >> company >> price;
        system.handle_sell(username, count, company, price);
    }
    else if (command == BUY_ORDER)
    {
        string username, company;
        int count, price;
        cin >> username >> count >> company >> price;
        system.handle_buy(username, count, company, price);
    }
    else if (command == CANCEL_ORDER)
    {
        int id;
        cin >> id;
        system.cancel_order(id);
    }
    else if (command == REPORT_PORTFOLIO)
    {
        string username;
        cin >> username;
        system.report_portfolio(username);
    }
    else if (command == REPORT_COMPANY)
    {
        string company;
        cin >> company;
        system.report_company(company);
    }
}