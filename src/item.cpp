
#include "item.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include "lib_json.hpp" 
#include <list>




Item::Item(const std::string& _id, const std::string& _description, double _amount, const Date& _date)
    : id(_id), description(_description), amount(_amount), date(_date) {}



std::string Item::getIdent() const{
    return id;
}


std::string Item::getDescription() const {
    return description;
}

void Item::setDescription(const std::string& newDescription) {
    description = newDescription;
}

bool Item::addTag(const std::string& tag) {
    auto result = std::find(tags.begin(), tags.end(), tag);
    if (result == tags.end()) {
        tags.push_back(tag);
        return true;  
    } else {
        return false;  
    }
}

bool Item::deleteTag(const std::string& tag) {
    for (std::string currentTag : tags) {
        if (currentTag == tag) {
            tags.remove(currentTag);
            return true;
        }
    }
    throw std::out_of_range("Error: invalid tag argument(s).");
}

int Item::numTags() const {
    return tags.size();
}


bool Item::containsTag(const std::string& tag) const {
    // Use find to check if the tag exists in the set
    for (std::string currentTag: tags) {
        if (tag == currentTag) {
            return true;
        }
    }
    return false; 
}


double Item::getAmount() const {
    return amount;
}

void Item::setAmount(const int& newAmount) {
    amount = newAmount;
}


Date Item::getDate() const {
    return date;
}


void Item::setDate(const Date& newDate) {
    date = newDate;
}


bool Item::operator==(const Item& other) const {
    //as list equivalence returns false is the list has the same data but is a different order, we copy
    //the lists and use .sort to prevent any false negatives
    std::list<std::string> copy1 = getTags();
    std::list<std::string> copy2 = other.getTags();

    copy1.sort();
    copy2.sort();

    return copy1 == copy2;
    return id == other.id &&
           description == other.description &&
           amount == other.amount &&
           date == other.date &&
           copy1 == copy2;  
}


std::string Item::str() const {
    // Create a JSON object using nlohmann::json
    nlohmann::json j;

    // Populate the JSON object
    j["amount"] = amount;
    j["date"] = date.toString();  
    j["description"] = description;
    j["tags"] = std::vector<std::string>(tags.begin(), tags.end());  // Convert set to vector for JSON

    return j.dump();
}

/*
Compares the item parameter to the current item object based on their ID, included to allow the
 .sort() to check through the items when called
*/
bool Item::operator<(const Item& other) const {
    return id < other.getIdent();  
}

//Returns the lists of tags attached to the item
std::list<std::string> Item::getTags() const {
    return tags;
}
