// include/Student.h

#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <iomanip>
#include <cstring>

class Student {
public:
    char admno[6];   // Admission Number (e.g., "S0010")
    char name[20];   // Student Name
    char stbno[6];   // Student Book Number (Book currently issued)
    int token;       // Number of books issued (0 or 1)

    // Default constructor initializes members to default values
    Student() {
        std::strcpy(admno, "");
        std::strcpy(name, "");
        std::strcpy(stbno, "");
        token = 0;
    }

    // Function to display student details
    void showStudent() const {
        std::cout << "\nAdmission Number: " << admno;
        std::cout << "\nStudent Name: " << name;
        std::cout << "\nNumber of Books Issued: " << token;
        if(token == 1) {
            std::cout << "\nBook Number: " << stbno;
        }
    }

    // Function to modify existing student details
    void modifyStudent() {
        std::cout << "\nModify Student Name: ";
        std::cin.ignore();
        std::cin.getline(name, 20);
    }

    // Function to display student information in a formatted way
    void report() const {
        std::cout << "\t" << admno << std::setw(20) << name << std::setw(10) << token << std::endl;
    }

    // Overload the output operator for easy display
    friend std::ostream& operator<<(std::ostream &os, const Student &student) {
        os << "Admission No: " << student.admno 
           << ", Name: " << student.name 
           << ", Books Issued: " << student.token;
        return os;
    }
};

#endif

