// Issue.cpp

#include "Issue.h"

// Default constructor implementation
Issue::Issue() {
    std::strcpy(issueID, "");
    std::strcpy(studentAdmNo, "");
    std::strcpy(resourceID, "");
    std::strcpy(dateOfIssue, "");
    std::strcpy(dueDate, "");
    std::strcpy(status, "Available");
    totalFine = 0.0;
}

// Parameterized constructor implementation
Issue::Issue(const char* id, const char* admNo, const char* resID, const char* issueDate, const char* returnDate, const char* stat, double fine) {
    std::strcpy(issueID, id);
    std::strcpy(studentAdmNo, admNo);
    std::strcpy(resourceID, resID);
    std::strcpy(dateOfIssue, issueDate);
    std::strcpy(dueDate, returnDate);
    std::strcpy(status, stat);
    totalFine = fine;
}

// Display issue details
void Issue::showIssue() const {
    std::cout << "\nIssue ID: " << issueID;
    std::cout << "\nStudent Admission No: " << studentAdmNo;
    std::cout << "\nResource ID: " << resourceID;
    std::cout << "\nDate of Issue: " << dateOfIssue;
    std::cout << "\nDue Date: " << dueDate;
    std::cout << "\nStatus: " << status;
    std::cout << "\nTotal Fine: $" << totalFine;
}

// Modify existing issue details
void Issue::modifyIssue() {
    std::cout << "\nModify Issue ID: ";
    std::cin.ignore(); // To ignore any leftover newline character
    std::cin.getline(issueID, 6);

    std::cout << "Modify Student Admission No: ";
    std::cin.getline(studentAdmNo, 6);

    std::cout << "Modify Resource ID: ";
    std::cin.getline(resourceID, 6);

    std::cout << "Modify Date of Issue (YYYY-MM-DD): ";
    std::cin.getline(dateOfIssue, 11);

    std::cout << "Modify Due Date (YYYY-MM-DD): ";
    std::cin.getline(dueDate, 11);

    std::cout << "Modify Status (Available/Taken): ";
    std::cin.getline(status, 10);

    std::cout << "Modify Total Fine: ";
    std::cin >> totalFine;
}

// Display issue information in a formatted way
void Issue::report() const {
    std::cout << std::left << std::setw(10) << issueID
              << std::setw(10) << studentAdmNo
              << std::setw(10) << resourceID
              << std::setw(12) << dateOfIssue
              << std::setw(12) << dueDate
              << std::setw(10) << status
              << "$" << std::setw(8) << totalFine << std::endl;
}

// Calculate fine based on days overdue (arbitrary formula)
void Issue::calculateFine(int daysOverdue) {
    const double finePerDay = 1.0; // $1 per day overdue
    if (daysOverdue > 0) {
        totalFine += daysOverdue * finePerDay;
        std::strcpy(status, "Taken"); // Assuming the book is still taken if overdue
    }
}

// Overload the output operator
std::ostream& operator<<(std::ostream &os, const Issue &issue) {
    os << "Issue ID: " << issue.issueID
       << ", Student Admission No: " << issue.studentAdmNo
       << ", Resource ID: " << issue.resourceID
       << ", Date of Issue: " << issue.dateOfIssue
       << ", Due Date: " << issue.dueDate
       << ", Status: " << issue.status
       << ", Total Fine: $" << issue.totalFine;
    return os;
}
