#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <limits> // For std::numeric_limits

// Base class for all library items
class LibraryItem {
protected:
    std::string itemNumber;
    std::string title;
    std::string creator;

public:
    LibraryItem(const std::string& number, const std::string& itemTitle, const std::string& itemCreator)
        : itemNumber(number), title(itemTitle), creator(itemCreator) {}

    virtual void showDetails() const {
        std::cout << "Item Number: " << itemNumber << ", Title: " << title << ", Creator: " << creator << "\n";
    }

    std::string getItemNumber() const { // Public accessor method
        return itemNumber;
    }

    virtual ~LibraryItem() {}
};

// Derived class for books
class Book : public LibraryItem {
private:
    int pages;

public:
    Book(const std::string& number, const std::string& bookTitle, const std::string& author, int numPages)
        : LibraryItem(number, bookTitle, author), pages(numPages) {}

    void showDetails() const override {
        std::cout << "Book Number: " << itemNumber << ", Title: " << title
                  << ", Author: " << creator << ", Pages: " << pages << "\n";
    }
};

// Derived class for magazines
class Magazine : public LibraryItem {
private:
    int issueNumber;

public:
    Magazine(const std::string& number, const std::string& magazineTitle, const std::string& publisher, int issue)
        : LibraryItem(number, magazineTitle, publisher), issueNumber(issue) {}

    void showDetails() const override {
        std::cout << "Magazine Number: " << itemNumber << ", Title: " << title
                  << ", Publisher: " << creator << ", Issue Number: " << issueNumber << "\n";
    }
};

// Class to represent a borrowed item and its due date
class BorrowedItem {
public:
    std::string itemNumber;
    std::string dueDate;

    BorrowedItem(const std::string& number, const std::string& due)
        : itemNumber(number), dueDate(due) {}
};

// Class to represent a student
class Student {
private:
    std::string name;
    std::string admissionNumber;
    std::vector<BorrowedItem> borrowedItems;

public:
    Student(const std::string& admNo, const std::string& studentName)
        : admissionNumber(admNo), name(studentName) {}

    bool canBorrowMore() const {
        return borrowedItems.size() < 3 && calculateFine() < 100;
    }

    void borrowItem(const std::string& itemNumber, const std::string& dueDate) {
        if (calculateFine() >= 100) {
            std::cout << "You cannot borrow a new item because your fine is Rs " 
                      << calculateFine() << ". Please pay your fine first.\n";
            return;
        }

        if (borrowedItems.size() < 3) {
            borrowedItems.emplace_back(itemNumber, dueDate);
            std::cout << "Item " << itemNumber << " borrowed successfully! Due Date: " << dueDate << "\n";
        } else {
            std::cout << "You have already borrowed the maximum number of items!\n";
        }
    }

    void showBorrowedItems() const {
        if (borrowedItems.empty()) {
            std::cout << "No items borrowed yet.\n";
            return;
        }

        std::cout << "Borrowed Items:\n";
        for (const auto& item : borrowedItems) {
            std::cout << "Item Number: " << item.itemNumber 
                      << ", Due Date: " << item.dueDate << "\n";
        }
    }

    double calculateFine() const {
        double totalFine = 0.0;
        std::time_t now = std::time(nullptr);

        for (const auto& item : borrowedItems) {
            std::tm dueTm = {};
            std::istringstream ss(item.dueDate);
            ss >> std::get_time(&dueTm, "%Y-%m-%d");

            if (!ss.fail()) {
                std::time_t dueTime = std::mktime(&dueTm);
                if (now > dueTime) {
                    int overdueDays = (now - dueTime) / (24 * 60 * 60);
                    totalFine += overdueDays * 20;
                }
            }
        }
        return totalFine;
    }

    void payFine() {
        double fine = calculateFine();
        if (fine > 0) {
            std::cout << "Your total fine is Rs " << fine << ". Please enter payment amount: ";
            double payment;
            std::cin >> payment;

            if (payment >= fine) {
                std::cout << "Fine paid successfully. Thank you!\n";
            } else {
                std::cout << "Insufficient payment! Remaining fine: Rs " << (fine - payment) << "\n";
            }
        } else {
            std::cout << "No fines to pay.\n";
        }
    }
};

// Utility function to calculate due date (30 days from today)
std::string calculateDueDate(int days = 30) {
    std::time_t now = std::time(nullptr);
    std::tm dueDate = *std::localtime(&now);
    dueDate.tm_mday += days;
    std::mktime(&dueDate); // Normalize the date structure
    std::ostringstream oss;
    oss << std::put_time(&dueDate, "%Y-%m-%d");
    return oss.str();
}

// Function to borrow an item
void borrowItem(Student& student, std::vector<LibraryItem*>& libraryItems) {
    if (!student.canBorrowMore()) {
        std::cout << "You cannot borrow more items. Either you have an outstanding fine of Rs 100 or more, or you've reached the borrowing limit.\n";
        return;
    }

    // Display available library items
    std::cout << "Available Library Items:\n";
    for (size_t i = 0; i < libraryItems.size(); ++i) {
        std::cout << i + 1 << ". ";
        libraryItems[i]->showDetails();
    }

    // Borrowing logic
    std::cout << "Enter the item number to borrow: ";
    int choice;
    std::cin >> choice;

    // Validate input
    if (std::cin.fail() || choice <= 0 || choice > static_cast<int>(libraryItems.size())) {
        std::cout << "Invalid choice! Please try again.\n";

        // Clear the error state and ignore the remaining input
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
        return;
    }

    // Borrow the selected item
    std::string dueDate = calculateDueDate();
    student.borrowItem(libraryItems[choice - 1]->getItemNumber(), dueDate);

    // Remove the borrowed item from the library
    delete libraryItems[choice - 1];
    libraryItems.erase(libraryItems.begin() + choice - 1);
}

// Main program
int main() {
    // Create library items
    std::vector<LibraryItem*> libraryItems;
    libraryItems.push_back(new Book("B001", "The Great Gatsby", "F. Scott Fitzgerald", 180));
    libraryItems.push_back(new Magazine("M001", "National Geographic", "NatGeo Society", 2023));

    // Create a student
    Student student("S001", "John Doe");

    bool running = true;
    while (running) {
        std::cout << "\nLibrary Menu:\n";
        std::cout << "1. Show Available Items\n";
        std::cout << "2. Borrow an Item\n";
        std::cout << "3. Show Borrowed Items\n";
        std::cout << "4. Calculate and Pay Fine\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                for (const auto& item : libraryItems) {
                    item->showDetails();
                }
                break;
            case 2:
                borrowItem(student, libraryItems);
                break;
            case 3:
                student.showBorrowedItems();
                break;
            case 4:
                student.payFine();
                break;
            case 5:
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    // Clean up
    for (auto& item : libraryItems) {
        delete item;
    }
    libraryItems.clear();

    std::cout << "Thank you for using the Library Management System!\n";
    return 0;
}