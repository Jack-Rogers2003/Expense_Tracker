
#ifndef ITEM_H
#define ITEM_H
#include "date.h"
#include <string>
#include <list>

class Item {
private:
    std::string id;
    std::string description;
    double amount;
    Date date;
    std::list<std::string> tags;
public:
    Item(const std::string& id, const std::string& description, double amount, const Date& date);
    std::string getIdent() const;
    std::string getDescription() const;
    void setDescription(const std::string& newDescription);
    bool addTag(const std::string& tag);
    bool deleteTag(const std::string& tag);
    int numTags() const;
    double getAmount() const;
    bool containsTag(const std::string& tag) const;
    void setAmount(const int& newAmount);
    Date getDate() const;
    void setDate(const Date& newDate);
    bool operator==(const Item& other) const;
    std::string str() const;
    std::list<std::string> getTags() const;
    bool operator<(const Item& other) const;
};

#endif // ITEM_H
