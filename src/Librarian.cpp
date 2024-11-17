// Librarian.cpp

#include "Librarian.h"
#include <ctime>

// Static variable to keep track of the next issue number
static int nextIssueNumber = 1;

// Function to generate a unique issue ID
std::string generateIssueID() {
    std::string issueID = "I";
    if(nextIssueNumber < 10)
        issueID += "000" + std::to_string(nextIssueNumber);
    else if(nextIssueNumber < 100)
        issueID += "00" + std::to_string(nextIssueNumber);
    else if(nextIssueNumber < 1000)
        issueID += "0" + std::to_string(nextIssueNumber);
    else
        issueID += std::to_string(nextIssueNumber); // Assuming not exceeding 9999

    nextIssueNumber++;
    return issueID;
}

// Default constructor
Librarian::Librarian() {
    std::strcpy(libID, "");
    std::strcpy(name, "");
}

// Parameterized constructor
Librarian::Librarian(const char* id, const char* libName) {
    std::strcpy(libID, id);
    std::strcpy(name, libName);
}

// Function to display librarian details
void Librarian::showLibrarian() const {
    std::cout << "Librarian ID: " << libID << "\n"
              << "Librarian Name: " << name << "\n";
}

// Function to approve or reject issue request
bool Librarian::approveIssueRequest(const char* admno, const char* resourceID, IssueDetails& issueDetails) {
    // Check if the student has any fines
    double totalFine = issueDetails.calculateTotalFine(admno);
    if(totalFine > 0.0) {
        std::cout << "Cannot approve issue request. Student has total fines: $" << totalFine << "\n";
        return false;
    }

    // Check if the resource is available
    if(!issueDetails.isResourceAvailable(resourceID)) {
        std::cout << "Cannot approve issue request. Resource ID " << resourceID << " is currently taken.\n";
        return false;
    }

    // Generate a unique issue ID
    std::string newIssueID = generateIssueID();

    // Get current date
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    char dateOfIssue[11];
    char dueDate[11];
    std::strftime(dateOfIssue, sizeof(dateOfIssue), "%Y-%m-%d", now);

    // Calculate due date, e.g., 14 days from issue date
    std::tm dueTm = *now;
    dueTm.tm_mday += 14;
    std::mktime(&dueTm); // Normalize the date
    std::strftime(dueDate, sizeof(dueDate), "%Y-%m-%d", &dueTm);

    // Create a new Issue object
    Issue newIssue;
    std::strcpy(newIssue.issueID, newIssueID.c_str());
    std::strcpy(newIssue.studentAdmNo, admno);
    std::strcpy(newIssue.resourceID, resourceID);
    std::strcpy(newIssue.dateOfIssue, dateOfIssue);
    std::strcpy(newIssue.dueDate, dueDate);
    std::strcpy(newIssue.status, "Taken");
    newIssue.totalFine = 0.0;

    // Add the new issue to IssueDetails
    if(issueDetails.addIssue(newIssue)) {
        std::cout << "Issue request approved. Issue ID: " << newIssue.issueID << "\n";
        return true;
    } else {
        std::cout << "Failed to add issue to history.\n";
        return false;
    }
}

// Function to modify librarian details
void Librarian::modifyLibrarian() {
    std::cout << "Modify Librarian ID: ";
    std::cin.ignore(); // To ignore any leftover newline character
    std::cin.getline(libID, 6);

    std::cout << "Modify Librarian Name: ";
    std::cin.getline(name, 20);
}

// Overload the output operator
std::ostream& operator<<(std::ostream& os, const Librarian& librarian) {
    os << "Librarian ID: " << librarian.libID << ", Name: " << librarian.name;
    return os;
}
