

#include "371expenses.h"
#include "lib_cxxopts.hpp"
#include "lib_json.hpp" 
#include "date.h"
#include "category.h"
#include <vector>
#include <sstream>




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

  Action a;
  try {
    a = parseActionArgument(args);
  } catch(const std::exception& e) {
    std::cerr << "Error: invalid action argument(s)." << std::endl;
    return 1;      
  }

  //Get arguments from what was inputted, setting a default value if they don't exist
  std::string category = args.count("category") ? args["category"].as<std::string>() : "";
  std::string item = args.count("item") ? args["item"].as<std::string>() : "";
  double amount = 1.0;
  //check that amount has been passed as a double to prevent crash
  try {
    amount = args.count("amount") ? std::stod(args["amount"].as<std::string>()) : 1.0;
  } catch (const std::exception&) {
    std::cerr << "Error: invalid amount argument(s)." << std::endl;
    return 1;
}
  std::string description = args.count("description") ? args["description"].as<std::string>() : "";
  std::string date = args.count("date") ? args["date"].as<std::string>() : "";
  std::string tag = args.count("tag") ? args["tag"].as<std::string>() : "";


  switch (a) {
    //args.count is used to check existing rather than default value as possible default value is the intended input
    case Action::CREATE:
    if (args.count("category")) {
      //creating category
      if (!args.count("item") && !args.count("amount") && !args.count("description") && !args.count("date")) {
        etObj.addCategory(Category(category));
      } else if(args.count("item")) {
        try {
          etObj.getCategory(category);
        } catch(const std::exception& e) {
          std::cerr << "Error: invalid category argument(s)." << std::endl;
          return 1;
        }
        //creating new item
        if(args.count("amount") && args.count("description") && !etObj.getCategory(category).checkItemExists(item)) {
          Date dateToAdd = Date();
          if (args.count("date")) {
            try {
              dateToAdd = Date(date);
            } catch(const std::exception& e) {
              std::cerr << "Error: invalid date argument(s)." << std::endl;
              return 1;              
            }
          }

          etObj.getCategory(category).newItem(item, description, amount, dateToAdd);
          //create tags with an item if provided
          if (args.count("tag")) {
            addTags(etObj, item, category, tag);
          }
          //Not creating a new item but creating a new tag for an existing item
        } else {
          try {
            etObj.getCategory(category).getItem(item);
          } catch(const std::exception& e) {
            std::cerr << "Error: invalid item argument(s)." << std::endl;
            return 1;  
          }
          if (args.count("tag")) {
            addTags(etObj, item, category, tag);
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
        //output contents of whole expensetracker
        std::cout << getJSON(etObj) << std::endl;
        break;

      } else if (args.count("category")) {
        try {
          etObj.getCategory(category);
        } catch(const std::exception& e) {
          std::cerr << "Error: invalid category argument(s)." << std::endl;
          return 1;       
        }
        if (!args.count("item")) {
          //output contents of provided category
          std::cout << getJSON(etObj, category) << std::endl;
          break;
        } else {
          try {
            etObj.getCategory(category).getItem(item);
          } catch(const std::exception& e) {
            std::cerr << "Error: invalid item argument(s)." << std::endl;
            return 1;  
          }
          //output contents of provided item within a category
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
        std::cerr << "Error: invalid category argument(s)." << std::endl;
        return 1;       
      }
      //update category id, splits by semicolon as specified in specification, the 
      //first element is the identifer, second element what to replace it with
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
          std::cerr << "Error: invalid item argument(s)." << std::endl;
          return 1;       
        }
        //updates item by passed elements
        if(args.count("amount")) {
          etObj.getCategory(category).getItem(item).setAmount(amount);
        } if (args.count("description")) {
          etObj.getCategory(category).getItem(item).setDescription(description);
        } if (args.count("date")) {
          try {
            etObj.getCategory(category).getItem(item).setDate(Date(date));
          } catch(const std::exception& e) {
            std::cerr << "Error: invalid date argument(s)." << std::endl;
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
        std::cerr << "Error: invalid category argument(s)." << std::endl;
        return 1;       
      }
      if (!args.count("item")) {
        //deletes category
        etObj.deleteCategory(category);
      } else {
        try {
          etObj.getCategory(category).getItem(item);
        } catch(const std::exception& e) {
          std::cerr << "Error: invalid item argument(s)." << std::endl;
          return 1;       
        }
        if (!args.count("tag")) {
          //deletes item
          etObj.getCategory(category).deleteItem(item);
        } else {
          try {
            //deletes tag from item
            etObj.getCategory(category).getItem(item).deleteTag(tag);
          } catch(const std::exception& e) {
            std::cerr << "Error: invalid tag argument(s)." << std::endl;
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
          std::cerr << "Error: invalid category argument(s)." << std::endl;
          return 1;       
        }
        //sums category
        std::cout << etObj.getCategory(category).getSum() << std::endl;
      } else {
        //sums expensetracker
        std::cout << etObj.getSum() << std::endl;
      }
    break;

    default:
    std::cerr << "Error: invalid action argument(s)." << std::endl;
    return 1;       
  }
  //If a request reaches this point we know it was valid so save to the db
  etObj.save(db);
  return 0;
} 

//Adds tags to an item within a specific catgeory
void App::addTags(ExpenseTracker& etObj, std::string item, std::string category, std::string tag) {
  std::stringstream ss(tag);
  std::string tagToAdd;
  while (std::getline(ss, tagToAdd, ',')) {
    etObj.getCategory(category).getItem(item).addTag(tagToAdd);
  }
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

std::string App::getJSON(ExpenseTracker &etObj) {
  // Only uncomment this once you have implemented the functions used!
  return etObj.str();
}

std::string App::getJSON(ExpenseTracker &etObj, const std::string &c) {
  // Only uncomment this once you have implemented the functions used!
  auto cObj = etObj.getCategory(c);
  return cObj.str();
}


std::string App::getJSON(ExpenseTracker &etObj, 
                         const std::string &c,
                         const std::string &id) {
  // Only uncomment this once you have implemented the functions used!
  auto iObj = etObj.getCategory(c).getItem(id);
  return iObj.str();
}
