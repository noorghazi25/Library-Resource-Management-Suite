// IssueDetails.cpp

#include "IssueDetails.h"

// Constructor initializes issueCount to 0
IssueDetails::IssueDetails() : issueCount(0) {}

// Function to add a new issue to the history
bool IssueDetails::addIssue(const Issue& issue) {
    if(issueCount >= MAX_ISSUES) {
        std::cerr << "Error: Issue history is full. Cannot add more issues.\n";
        return false;
    }
    issues[issueCount++] = issue;
    return true;
}

// Function to display all issues
void IssueDetails::showAllIssues() const {
    if(issueCount == 0) {
        std::cout << "No issues recorded.\n";
        return;
    }
    for(int i = 0; i < issueCount; ++i) {
        issues[i].showIssue();
        std::cout << "---------------------------\n";
    }
}

// Function to display issues by student admission number
void IssueDetails::showIssuesByStudent(const char* admno) const {
    bool found = false;
    for(int i = 0; i < issueCount; ++i) {
        if(std::strcmp(issues[i].studentAdmNo, admno) == 0) {
            issues[i].showIssue();
            std::cout << "---------------------------\n";
            found = true;
        }
    }
    if(!found) {
        std::cout << "No issues found for Student Admission No: " << admno << "\n";
    }
}

// Function to display issues by resource ID
void IssueDetails::showIssuesByResource(const char* resID) const {
    bool found = false;
    for(int i = 0; i < issueCount; ++i) {
        if(std::strcmp(issues[i].resourceID, resID) == 0) {
            issues[i].showIssue();
            std::cout << "---------------------------\n";
            found = true;
        }
    }
    if(!found) {
        std::cout << "No issues found for Resource ID: " << resID << "\n";
    }
}

// Function to delete an issue by issue ID
bool IssueDetails::deleteIssue(const char* issueID) {
    for(int i = 0; i < issueCount; ++i) {
        if(std::strcmp(issues[i].issueID, issueID) == 0) {
            // Shift all subsequent issues up by one
            for(int j = i; j < issueCount -1; ++j) {
                issues[j] = issues[j+1];
            }
            --issueCount;
            std::cout << "Issue ID: " << issueID << " has been deleted.\n";
            return true;
        }
    }
    std::cout << "Issue ID: " << issueID << " not found.\n";
    return false;
}

// Function to calculate total fine for a student
double IssueDetails::calculateTotalFine(const char* admno) const {
    double total = 0.0;
    for(int i = 0; i < issueCount; ++i) {
        if(std::strcmp(issues[i].studentAdmNo, admno) == 0) {
            total += issues[i].totalFine;
        }
    }
    return total;
}

// Function to display all fines
void IssueDetails::showAllFines() const {
    if(issueCount == 0) {
        std::cout << "No issues recorded.\n";
        return;
    }
    std::cout << std::left << std::setw(10) << "IssueID" 
              << std::setw(10) << "AdmNo" 
              << std::setw(10) << "ResID" 
              << std::setw(12) << "IssueDate" 
              << std::setw(12) << "DueDate" 
              << std::setw(10) << "Status" 
              << std::setw(10) << "Fine" << "\n";
    for(int i = 0; i < issueCount; ++i) {
        std::cout << std::left << std::setw(10) << issues[i].issueID
                  << std::setw(10) << issues[i].studentAdmNo
                  << std::setw(10) << issues[i].resourceID
                  << std::setw(12) << issues[i].dateOfIssue
                  << std::setw(12) << issues[i].dueDate
                  << std::setw(10) << issues[i].status
                  << "$" << std::setw(9) << issues[i].totalFine << "\n";
    }
}

// Function to save issues to a file (persistent storage)
bool IssueDetails::saveIssuesToFile(const char* filename) const {
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    if(!ofs) {
        std::cerr << "Error: Unable to open file for writing: " << filename << "\n";
        return false;
    }
    ofs.write(reinterpret_cast<const char*>(&issueCount), sizeof(issueCount));
    ofs.write(reinterpret_cast<const char*>(issues), sizeof(Issue) * issueCount);
    ofs.close();
    return true;
}

// Function to load issues from a file
bool IssueDetails::loadIssuesFromFile(const char* filename) {
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    if(!ifs) {
        std::cerr << "Error: Unable to open file for reading: " << filename << "\n";
        return false;
    }
    ifs.read(reinterpret_cast<char*>(&issueCount), sizeof(issueCount));
    if(issueCount > MAX_ISSUES) {
        std::cerr << "Error: Issue count in file exceeds maximum limit.\n";
        issueCount = 0;
        return false;
    }
    ifs.read(reinterpret_cast<char*>(issues), sizeof(Issue) * issueCount);
    ifs.close();
    return true;
}

// Function to update issue status and fine
bool IssueDetails::updateIssueStatus(const char* issueID, const char* newStatus, double newFine) {
    for(int i = 0; i < issueCount; ++i) {
        if(std::strcmp(issues[i].issueID, issueID) == 0) {
            std::strcpy(issues[i].status, newStatus);
            issues[i].totalFine = newFine;
            std::cout << "Issue ID: " << issueID << " has been updated.\n";
            return true;
        }
    }
    std::cout << "Issue ID: " << issueID << " not found.\n";
    return false;
}

// Function to check if a resource is available
bool IssueDetails::isResourceAvailable(const char* resID) const {
    for(int i = 0; i < issueCount; ++i) {
        if(std::strcmp(issues[i].resourceID, resID) == 0 &&
           std::strcmp(issues[i].status, "Taken") == 0) {
            return false; // Resource is currently taken
        }
    }
    return true; // Resource is available
}

// Overloaded output operator to display all issues
std::ostream& operator<<(std::ostream& os, const IssueDetails& issueDetails) {
    for(int i = 0; i < issueDetails.issueCount; ++i) {
        os << issueDetails.issues[i] << "\n";
    }
    return os;
}
