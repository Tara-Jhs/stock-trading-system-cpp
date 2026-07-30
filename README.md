# Stock Trading Management System

An object-oriented command-line stock trading management system developed individually in C++ as a university Advanced Programming project.

## Overview

This project simulates a simplified stock exchange in which shareholders can register buy and sell orders for different companies.

The system manages shareholders, account balances, stock portfolios, order queues, transactions, locked assets, current stock prices, and financial reports.

## Features

- Loading company information from a CSV file
- Loading shareholder information from a CSV file
- Registering new shareholders
- Managing account credit and stock portfolios
- Creating buy and sell orders
- Assigning a unique ID to every order
- Maintaining separate buy and sell queues for each company
- Matching compatible buy and sell orders
- Prioritizing older orders when multiple matches are available
- Transferring money and shares after successful transactions
- Updating the current stock price after each transaction
- Locking credit for queued buy orders
- Locking shares for queued sell orders
- Cancelling queued orders and releasing locked assets
- Generating shareholder portfolio reports
- Generating company and order-queue reports
- Input validation and error handling

## Order Matching

A buy order and a sell order are matched when they belong to the same company and have the same:

- Number of shares
- Price per share

When multiple compatible orders exist, the order with the smaller ID has priority.

If no compatible order is found, the new order is added to the corresponding queue.

## Input Files

The program receives two CSV file paths through command-line arguments:

1. Companies file
2. Shareholders file

Example companies file:

```csv
Name,Price
Foolad,50
Khodro,70
Petroshimi,30
```

Example shareholders file:

```csv
Username,Credit,Shares
Mehrab,500,Foolad:5
Daei,100,Petroshimi:2;Foolad:3;Khodro:1
```

## Main Commands

```text
register <username> <credit>
sell_order <username> <shares_count> <company> <share_price>
buy_order <username> <shares_count> <company> <share_price>
cancel_order <id>
report_portfolio <username>
report_company <company>
```

## Build

Compile the project using the provided Makefile:

```bash
make
```

The project uses the C++20 standard.

## Run

```bash
./UTTrade <path-to-companies.csv> <path-to-shareholders.csv>
```

Example:

```bash
./UTTrade companies.csv shareholders.csv
```

## Programming Concepts

- Object-oriented programming
- Classes and encapsulation
- Multi-file C++ projects
- Modular software design
- File handling
- CSV processing
- Queues and containers
- Order-matching algorithms
- Sorting and prioritization
- Command-line interfaces
- Input validation
- Error handling

## Technologies

- C++
- C++20
- Makefile
- Visual Studio Code
- Linux command line

## Author

**Tara Jahanshahi**

Computer Engineering Student  
University of Tehran
