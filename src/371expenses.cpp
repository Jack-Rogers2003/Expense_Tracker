// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2024/25)
// Department of Computer Science, Swansea University
//
// Author: 2210081
//
// Canvas: https://canvas.swansea.ac.uk/courses/52781
// -----------------------------------------------------

#include "371expenses.h"
#include "lib_cxxopts.hpp"
#include "lib_json.hpp" 
#include "date.h"
#include "category.h"
#include <vector>
#include <sstream>



// TODO Complete this function. You have been provided some skeleton code which
//  retrieves the database file name from cxxopts.
//  1. Load the database file by calling load() on a ExpenseTracker object
//  2. Parse the 'action' argument to decide what action should be taken
//     (json, create, sum, delete, update).  'json' and 'sum' are the easiest to implement and
//     'update' is the hardest. The details of how these arguments work is in
//     the coursework specification.
//  3. Save the updated ExpenseTracker object to the JSON file if there have been
//     changes (calling save() on the ExpenseTracker object).
//
// Some commented out code has been provided. Using some of this will be
// demonstrated in the coursework video on Canvas. Remember, this coursework is
// meant to be challenging and testing your understanding of programming in C++.
// Part of the challenge is figuring things out on your own. That is a major
// part of software development.
//
// Example:
//  int main(int argc, char *argv[]) { return App::run(argc, argv); }
int App::run(int argc, char *argv[]) {
  auto options = App::cxxoptsSetup();
  auto args = options.parse(argc, argv);

  if (args.count("help")) {
      std::cout << options.help() << '\n';
      return 0;
  }

  std::string db;
  if (args.count("db")) {
    db = args["db"].as<std::string>();
  } else {
    db = "database.json";
  }

  if (!args.count("action")) {
      std::cerr << "Error: Missing required --action argument\n";
      return 1;
  }

  ExpenseTracker etObj{};
  etObj.load(db);
  etObj.getCategories();

  Action a = parseActionArgument(args);

  //Get arguments from what was inputted, setting a default value if they don't exist
  std::string category = args.count("category") ? args["category"].as<std::string>() : "";

  std::string item = args.count("item") ? args["item"].as<std::string>() : "";
  double amount = args.count("amount") ? std::stod(args["amount"].as<std::string>()) : 1.0;
  std::string description = args.count("description") ? args["description"].as<std::string>() : "";
  std::string date = args.count("date") ? args["date"].as<std::string>() : "";
  std::string tag = args.count("tag") ? args["tag"].as<std::string>() : "";


  switch (a) {

    case Action::CREATE:
    //args.count is used to check existing rather than default value as possible default value is the intended input
    if (args.count("category")) {
      //creaying category
      if (!args.count("item") && !args.count("amount") && !args.count("description") && !args.count("date")) {
        etObj.addCategory(Category(category));
      } else if(args.count("item")) {
        try {
          etObj.getCategory(category);
        } catch(const std::exception& e) {
          return 1;
        }
        //creating new item
        if(args.count("amount") && args.count("description")) {
          Date date = Date();
          if (args.count("date")) {
            try {
              date = Date(date);
            } catch(const std::exception& e) {
              return 1;              }
          }

          etObj.getCategory(category).newItem(item, description, amount, date);
          if (args.count("tag")) {
            std::stringstream ss(tag);
            std::vector<std::string> result;
            std::string tag;
            while (std::getline(ss, tag, ',')) {
              etObj.getCategory(category).getItem(item).addTag(tag);
            }
          }
          //Not creating a new time but creating a new tag for an existing item
        } else {
          try {
            etObj.getCategory(category).getItem(item);
          } catch(const std::exception& e) {
            return 1;  
          }
          if (args.count("tag")) {
            std::stringstream ss(tag);
            std::vector<std::string> result;
            std::string tag;
            while (std::getline(ss, tag, ',')) {
              etObj.getCategory(category).getItem(item).addTag(tag);
            }
          }
        }
      }
    } else {
      std::cerr << "Error: missing category, item, amount, description argument(s)." << std::endl;
      return 1;       
    }    
    break;

    case Action::JSON:
      if (!args.count("category") && !args.count("item")) {
        std::cout << getJSON(etObj) << std::endl;
        break;

      } else if (args.count("category")) {
        try {
          etObj.getCategory(category);
        } catch(const std::exception& e) {
          return 1;       
        }
        if (!args.count("item")) {
          std::cout << getJSON(etObj, category) << std::endl;
          break;
        } else {
          try {
            etObj.getCategory(category).getItem(item);
          } catch(const std::exception& e) {
            return 1;  
          }
          std::cout << getJSON(etObj, category, item) << std::endl;
          break;   
        }
      } else {
        std::cerr << "Error: missing category argument(s)." << std::endl;
        return 1;       
      }
    break;

    case Action::UPDATE:
    if (args.count("category")) {
      try {
        etObj.getCategory(category);
      } catch(const std::exception& e) {
        return 1;       
      }
      if (!args.count("item") && !args.count("amount")) {
        std::stringstream ss(category);
        std::vector<std::string> result;
        std::string categoryName;
        while (std::getline(ss, categoryName, ':')) {
          result.push_back(categoryName);
        }      
        etObj.getCategory(result.front()).setIdent(result.back());
      } else if (args.count("item")) {
        try {
          etObj.getCategory(category).getItem(item);
        } catch(const std::exception& e) {
          return 1;       
        }
        if(args.count("amount")) {
          etObj.getCategory(category).getItem(item).setAmount(amount);
        } if (args.count("description")) {
          etObj.getCategory(category).getItem(item).setDescription(description);
        } if (args.count("date")) {
          try {
            etObj.getCategory(category).getItem(item).setDate(Date(date));
          } catch(const std::exception& e) {
            return 1;       
          }
        }
      }
    } else {
      std::cerr << "Error: missing category, item, amount, description argument(s)." << std::endl;
      return 1;   
    }
    break;

    case Action::DELETE:
    if (args.count("category")) {
      try {
        etObj.getCategory(category);
      } catch(const std::exception& e) {
        return 1;       
      }
      if (!args.count("item")) {
        etObj.deleteCategory(category);
      } else {
        try {
          etObj.getCategory(category).getItem(item);
        } catch(const std::exception& e) {
          return 1;       
        }
        if (!args.count("tag")) {
          etObj.getCategory(category).deleteItem(item);
        } else {
          try {
            etObj.getCategory(category).getItem(item).deleteTag(tag);
          } catch(const std::exception& e) {
            return 1;    
          }
        }
      }
    } else {
      std::cerr << "Error: missing category, item, amount, description argument(s)." << std::endl;
      return 1;   
    }
    break;

    case Action::SUM:
      if (args.count("category")) {
        try {
          etObj.getCategory(category);
        } catch(const std::exception& e) {
          return 1;       
        }
        std::cout << etObj.getCategory(category).getSum() << std::endl;
      } else {
        std::cout << etObj.getSum() << std::endl;
      }
    break;

    default:
    std::cerr << "Error: invalid action argument(s)." << std::endl;
    return 1;       
  }
  etObj.save(db);
  return 0;
} 

// Create a cxxopts instance. You do not need to modify this function.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
cxxopts::Options App::cxxoptsSetup() {
  cxxopts::Options cxxopts("371expenses", "Student ID: " + STUDENT_NUMBER + "\n");

  cxxopts.add_options()(
      "db", "Filename of the 371expenses database",
      cxxopts::value<std::string>()->default_value("database.json"))(

      "action",
      "Action to take, can be: 'create', 'json', 'update', 'delete', 'sum'.",
      cxxopts::value<std::string>())(

      "category",
      "Apply action (create, json, update, delete, sum) to a category. If you "
      "want to add a category, set the action argument to 'create' and the "
      "category argument to your chosen category identifier.",
      cxxopts::value<std::string>())(

      "description",
      "Apply action (create, update) to an expense description. If you want to "
      "add an expense item, set the action argument to 'create', the category "
      "argument to your chosen category identifier and the description "
      "argument to the expense description and the amount argument to the "
      "expense amount, and the id argument to the unique expense id.",
      cxxopts::value<std::string>())(

      "amount",
      "Apply action (create, update) to an expense amount. If you want to add "
      "an expense item, set the action argument to 'create', the category "
      "argument to your chosen category identifier and the description "
      "argument to the expense description and the amount argument to the "
      "expense amount, and the id argument to the unique expense id.",
      cxxopts::value<std::string>())(

      "item",
      "Apply action (create, update, json, delete) to an expense item "
      "identified by its id. If you want to update an expense item, set the "
      "action argument to 'update', the id argument to the expense identifier, "
      "the description argument to the updated expense description, and the "
      "amount argument to the updated expense amount.",
      cxxopts::value<std::string>())(

      "date",
      "When creating or updating an expense item, set the date flag to change "
      "the expense item's date to the one specified as an argument (e.g. "
      "'2024-11-23'). If the date argument is ommitted set the expense date "
      "to today's date when creating an expense item, and leave the date "
      "unchanged if omitted when updating an expense item.",
      cxxopts::value<std::string>())(

      "tag",
      "Apply action (create or delete) to a tag.  If you want to add a tag, "
      "set the action argument to 'create', the category argument to your "
      "chosen category identifier, the item argument to your chosen item "
      "identifier, and the tag argument to a single tag 'tag' or comma "
      "seperated list of tags: 'tag1,tag2'). The action update/sum/json are "
      "unsupported here.",
      cxxopts::value<std::string>())(

      "h,help", "Print usage.");

  return cxxopts;
}

// TODO Rewrite this function so that it works. This function should
//  case-insensitively check the action argument retrieved from cxxopts and
//  convert this to a value from the ACTION enum. If an invalid value is given
//  in a string, throw an std::invalid_argument exception.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
//  App::Action action = parseActionArgument(args);
App::Action App::parseActionArgument(cxxopts::ParseResult &args) {
  std::string input = args["action"].as<std::string>();

  std::transform(input.begin(), input.end(), input.begin(), ::tolower);
  if (input == "create") {
      return Action::CREATE;
  } else if (input == "json") {
      return Action::JSON;
  } else if (input == "update") {
      return Action::UPDATE;
  } else if (input == "delete") {
      return Action::DELETE;
  } else if (input == "sum") {
      return Action::SUM;
  } else {
    throw std::invalid_argument("action"); 
  }
}

// TODO Write a function, getJSON, that returns a std::string containing the
// JSON representation of a ExpenseTracker object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  ExpenseTracker etObj{};
//  std::cout << getJSON(etObj);
std::string App::getJSON(ExpenseTracker &etObj) {
  // Only uncomment this once you have implemented the functions used!
  return etObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Category in a ExpenseTracker object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function and
// make any other adjustments).
//
// Example:
//  ExpenseTracker etObj{};
//  std::string c = "category argument value";
//  std::cout << getJSON(etObj, c);
std::string App::getJSON(ExpenseTracker &etObj, const std::string &c) {
  // Only uncomment this once you have implemented the functions used!
  auto cObj = etObj.getCategory(c);
  return cObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific ExpenseItem in a ExpenseTracker object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function and
// make any other adjustments).
//
// Example:
//  ExpenseTracker etObj{};
//  std::string c = "category argument value";
//  std::string id = "expense id argument value";
//  std::cout << getJSON(etObj, c, id);
std::string App::getJSON(ExpenseTracker &etObj, 
                         const std::string &c,
                         const std::string &id) {
  // Only uncomment this once you have implemented the functions used!
  auto iObj = etObj.getCategory(c).getItem(id);
  return iObj.str();
}
