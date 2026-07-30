#include "TradingSystem.hpp"
#include "Order.hpp"
#include <sstream>
#include <fstream>
#include <algorithm>

void TradingSystem ::loadCompanies(const string &filePath)
{
    ifstream file(filePath);
    string line, name, priceStr;
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty() || line == "\r")
            continue;

        stringstream ss(line);
        getline(ss, name, ',');
        getline(ss, priceStr, ',');

        companies.push_back(Company(name, stoi(priceStr)));
    }
    file.close();
}

void TradingSystem ::loadShareholders(const string &filePath)
{
    ifstream file(filePath);
    string line, username, creditStr, shareItem, compName, countStr;

    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty() || line == "\r")
            continue;

        stringstream ss(line);
        getline(ss, username, ',');
        getline(ss, creditStr, ',');

        User newUser(username, stoi(creditStr));
        while (getline(ss, shareItem, ';'))
        {
            stringstream shareStream(shareItem);
            getline(shareStream, compName, ':');
            getline(shareStream, countStr, ':');

            if (!compName.empty() && !countStr.empty())
            {
                newUser.addShare(compName, stoi(countStr));
            }
        }

        users.push_back(newUser);
    }
    file.close();
}

bool TradingSystem ::find_user(const string &username) const
{
    for (auto &u : users)
    {
        if (u.get_user_name() == username)
        {
            return true;
        }
    }
    return false;
}
void TradingSystem ::add_user_error(const string &username) const
{
    cout << username << " already exists." << endl;
}

void TradingSystem ::add_user(const string &username, int credit)
{
    users.push_back(User(username, credit));
    cout << username << " registered successfully." << endl;
}

User *TradingSystem ::find_address_of_user(const string &username)
{
    for (auto &u : users)
    {
        if (u.username == username)
        {
            return &u;
        }
    }
    return nullptr;
}

const Company *TradingSystem ::find_address_of_company(const string &company_name) const
{
    for (const auto &company : companies)
    {
        if (company.get_name() == company_name)
        {
            return &company;
        }
    }
    return nullptr;
}

bool TradingSystem::has_opposite_order(const string &name, const string &company, bool looking_for_buy) const
{
    const vector<Order> &queue = looking_for_buy ? buy_queue : sell_queue;
    for (const auto &q : queue)
    {
        if (q.username == name && q.company == company)
        {
            return true;
        }
    }
    return false;
}

void TradingSystem ::update_momentary_price(int trans_price, const string &company)
{
    for (auto &c : companies)
    {
        if (c.get_name() == company)
            c.set_price(trans_price);
    }
    return;
}

void TradingSystem::handle_sell(const string &username, int count, const string &company, int price)
{
    User *seller = find_address_of_user(username);

    if (has_opposite_order(username, company, true))
    {
        cout << username << " already has a buy order queued for " << company << "." << endl;
        return;
    }

    if (seller->shares[company].first < count)
    {
        cout << "Insufficient free shares." << endl;
        return;
    }

    int new_order_id = next_order_id++;

    sort(buy_queue.begin(), buy_queue.end(), [](const Order &a, const Order &b)
         { return a.id < b.id; });

    for (auto it = buy_queue.begin(); it != buy_queue.end(); ++it)
    {
        if (it->company == company && it->count == count && it->price == price)
        {
            User *buyer = find_address_of_user(it->username);
            long long amount = 1LL * count * price;

            seller->shares[company].first -= count;
            seller->credit += amount;

            buyer->lock_credit -= amount;
            buyer->shares[company].first += count;

            update_momentary_price(price, company);

            cout << "Order " << new_order_id << " matched with order " << it->id << "." << endl;

            buy_queue.erase(it);
            return;
        }
    }

    seller->shares[company].first -= count;
    seller->shares[company].second += count;

    sell_queue.push_back({new_order_id, username, company, count, price, false});

    cout << "Order " << new_order_id << " queued." << endl;
}

void TradingSystem::handle_buy(const string &username, int count, const string &company, int price)
{
    User *buyer = find_address_of_user(username);
    long long amount = 1LL * count * price;

    if (has_opposite_order(username, company, false))
    {
        cout << username << " already has a sell order queued for " << company << "." << endl;
        return;
    }

    if (buyer->credit < amount)
    {
        cout << "Insufficient free credit." << endl;
        return;
    }

    int new_order_id = next_order_id++;

    sort(sell_queue.begin(), sell_queue.end(), [](const Order &a, const Order &b)
         { return a.id < b.id; });

    for (auto it = sell_queue.begin(); it != sell_queue.end(); ++it)
    {
        if (it->company == company && it->count == count && it->price == price)
        {
            User *seller = find_address_of_user(it->username);

            buyer->credit -= amount;
            buyer->shares[company].first += count;

            seller->shares[company].second -= count;
            seller->credit += amount;

            update_momentary_price(price, company);

            cout << "Order " << new_order_id << " matched with order " << it->id << "." << endl;

            sell_queue.erase(it);
            return;
        }
    }

    buyer->credit -= amount;
    buyer->lock_credit += amount;

    buy_queue.push_back({new_order_id, username, company, count, price, true});

    cout << "Order " << new_order_id << " queued." << endl;
}

void TradingSystem ::cancel_buy_order(vector<Order>::iterator it)
{
    User *buyer = find_address_of_user(it->username);
    long long amount = 1LL * it->count * it->price;
    buyer->credit += amount;
    buyer->lock_credit -= amount;
    cout << "Canceled order " << it->id << "." << endl;
    buy_queue.erase(it);
    return;
}

void TradingSystem ::cancel_sell_order(vector<Order>::iterator it)
{
    User *seller = find_address_of_user(it->username);
    seller->shares[it->company].first += it->count;
    seller->shares[it->company].second -= it->count;
    cout << "Canceled order " << it->id << "." << endl;
    sell_queue.erase(it);
    return;
}

void TradingSystem ::cancel_order(int id)
{
    for (auto it = buy_queue.begin(); it != buy_queue.end(); ++it)
    {
        if (it->id == id)
        {
            cancel_buy_order(it);
            return;
        }
    }
    for (auto it = sell_queue.begin(); it != sell_queue.end(); ++it)
    {
        if (it->id == id)
        {
            cancel_sell_order(it);
            return;
        }
    }
    cout << "Order " << id << " not found." << endl;
}

int TradingSystem::get_company_price(const string &company_name) const
{
    const Company *company = find_address_of_company(company_name);
    if (company == nullptr)
        return 0;

    return company->get_price();
}

long long TradingSystem::calculate_total_free_assets(const User &user) const
{
    long long total = user.credit;

    for (const auto &[company_name, share_counts] : user.shares)
    {
        int free_shares = share_counts.first;
        int current_price = get_company_price(company_name);
        total += 1LL * free_shares * current_price;
    }
    return total;
}

void TradingSystem::print_portfolio_header(const User &user, long long total_free_assets) const
{
    cout << user.username << " Portfolio" << endl;
    cout << "Total free assets: $" << total_free_assets << endl;
    cout << "Free credit: $" << user.credit << endl;
    cout << "Locked credit: $" << user.lock_credit << endl;
}

void TradingSystem::print_free_shares(const User &user) const
{
    cout << "Free shares:" << endl;
    int index = 1;
    for (const auto &[company_name, share_counts] : user.shares)
    {
        int free_shares = share_counts.first;

        if (free_shares > 0)
        {
            cout << index << ". " << company_name << ": " << free_shares << endl;
            index++;
        }
    }

    if (index == 1)
        cout << "(empty)" << endl;
}

void TradingSystem::print_locked_shares(const User &user) const
{
    cout << "Locked shares:" << endl;

    int index = 1;
    for (const auto &[company_name, share_counts] : user.shares)
    {
        int locked_shares = share_counts.second;

        if (locked_shares > 0)
        {
            cout << index << ". " << company_name << ": " << locked_shares << endl;
            index++;
        }
    }

    if (index == 1)
        cout << "(empty)" << endl;
}

void TradingSystem::report_portfolio(const string &username)
{
    User *user = find_address_of_user(username);

    if (user == nullptr)
    {
        cout << username << " not found." << endl;
        return;
    }

    long long total_free_assets = calculate_total_free_assets(*user);

    print_portfolio_header(*user, total_free_assets);
    print_free_shares(*user);
    print_locked_shares(*user);
}

vector<Order> TradingSystem::get_company_sell_orders(const string &company_name) const
{
    vector<Order> result;

    for (const auto &order : sell_queue)
    {
        if (order.company == company_name)
            result.push_back(order);
    }

    sort(result.begin(), result.end(), [](const Order &a, const Order &b)
         {
        if (a.price != b.price)
            return a.price < b.price;

        return a.id < b.id; });

    return result;
}

vector<Order> TradingSystem::get_company_buy_orders(const string &company_name) const
{
    vector<Order> result;

    for (const auto &order : buy_queue)
    {
        if (order.company == company_name)
            result.push_back(order);
    }

    sort(result.begin(), result.end(), [](const Order &a, const Order &b)
         {
        if (a.price != b.price)
            return a.price > b.price;

        return a.id < b.id; });

    return result;
}

void TradingSystem::print_order_queue(const vector<Order> &orders) const
{
    if (orders.empty())
    {
        cout << "(empty)" << endl;
        return;
    }

    for (int i = 0; i < (int)orders.size(); i++)
    {
        cout << i + 1 << ". "
             << "Shares: " << orders[i].count
             << " - Price: $" << orders[i].price
             << " - ID: " << orders[i].id
             << endl;
    }
}

void TradingSystem::report_company(const string &company_name)
{
    const Company *company = find_address_of_company(company_name);

    if (company == nullptr)
    {
        cout << company_name << " not found." << endl;
        return;
    }

    vector<Order> sell_orders = get_company_sell_orders(company_name);
    vector<Order> buy_orders = get_company_buy_orders(company_name);

    cout << company_name << " report" << endl;
    cout << "Current price: $" << company->get_price() << endl;

    cout << "sell queue:" << endl;
    print_order_queue(sell_orders);

    cout << "buy queue:" << endl;
    print_order_queue(buy_orders);
}