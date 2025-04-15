

#include "category.h"
#include "item.h"
#include <stdexcept>
#include "lib_json.hpp" 
#include <iostream>
#include <list>
#include <string>




Category::Category(std::string _id) {
    id = _id;
}

unsigned int Category::size() const {
    return items.size();
}




std::string Category::getIdent() const {
    return id;
}


void Category::setIdent(const std::string& newID) {
    id = newID;
}

void Category::merge(const Category& other) {
    items.insert(items.end(), other.items.begin(), other.items.end());
}

Item Category::newItem(const std::string id, const std::string description, double price, const Date date) {
    try {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->getIdent() == id) {
                items.erase(it);  // Erase by iterator
                break;  // Exit the loop after removing the item
            }
        } 
        items.push_back(Item{id, description, price, date});

    } catch(const std::runtime_error& e) {
        std::cerr << "Error: invalid item argument(s)." << std::endl;
        throw std::out_of_range("Error: invalid item argument(s).");
    }
    return Item(id, description, price, date);

}



bool Category::addItem(Item& newItem) {
    for(Item currentItem: items) {
        if(currentItem.getIdent() == newItem.getIdent()) {
            currentItem.setDescription(newItem.getDescription());
            currentItem.setAmount(newItem.getAmount());
            currentItem.setDate(newItem.getDate());
            std::list<std::string> newTags = newItem.getTags();
            currentItem.getTags().insert(currentItem.getTags().end(), newTags.begin(), newTags.end());
            return false;
        };
    }
    items.push_back(newItem);
    return true;
}



Item& Category::getItem(std::string id) {
    for (auto& item : items) {
        if (item.getIdent() == id) {
            return item;  // Return the found category
        }
    }    
    throw std::out_of_range("Error: invalid item argument(s).");
}


double Category::getSum() const {
    double sum = 0.0;
    for(const Item& currentItem: items) {
        sum = sum + currentItem.getAmount();
    }
    return sum;
}



bool Category::deleteItem(const std::string& id) {
    for(Item currentItem: items) {
        if(currentItem.getIdent() == id) {
            items.remove(currentItem);
            return true;
        }
    }
    std::cerr << "Error: invalid item argument(s)." << std::endl;
    throw std::out_of_range("Error: invalid item argument(s).");
}



bool Category::operator==(const Category& category) const {
    //Same logic as was mentioned in the item operator==, as lists aren't equivalient
    //with the same lists of different orders, the lists are copied and then sort to prevent#
    //false negatives
    std::list<Item> copy1 = items;
    std::list<Item> copy2 = category.getItems();
    copy1.sort();
    copy2.sort();
    return id == category.getIdent() && copy1 == copy2;
}



std::string Category::str() const {
    nlohmann::json j;

    // Loop over all items and convert them to JSON
    for (const auto& item : items) {
        //convert item from string to json, this is to prevent formatting issues when returning
        //with .dump()
        j[item.getIdent()] = nlohmann::json::parse(item.str());
    }
    return j.dump();

}

/*
Iterates through the list of items and returns true if the item id passed through exists within the set, or false if it does not
*/
bool Category::checkItemExists(std::string itemId) {
    for (const auto& item : items) {
        if(item.getIdent() == itemId) {
            return true;
        }
    }
    return false;
}

/*
Compares the item parameter to the current category object based on their ID, included to allow the
 .sort() to check through the items when called
*/
bool Category::operator<(const Category& other) const {
    return id < other.getIdent();  
}

/*
Returns the list of items within the Category
*/
std::list<Item> Category::getItems() const {
    return items;
}
