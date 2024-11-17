// src/Article.cpp

#include "Article.h"

// Default constructor implementation
Article::Article() {
    std::strcpy(ano, "");
    std::strcpy(title, "");
    std::strcpy(author, "");
    std::strcpy(journal, "");
    year = 0;
}

// Display article details
void Article::showArticle() const {
    std::cout << "\nArticle Number: " << ano;
    std::cout << "\nTitle: " << title;
    std::cout << "\nAuthor: " << author;
    std::cout << "\nJournal: " << journal;
    std::cout << "\nPublication Year: " << year;
}

// Modify existing article details
void Article::modifyArticle() {
    std::cout << "\nModify Article Title: ";
    std::cin.ignore(); // To ignore any leftover newline character
    std::cin.getline(title, 100);

    std::cout << "Modify Author's Name: ";
    std::cin.getline(author, 50);

    std::cout << "Modify Journal Name: ";
    std::cin.getline(journal, 50);

    std::cout << "Modify Publication Year: ";
    std::cin >> year;
}

// Display article information in a formatted way
void Article::report() const {
    std::cout << std::left << std::setw(10) << ano
              << std::setw(30) << title
              << std::setw(25) << author
              << std::setw(25) << journal
              << std::setw(6)  << year << std::endl;
}

// Overload the output operator
std::ostream& operator<<(std::ostream &os, const Article &article) {
    os << "Article No: " << article.ano
       << ", Title: " << article.title
       << ", Author: " << article.author
       << ", Journal: " << article.journal
       << ", Year: " << article.year;
    return os;
}
