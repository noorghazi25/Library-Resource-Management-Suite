// Issue.h

#ifndef ISSUE_H
#define ISSUE_H

#include <iostream>
#include <iomanip>
#include <cstring>

class Issue {
public:
    char issueID[6];        // Issue ID (e.g., "I0010")
    char studentAdmNo[6];   // Student Admission Number (e.g., "S0010")
    char resourceID[6];     // Resource ID (Book/AudioTape/Article Number)
    char dateOfIssue[11];   // Date of Issue (YYYY-MM-DD)
    char dueDate[11];       // Due Date for Return (YYYY-MM-DD)
    char status[10];        // Status: "Available" or "Taken"
    double totalFine;       // Total Fine

    // Default constructor initializes members to default values
    Issue();

    // Parameterized constructor for initializing issue details
    Issue(const char* id, const char* admNo, const char* resID, const char* issueDate, const char* returnDate, const char* stat, double fine);

    // Function to display issue details
    void showIssue() const;

    // Function to modify existing issue details
    void modifyIssue();

    // Function to display issue information in a formatted way
    void report() const;

    // Function to calculate fine based on days overdue (arbitrary formula)
    void calculateFine(int daysOverdue);

    // Overload the output operator for easy display
    friend std::ostream& operator<<(std::ostream &os, const Issue &issue);
};

#endif // ISSUE_H
