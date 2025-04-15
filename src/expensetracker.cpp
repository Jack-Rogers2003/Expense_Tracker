
#include "expensetracker.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include "lib_json.hpp" 


ExpenseTracker::ExpenseTracker() {

}


int ExpenseTracker::size() const {
    return categories.size();
}


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



Category& ExpenseTracker::getCategory(const std::string& id) {
    for (auto& category : categories) {
        if (category.getIdent() == id) {
            return category;  
        }
    }
    throw std::out_of_range("Error: invalid category argument(s).");    
}


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


double ExpenseTracker::getSum() const {
    double sum = 0.0;
    for(Category currentCategory: categories) {
        sum = sum + currentCategory.getSum();
    }

    return sum;
}



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