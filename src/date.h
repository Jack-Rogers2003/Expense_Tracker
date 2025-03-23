// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2024/25)
// Department of Computer Science, Swansea University
//
// Author: 2210081
//
// Canvas: https://canvas.swansea.ac.uk/courses/52781
// -----------------------------------------------------
// An Date class that contains the following member variables
// - year
// - month
// - day
// -----------------------------------------------------

#ifndef DATE_H
#define DATE_H
#include <string>


class Date {
private:
    int year, month, day;

public:
    Date();  
    Date(int y, int m, int d); 
    Date(const std::string& dateString); 
    std::string str() const;  
    void setDate(int y, int m, int d); 

    unsigned int getYear() const;
    unsigned int getMonth() const;
    unsigned int getDay() const;

    bool operator==(const Date& other) const;
    bool operator<(const Date& other) const;

    std::string toString() const;
};

#endif // DATE_H
