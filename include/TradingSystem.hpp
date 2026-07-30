#pragma once

#include"Company.hpp"
#include"User.hpp"
#include"Order.hpp"
#include<string>
#include<vector>
#include<algorithm>
#include<iostream>

using namespace std;

class TradingSystem
{
private:
    vector<User> users;
    vector<Company> companies;
    vector<Order> buy_queue;
    vector<Order> sell_queue;
    int next_order_id = 1;

public:
    void loadCompanies(const string &filePath);
    void loadShareholders(const string &filePath);
    bool find_user(const string &username) const;
    void add_user(const string &username, int credit);
    void add_user_error(const string &username) const;
    User *find_address_of_user(const string &username);
    const Company *find_address_of_company(const string &company_name) const;
    bool has_opposite_order(const string &name, const string &company, bool looking_for_buy) const;
    void update_momentary_price(int trans_price, const string &company);
    void handle_sell(const string &nusername, int count, const string &company, int price);
    void handle_buy(const string &nusername, int count, const string &company, int price);
    void cancel_order(int id);
    void cancel_buy_order(vector<Order>::iterator it);
    void cancel_sell_order(vector<Order>::iterator it);
    int get_company_price(const string &company_name) const;
    long long calculate_total_free_assets(const User &user) const;
    void print_portfolio_header(const User &user, long long total_free_assets) const;
    void print_free_shares(const User &user) const;
    void print_locked_shares(const User &user) const;
    void report_portfolio(const string &username);
    vector<Order> get_company_sell_orders(const string &company_name) const;
    vector<Order> get_company_buy_orders(const string &company_name) const;
    void print_order_queue(const vector<Order> &orders) const;
    void report_company(const string &company_name);
};