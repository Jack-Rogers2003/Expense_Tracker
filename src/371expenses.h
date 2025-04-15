

#ifndef _371EXPENSES_H
#define _371EXPENSES_H

#include <string>

#include "lib_cxxopts.hpp"
#include "expensetracker.h"

namespace App {



enum Action { CREATE, SUM, JSON, DELETE, UPDATE };

int run(int argc, char *argv[]);

cxxopts::Options cxxoptsSetup();

App::Action parseActionArgument(cxxopts::ParseResult &args);

std::string getJSON(ExpenseTracker &et);
std::string getJSON(ExpenseTracker &et, const std::string &c);
std::string getJSON(ExpenseTracker &et, const std::string &c, const std::string &id);
void addTags(ExpenseTracker& etObj, std::string item, std::string category, std::string tag);
} // namespace App

#endif // _371EXPENSES_H
