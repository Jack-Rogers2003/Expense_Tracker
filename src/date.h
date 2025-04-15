
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
    bool isValidDate(int year, int month, int day) const;
};

#endif // DATE_H
