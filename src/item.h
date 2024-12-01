#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <iomanip>
#include <cstring>

class Item {
protected:
    char id[6]; // Common ID field for items

public:
    // Default constructor initializes ID to an empty string
    Item() {
        std::strcpy(id, "");
    }

    // Parameterized constructor for setting ID
    Item(const char* itemId) {
        std::strcpy(id, itemId);
    }

    // Virtual destructor
    virtual ~Item() {}

    // Pure virtual function for displaying details (to be implemented by derived classes)
    virtual void showDetails() const = 0;

    // Pure virtual function for modifying details (to be implemented by derived classes)
    virtual void modifyDetails() = 0;

    // Pure virtual function for reporting details in a formatted way
    virtual void report() const = 0;

    // Accessor for ID
    const char* getId() const {
        return id;
    }

    // Mutator for ID
    void setId(const char* newId) {
        std::strcpy(id, newId);
    }
};

#endif // ITEM_H
