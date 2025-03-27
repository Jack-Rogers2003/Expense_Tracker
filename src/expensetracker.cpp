// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2024/25)
// Department of Computer Science, Swansea University
//
// Author: 2210081
//
// Canvas: https://canvas.swansea.ac.uk/courses/52781
// -----------------------------------------------------
#include "expensetracker.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include "lib_json.hpp" 

// TODO Write a ExpenseTracker constructor that takes no parameters and constructs an
//  an ExpenseTracker object
//
// Example:
//  ExpenseTracker etObj{};

ExpenseTracker::ExpenseTracker() {

}

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of categories the ExpenseTracker contains.
//
// Example:
//  ExpenseTracker etObj{};
//  auto size = etObj.size();

int ExpenseTracker::size() const {
    return categories.size();
}

// TODO Write a function, newCategory, that takes one parameter, a category
//  identifier, and returns the Category object as a reference. If an object
//  with the same identifier already exists, then the existing object should be
//  returned. Throw a std::runtime_error if the Category object cannot be
//  inserted into the container.
//
// Example:
//  ExpenseTracker etObj{};
//  etObj.newCategory("categoryIdent");

Category ExpenseTracker::newCategory(const Category& id) {
    auto it = std::find(categories.begin(), categories.end(), Category(id));
    if (it != categories.end()) {
        return *it;  
    }
    
    try {
        Category newCategory(id);
        categories.push_back(newCategory); 
        return categories.back(); 
    } catch (const std::exception& e) {
        throw std::runtime_error("Error creating a new category: " + std::string(e.what()));
    }
}

// TODO Write a function, addCategory, that takes one parameter, a Category
//  object, and returns true if the object was successfully inserted. If an
//  object with the same identifier already exists, then the contents should be
//  merged (see also Category::addItem) and then returns false. Throw a
//  std::runtime_error if the Category object cannot be inserted into the
//  container for whatever reason.
//
// Example:
//  ExpenseTracker etObj{};
//  Category cObj{"categoryIdent"};
//  etObj.addCategory(cObj);

bool ExpenseTracker::addCategory(const Category& category) {
    auto it = std::find(categories.begin(), categories.end(), category.getIdent());
        
    if (it != categories.end()) {
        Category existingCategory = *it;  
        existingCategory.merge(category);  

        categories.erase(it);
        categories.push_back(existingCategory);

        return false;  
    }

    try {

        auto it = std::find(categories.begin(), categories.end(), category);
        if (it != categories.end()) {
            return false;  
        }
        try {
            categories.push_back(category);  
            return true;  
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to insert the category: " + std::string(e.what()));
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error creating a new category: " + std::string(e.what()));
    }
}

// TODO Write a function, getCategory, that takes one parameter, a Category
//  identifier and returns the Category with that identifier. If no Category
//  exists, throw an appropriate exception.
//
// Example:
//  ExpenseTracker etObj{};
//  etObj.newCategory("categoryIdent");
//  auto cObj = etObj.getCategory("categoryIdent");

Category& ExpenseTracker::getCategory(const std::string& id) {
    for (auto& category : categories) {
        if (category.getIdent() == id) {
            return category;  
        }
    }
    throw std::out_of_range("Error: invalid category argument(s).");    
}

// TODO Write a function, deleteCategory, that takes one parameter, a Category
//  identifier, and deletes that catagory from the container, and returns true
//  if the Category was deleted. If no Category exists, throw an appropriate
//  exception.
//
// Example:
//  ExpenseTracker etObj{};
//  etObj.newCategory("categoryIdent");
//  etObj.deleteCategory("categoryIdent");

bool ExpenseTracker::deleteCategory(const std::string& id) {
    for(Category currentCategory: categories) {
        if(currentCategory.getIdent() == id) {
            categories.remove(currentCategory);
            return true;
        }
    }
    std::cerr << "Error: invalid category argument(s)." << std::endl;
    throw std::out_of_range("Error: invalid category argument(s).");
}

// TODO Write a function, getSum, that returns the sum of all Category expense
// sums. This consists of the sum of all individual item amounts across all categories.
// If no categories or no items exists return 0.
//
// Example:
//  ExpenseTracker etObj{};
//  Category cObj = etObj.newCategory("categoryIdent");
//  Category cObj2 = etObj.newCategory("categoryIdent2");
//  cObj.newItem("newItemName", "Description", "1.0", Date(2024,12,25));
//  cObj.newItem("newItemName2", "Description", "2.0", Date(2024,12,25));
//  cObj2.newItem("newItemName3", "Description", "3.0", Date(2024,12,25));
//  cObj2.newItem("newItemName4", "Description", "4.0", Date(2024,12,25));
//  auto sum = ejObj.getSum() // 10.0

double ExpenseTracker::getSum() const {
    double sum = 0.0;
    for(Category currentCategory: categories) {
        sum = sum + currentCategory.getSum();
    }

    return sum;
}

// TODO Write a function, load, that takes one parameter, a std::string,
//  containing the filename for the database. Open the file, read the contents,
//  and populates the container for this ExpenseTracker. If the file does not open throw
//  an appropriate exception (either std::runtime_error or a derived class).
//
// A note on clashes:
//  If you encounter two categories with the same identifier, or two expense items with the same
//  identifier within the same category, you may choose whichever category/item is parsed
//  by the nlohmann json library. The json spec on duplicate keys is undefined, so
//  you can default to the behaviour of the nlohmann json library.
// JSON formatting:
//  The JSON file has the following format (see the sample database.json file
//  also provided with the coursework framework):
// {
//   "Studies": {
//     "1": {
//       "amount": 999.99,
//       "date": "2024-12-25",
//       "description": "Laptop",
//       "tags": [
//         "computer",
//         "programming",
//         "uni"
//       ]
//     },
//     ...
//   },
//   "Travel": {
//     "3": {
//       "amount": 164.0,
//       "date": "2024-12-30",
//       "description": "Bus Pass",
//       "tags": [
//         "bus",
//         "uni"
//       ]
//     }
//   },
//   ...
// }
//
// More help:
//  To help you with this function, I've selected the nlohmann::json
//  library that you must use for your coursework. Read up on how to use it
//  here: https://github.com/nlohmann/json. You may not use any other external
//  library other than the one I have provided. You may choose to process the
//  JSON yourself without the help of the library but I guarantee this will be
//  more work.
//
//  Understanding how to use external libraries is part of this coursework! You
//  will need to use this file to deserialize the JSON from string
//  to the JSON object provided by this library. Don't just look at the code
//  provided below, or in the README on the GitHub and despair. Google search,
//  look around, try code out in a separate file to all figure out how to use
//  this library.
//
//  Once you have deserialized the JSON string into an object, you will need to
//  loop through this object, constructing Category and Item objects according
//  to the JSON data in the file.
//
// Example:
//  ExpenseTracker etObj{};
//  etObj.load("database.json");

void ExpenseTracker::load(const std::string& filename) {
    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file: " + filename);
    }

    // Parse the JSON data
    nlohmann::json j;
    try {
        file >> j;  // Read the entire file into the JSON object
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing the JSON file: " + std::string(e.what()));
    }

    // Loop through each category in the JSON object
    for (auto& categoryPair : j.items()) {
        const std::string& categoryName = categoryPair.key();
        const auto& categoryData = categoryPair.value();

        // Create the Category object
        Category category = Category(categoryName);
        categories.push_back(category);

        // Loop through the items in the category
        for (auto& itemPair : categoryData.items()) {
            const std::string& itemId = itemPair.key();
            const auto& itemData = itemPair.value();

            // Create the Item object from the JSON data
            Item item(itemId, itemData["description"].get<std::string>(), itemData["amount"].get<double>(), itemData["date"].get<std::string>());

            // Add tags
            for (const auto& tag : itemData["tags"]) {
                item.addTag(tag.get<std::string>());
            }

            // Add the item to the category
            category.addItem(item);
        }

        // Insert the category into the ExpenseTracker
        try {
            addCategory(category);

        } catch (const std::runtime_error& e) {
            // Handle failure to add category if needed
            throw std::runtime_error("Failed to add category: " + categoryName);
        }
    }
}

// TODO Write a function, save, that takes one parameter, the path of the file
//  to write the database to. The function should serialise the ExpenseTracker object
//  as JSON.
//
// Example:
//  ExpenseTracker etObj{};
//  etObj.load("database.json");
//  ...
//  etObj.save("database.json");

void ExpenseTracker::save(const std::string& filename) const {
    nlohmann::json data;
    for (const auto& category : categories) {
        nlohmann::json categoryJson;

        for (const auto& item : category.getItems()) {
            nlohmann::json itemJson;
            itemJson["amount"] = item.getAmount();
            itemJson["date"] = item.getDate().toString();  
            itemJson["description"] = item.getDescription();
            itemJson["tags"] = item.getTags();  

            categoryJson[item.getIdent()] = itemJson;
        }

        data[category.getIdent()] = categoryJson;
    }

    // Open the file for writing and dump the serialized data into it
    std::ofstream file(filename);
    if (file.is_open()) {
        file << data;
        file.close();
    } else {
        throw std::runtime_error("Failed to open file for writing");
    }
}


// TODO Write an == operator overload for the ExpenseTracker class, such that two
//  ExpenseTracker objects are equal only if they have the exact same data.
//
// Example:
//  ExpenseTracker etObj1{};
//  ExpenseTracker etObj2{};
//  if(etObj1 == etObj2) {
//    ...
//  }

bool ExpenseTracker::operator==(const ExpenseTracker& other) const {
    std::list<Category> copy1 = getCategories();
    std::list<Category> copy2 = other.getCategories();

    copy1.sort();
    copy2.sort();
    return copy1 == copy2; 
}

/*
Return the expensetracker's set of categories 
*/
std::list<Category> ExpenseTracker::getCategories() const {
    return categories;
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the ExpenseTracker.
//
// Hint:
//  See the coursework specification for how this JSON should look.
//
// Example:
//  ExpenseTracker etObj{};
//  std::string s = etObj.str();

std::string ExpenseTracker::str() const {
    nlohmann::json data;

    for (const auto& category : categories) {
        nlohmann::json categoryJson;
        
        for (const auto& item : category.getItems()) {
            nlohmann::json itemJson;
            itemJson["amount"] = item.getAmount();
            itemJson["date"] = item.getDate().str();  
            itemJson["description"] = item.getDescription();
            itemJson["tags"] = item.getTags();  
            
            categoryJson[item.getIdent()] = itemJson;
        }

        data[category.getIdent()] = categoryJson;  
    }

    return data.dump();
}