// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2024/25)
// Department of Computer Science, Swansea University
//
// Author: 2210081
//
// Canvas: https://canvas.swansea.ac.uk/courses/52781
// -----------------------------------------------------
// A Category contains one or more Expense Items, each with
// their own identifier ('ident').
// -----------------------------------------------------

#ifndef CATEGORY_H
#define CATEGORY_H
#include <string>
#include "item.h"
#include <list>


class Category {
private:
    std::string id;
    std::list<Item> items;
public:
    Category(std::string _id);
    unsigned int size() const;
    std::string getIdent() const;
    void setIdent(const std::string& newID);
    Item newItem(const std::string id, const std::string description, double price, const Date date);
    bool addItem(Item& newItem);
    Item& getItem(std::string _id);
    double getSum() const;
    bool deleteItem(const std::string& id);
    bool operator==(const Category& category) const;
    std::list<Item> getItems() const;
    std::string str() const;
    void merge(const Category& other);
    bool operator<(const Category& other) const;
};

#endif // CATEGORY_H
