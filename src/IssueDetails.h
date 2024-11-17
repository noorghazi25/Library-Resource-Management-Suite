// IssueDetails.h

#ifndef ISSUEDETAILS_H
#define ISSUEDETAILS_H

#include "Issue.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>

class IssueDetails {
private:
    static const int MAX_ISSUES = 1000; // Maximum number of issues
    Issue issues[MAX_ISSUES];           // Array to store Issue objects
    int issueCount;                     // Current number of issues

public:
    // Constructor
    IssueDetails();

    // Function to add a new issue to the history
    bool addIssue(const Issue& issue);

    // Function to display all issues
    void showAllIssues() const;

    // Function to display issues by student admission number
    void showIssuesByStudent(const char* admno) const;

    // Function to display issues by resource ID
    void showIssuesByResource(const char* resID) const;

    // Function to delete an issue by issue ID
    bool deleteIssue(const char* issueID);

    // Function to calculate total fine for a student
    double calculateTotalFine(const char* admno) const;

    // Function to display all fines
    void showAllFines() const;

    // Function to save issues to a file (persistent storage)
    bool saveIssuesToFile(const char* filename) const;

    // Function to load issues from a file
    bool loadIssuesFromFile(const char* filename);

    // Function to update issue status and fine
    bool updateIssueStatus(const char* issueID, const char* newStatus, double newFine);

    // Function to check if a resource is available
    bool isResourceAvailable(const char* resID) const;

    // Overloaded output operator to display all issues
    friend std::ostream& operator<<(std::ostream& os, const IssueDetails& issueDetails);
};

#endif // ISSUEDETAILS_H
