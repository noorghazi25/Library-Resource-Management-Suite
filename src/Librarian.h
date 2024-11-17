// Librarian.h

#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include "IssueDetails.h"
#include "Student.h"
#include "Book.h"         // For accessing Book
#include "AudioTape.h"    // For accessing AudioTape
#include "Article.h"      // For accessing Article

class Librarian {
private:
    char libID[6];   // Librarian ID (e.g., "L0010")
    char name[20];   // Librarian Name

public:
    // Default constructor initializes members to default values
    Librarian();

    // Parameterized constructor for initializing librarian details
    Librarian(const char* id, const char* libName);

    // Function to display librarian details
    void showLibrarian() const;

    // Function to approve or reject issue request
    bool approveIssueRequest(const char* admno, const char* resourceID, IssueDetails& issueDetails);

    // Function to modify librarian details
    void modifyLibrarian();

    // Overload output operator for easy display
    friend std::ostream& operator<<(std::ostream& os, const Librarian& librarian);
};

#endif // LIBRARIAN_H
