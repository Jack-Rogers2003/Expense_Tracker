

#include "date.h"
#include <ctime>
#include <stdexcept>
#include <sstream>
#include <string>
#include <iostream>

Date::Date() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    year = now->tm_year + 1900;
    month = now->tm_mon + 1;
    day = now->tm_mday;
}


Date::Date(int y, int m, int d) {
    if (!isValidDate(y, m, d)) {
        throw std::invalid_argument("Invalid date format");   
    }
    year = y;
    month = m;
    day = d;
}


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



std::string Date::str() const {
    std::ostringstream ss;
    ss << year << "-" 
       << (month < 10 ? "0" : "") << month << "-" 
       << (day < 10 ? "0" : "") << day;
    return ss.str();
}


void Date::setDate(int y, int m, int d) {
    if (!isValidDate(y, m, d)) {
        throw std::invalid_argument("Invalid date");
    }
    year = y;
    month = m;
    day = d;
}


unsigned int Date::getYear() const { return year; }



unsigned int Date::getMonth() const { return month; }



unsigned int Date::getDay() const { return day; }



bool Date::operator==(const Date& other) const {
    return (year == other.year && month == other.month && day == other.day);
}




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
