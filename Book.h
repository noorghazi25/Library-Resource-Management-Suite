// include/Book.h

#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <iomanip>
#include <cstring>

class Book {
public:  // Ensure that these members are public
    char bno[6];     // Book Number (e.g., "B0010")
    char bname[50];  // Book Name
    char aname[20];  // Author Name

    // Default constructor initializes members to default values
    Book() {
        std::strcpy(bno, "");
        std::strcpy(bname, "");
        std::strcpy(aname, "");
    }

    // Function to display book details
    void showBook() const {
        std::cout << "\nBook Number: " << bno;
        std::cout << "\nBook Name: " << bname;
        std::cout << "\nAuthor Name: " << aname;
    }

    // Function to modify existing book details
    void modifyBook() {
        std::cout << "\nModify Book Name: ";
        std::cin.ignore();
        std::cin.getline(bname, 50);
        std::cout << "\nModify Author's Name: ";
        std::cin.getline(aname, 20);
    }

    // Function to display book information in a formatted way
    void report() const {
        std::cout << std::left << std::setw(10) << bno 
                  << std::setw(30) << bname 
                  << std::setw(30) << aname << std::endl;
    }

    // Overload the output operator for easy display
    friend std::ostream& operator<<(std::ostream &os, const Book &book) {
        os << "Book No: " << book.bno 
           << ", Name: " << book.bname 
           << ", Author: " << book.aname;
        return os;
    }
};

#endif




