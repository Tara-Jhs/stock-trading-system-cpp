#include"TradingSystem.hpp"
#include"Order.hpp"
#include"function.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: ./UTTrade <path/to/companies.csv> <path/to/shareholders.csv>" << endl;
        return 1;
    }
    TradingSystem system;
    system.loadCompanies(argv[1]);
    system.loadShareholders(argv[2]);
    string command;

    while (cin >> command)
    {
        run_program(system, command);
    }

    return 0;
}