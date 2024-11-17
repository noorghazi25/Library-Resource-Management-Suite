// include/Article.h

#ifndef ARTICLE_H
#define ARTICLE_H

#include <iostream>
#include <iomanip>
#include <cstring>

class Article {
public:
    char ano[6];        // Article Number (e.g., "A0010")
    char title[100];    // Article Title
    char author[50];    // Author Name
    char journal[50];   // Journal Name
    int year;           // Publication Year

    // Default constructor initializes members to default values
    Article();

    // Function to display article details
    void showArticle() const;

    // Function to modify existing article details
    void modifyArticle();

    // Function to display article information in a formatted way
    void report() const;

    // Overload the output operator for easy display
    friend std::ostream& operator<<(std::ostream &os, const Article &article);
};

#endif // ARTICLE_H
