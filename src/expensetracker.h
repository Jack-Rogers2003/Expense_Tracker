
#ifndef EXPENSETRACKER_H
#define EXPENSETRACKER_H
#include "category.h"
#include <list>

class ExpenseTracker {
private:
    std::list<Category> categories;
public:
    ExpenseTracker();
    std::list<Category> getCategories() const;
    int size() const;
    Category newCategory(const Category& newCategory);
    bool addCategory(const Category& category);
    Category& getCategory(const std::string& id);
    bool deleteCategory(const std::string& id);
    double getSum() const;
    void load(const std::string& filename);
    void save(const std::string& filename) const;
    bool operator==(const ExpenseTracker& other) const;
    std::string str() const;
};

#endif // EXPENSETRACKER_H
