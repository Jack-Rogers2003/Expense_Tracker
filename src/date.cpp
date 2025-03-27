// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2024/25)
// Department of Computer Science, Swansea University
//
// Author: 2210081
//
// Canvas: https://canvas.swansea.ac.uk/courses/52781
// -----------------------------------------------------

#include "date.h"
#include <ctime>
#include <stdexcept>
#include <sstream>
#include <string>
#include <iostream>

// TODO write a default constructor that sets the member variables to today's
// date. 
// Example:
//  Date d = Date();

Date::Date() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    year = now->tm_year + 1900;
    month = now->tm_mon + 1;
    day = now->tm_mday;
}

// TODO write a constructor that takes a three integer argument and sets the
// appropriate member variables (year, month, day). If the date is not valid
// throw an appropriate exception.
// Example:
//  Date d = Date("2024-12-25");
Date::Date(int y, int m, int d) {
    if (!isValidDate(y, m, d)) {
        throw std::invalid_argument("Invalid date format");   
    }
    year = y;
    month = m;
    day = d;
}

// TODO write a constructor that takes a string argument in "YYYY-MM-DD" format
// and sets the appropriate member variables (year, month, day). If dateString
// is not valid throw an appropriate exception.
// Example:
//  Date d = Date("2024-12-25");

Date::Date(const std::string& dateString) {
    std::istringstream ss(dateString);
    char dash1, dash2;
    int y, m, d;
    
    //check the format is valid and that the date is valid
    if (!(ss >> y >> dash1 >> m >> dash2 >> d) || dash1 != '-' || dash2 != '-' || !isValidDate(y, m, d)) {
        throw std::invalid_argument("Invalid date format");   
    }
    
    year = y;
    month = m;
    day = d;
}

// TODO Write a function, str, that takes no parameters and returns a
// std::string representation of the Date object in YYYY-MM-DD format.  
// Example:
//  Date d = Date(2024,12,25);
//  std::cout << d.str() << std::endl;

std::string Date::str() const {
    std::ostringstream ss;
    ss << year << "-" 
       << (month < 10 ? "0" : "") << month << "-" 
       << (day < 10 ? "0" : "") << day;
    return ss.str();
}

// TODO create a function setDate, that takes three parameters: year, month,
// day and sets the appropriate member variables to those parameters.
// Example:
//  Date d = Date();
//  d.setDate(2024, 12, 25);

void Date::setDate(int y, int m, int d) {
    if (!isValidDate(y, m, d)) {
        throw std::invalid_argument("Invalid date");
    }
    year = y;
    month = m;
    day = d;
}

// TODO Write a function, getYear, that takes no parameters and returns year
// member variable
// Example:
//  Date d = Date();
//  auto year = d.getYear();

unsigned int Date::getYear() const { return year; }


// TODO Write a function, getMonth, that takes no parameters and returns month
// member variable
// Example:
//  Date d = Date();
//  auto month = d.getMonth();

unsigned int Date::getMonth() const { return month; }


// TODO Write a function, getDay, that takes no parameters and returns day
// member variable
// Example:
//  Date d = Date();
//  auto day = d.getDay();

unsigned int Date::getDay() const { return day; }


// TODO Write an == operator overload for the Date class, such that two
// Date objects are equal only if they have the same member variables.
// Example:
//  Date d1 = Date(2024, 12, 25);
//  Date d2 = Date(2024, 12, 25);
//  if (d1 == d2) {
//    ...
//  }

bool Date::operator==(const Date& other) const {
    return (year == other.year && month == other.month && day == other.day);
}


// TODO Write an < operator overload for the Date class, that returns true if
// the first Date object is chronologically before the second date object.
// Otherwise return false. 
// Example:
//  Date d1 = Date(2024, 12, 25);
//  Date d2 = Date();
//  Date d2 = Date(2024, 12, 31);
//   if (d1 < d2) {
//     ...
//   }

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

/*
Convert date to a string in the format requested by the specification
*/
std::string Date::toString() const {
    return std::to_string(year) + "-" + (month < 10 ? "0" : "") + std::to_string(month) + "-" + (day < 10 ? "0" : "") + std::to_string(day);
}

/*
Checks if the date passed through as 3 seperate ints is a valid date
*/
bool Date::isValidDate(int year, int month, int day) const{
    if (month < 1 || month > 12 || day < 1) return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    //leap year check
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        daysInMonth[1] = 29;
    }
    
    return day <= daysInMonth[month - 1];
}
