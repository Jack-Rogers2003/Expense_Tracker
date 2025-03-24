// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2024/25)
// Department of Computer Science, Swansea University
//
// Author: 2210081
//
// Canvas: https://canvas.swansea.ac.uk/courses/52781
// -----------------------------------------------------

#include "item.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include "lib_json.hpp" 
#include <list>



// TODO Write a constructor that takes four parameters, a string identifier,
// a description, an amount, and a date and initialises the object and member data.
//
// Example:
//  Item iObj{"1", "Description", 1.99, Date(2024,12,25)};

Item::Item(const std::string& _id, const std::string& _description, double _amount, const Date& _date)
    : id(_id), description(_description), amount(_amount), date(_date) {}


// TODO Write a function, getIdent, that returns the identifier for the Item.
//
// Example:
//  Item iObj{"1", "Description", 1.99, Date(2024,12,25)};
//  auto ident = iObj.getIdent();

std::string Item::getIdent() const{
    return id;
}

// TODO Write a function, getDescription, that returns the description for the Item.
//
// Example:
//  Item iObj{"1", "Description", 1.99, Date(2024,12,25)};
//  auto ident = iObj.getDescription();

std::string Item::getDescription() const {
    return description;
}

// TODO Write a function, setDescription, that takes one parameter, a string for a new
//  Item description, and updates the member variable. It returns nothing.
//
// Example:
//  Item iObj{"1", "Description", 1.99, Date(2024,12,25)};
//  auto ident = iObj.setDescription("New Item Description");

void Item::setDescription(const std::string& newDescription) {
    description = newDescription;
}

// TODO Write a function, addTag, that takes one parameters, a tag
//  string and returns true if the entry was inserted into the
//  container or false if the tag already existed.
//
// Example:
//  Item iObj{"1", "Description", 1.99, Date(2024,12,25)};
//  iObj.addTag("tag");

bool Item::addTag(const std::string& tag) {
    auto result = std::find(tags.begin(), tags.end(), tag);
    if (result == tags.end()) {
        tags.push_back(tag);
        return true;  
    } else {
        return false;  
    }
}

// TODO Write a function, deleteTag, that takes one parameter, a tag
// string, deletes it from the container, and returns true if the tag
// string was deleted. If no tag exists with that name, throw an appropriate
// exception.
//
// Example:
//  Item iObj{"1", "Description", 1.99, Date(2024,12,25)};
//  iObj.addTag("tag");
//  iObj.deleteTag("tag");

bool Item::deleteTag(const std::string& tag) {
    for (std::string currentTag : tags) {
        if (currentTag == tag) {
            tags.remove(currentTag);
            return true;
        }
    }
    std::cerr << "Error: invalid tag argument(s)." << std::endl;
    throw std::out_of_range("");
}

// TODO Write a function, numTags, that takes no parameters and returns an
// unsigned int of the number of tags in the Item contains.
//
// Example:
//  Item iObj{"1", "Description", 1.99, Date(2024,12,25)};
//  iObj.numTags(); // 0

int Item::numTags() const {
    return tags.size();
}

// TODO Write a function, containsTag, that takes one parameter, a tag string.
// If the tag exists, return true. Otherwise return false.
//
// Example:
//  Item iObj{"1", "Description", 1.99, Date(2024,12,25)};
//  iObj.addTag("tag");
//  iObj.containsTag("tag"); // true

bool Item::containsTag(const std::string& tag) const {
    // Use find to check if the tag exists in the set
    for (std::string currentTag: tags) {
        if (tag == currentTag) {
            return true;
        }
    }
    return false; 
}

// TODO Write a function, getAmount, that returns the amount for the Item.

double Item::getAmount() const {
    return amount;
}

// TODO Write a function setAmount, that takes one parameter, a double for
// a new amount, and updates the member variable. It returns nothing.

void Item::setAmount(const int& newAmount) {
    amount = newAmount;
}

// TODO Write a function, getDate, that returns the date for the Item.

Date Item::getDate() const {
    return date;
}

// TODO Write a function setDate, that takes one parameter, a date, and updates
// the member variable. It returns nothing.

void Item::setDate(const Date& newDate) {
    date = newDate;
}

std::list<std::string> Item::getTags() const {
    return tags;
}

// TODO Write an == operator overload for the Item class, such that two
// Item objects are equal only if they have the same identifier, date,
// amount, description, and tags.
// Example:
//  Item iObj1{"Item Name"};
//  Item iObj2{"Item Name"};
//  if(iObj1 == iObj2) {
//   ...
//  }

bool Item::operator==(const Item& other) const {
    return id == other.id &&
           description == other.description &&
           amount == other.amount &&
           date == other.date &&
           tags == other.tags;  
}

// TODO Write a function, str, that takes no parameters and returns a
// std::string of the JSON representation of the data in the Item.
// See the coursework specification for how this JSON should look.
// Example:
//  Item iObj{"itemIdent"};
//  std::string s = iObj.str();

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

bool Item::operator<(const Item& other) const {
    return id < other.getIdent();  
}
