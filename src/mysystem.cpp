#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring> // For strlen()
#include <iomanip> // For std::setw
#include <algorithm> // For std::transform
#include <cctype>    // For std::tolower
#include "Button.h"
#include "Book.h"
#include "Student.h"

// For case-insensitive string comparison
#ifdef _WIN32
    #define strcmpi _stricmp
#else
    #define strcmpi strcasecmp
#endif

// Enum to represent different application states
enum AppState {
    WELCOME,
    MAIN_MENU,
    ADMIN_MENU,
    CREATE_BOOK,
    SHOW_BOOKS,
    MODIFY_BOOK,
    GENERATE_REPORT,
    EXIT_APP
};

// Helper function to trim whitespace from a string
std::string trim(const std::string& str) {
    if(str.empty()) return str;
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

// Helper function to read all books from a text file
std::vector<Book> readAllBooks() {
    std::vector<Book> books;
    std::ifstream infile("book.txt");
    if (!infile) {
        // File might not exist initially
        return books;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if(line.empty()) continue; // Skip empty lines

        std::istringstream iss(line);
        Book temp;
        std::string bno, bname, aname;

        if (std::getline(iss, bno, ',') && 
            std::getline(iss, bname, ',') && 
            std::getline(iss, aname)) { // Read until the end of the line
            // Trim whitespace
            bno = trim(bno);
            bname = trim(bname);
            aname = trim(aname);

            // Ensure that strings do not exceed buffer sizes
            std::strncpy(temp.bno, bno.c_str(), sizeof(temp.bno) - 1);
            temp.bno[sizeof(temp.bno) - 1] = '\0';

            std::strncpy(temp.bname, bname.c_str(), sizeof(temp.bname) - 1);
            temp.bname[sizeof(temp.bname) - 1] = '\0';

            std::strncpy(temp.aname, aname.c_str(), sizeof(temp.aname) - 1);
            temp.aname[sizeof(temp.aname) - 1] = '\0';

            books.push_back(temp);
        }
    }

    infile.close();
    return books;
}

// Helper function to write all books to a text file
void writeAllBooks(const std::vector<Book>& books) {
    std::ofstream outfile("book.txt", std::ios::trunc); // Overwrite the file
    if (!outfile) {
        std::cerr << "Error opening book.txt for writing.\n";
        return;
    }

    for (const auto& book : books) {
        outfile << book.bno << "," << book.bname << "," << book.aname << "\n";
    }

    outfile.close();
}

// Helper function to draw text with a semi-transparent background
void drawTextWithBackground(sf::RenderWindow& window, sf::Text& text, sf::Color bgColor = sf::Color(0, 0, 0, 150)) {
    sf::FloatRect textBounds = text.getGlobalBounds();

    sf::RectangleShape backgroundRect;
    backgroundRect.setPosition(textBounds.left - 5, textBounds.top - 5);
    backgroundRect.setSize(sf::Vector2f(textBounds.width + 10, textBounds.height + 10));
    backgroundRect.setFillColor(bgColor);

    window.draw(backgroundRect);
    window.draw(text);
}

// Function to make a string lowercase (for case-insensitive comparison)
std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(str.begin(), str.end(), lowerStr.begin(), [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}

int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Library Management System");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Failed to load font!\n";
        return -1;
    }

    // Load background image
    sf::Texture bgTexture;
    sf::Sprite background;
    if(bgTexture.loadFromFile("library.png")) { // Ensure correct path
        background.setTexture(bgTexture);
        background.setScale(
            800.0f / background.getLocalBounds().width,
            600.0f / background.getLocalBounds().height
        );
    } else {
        std::cout << "Failed to load background image!\n";
    }

    // **Add background music loading here**
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("Game of Thrones.wav")) {
        std::cout << "Failed to load background music!" << std::endl;
        // Handle error or decide to continue without music
    } else {
        backgroundMusic.setLoop(true); // Loop the music
        backgroundMusic.play();        // Start the music
    }

    // Initialize texts for Welcome Screen
    sf::Text welcomeText("Welcome to the Library Resource Management Suite", font, 25);
    welcomeText.setFillColor(sf::Color::Cyan);
    welcomeText.setPosition(100, 200);
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setOutlineThickness(1);
    welcomeText.setOutlineColor(sf::Color::Black);

    sf::Text startText("Click anywhere to Continue", font, 20);
    startText.setFillColor(sf::Color::Yellow);
    startText.setPosition(300, 300);
    startText.setStyle(sf::Text::Bold);
    startText.setOutlineThickness(1);
    startText.setOutlineColor(sf::Color::Black);

    // Initialize buttons for Main Menu
    Button adminButton(300, 200, 200, 50, font, "Administrator Menu", sf::Color::Cyan, sf::Color::Green, sf::Color::Blue);
    Button exitButton(300, 300, 200, 50, font, "Exit", sf::Color::Cyan, sf::Color::Green, sf::Color::Red);

    // Initialize buttons for Admin Menu
    Button createBookButton(100, 150, 200, 50, font, "Create a Book", sf::Color::Cyan, sf::Color::Green, sf::Color::Blue);
    Button showBooksButton(100, 220, 200, 50, font, "Show All Books", sf::Color::Cyan, sf::Color::Green, sf::Color::Blue);
    Button modifyBookButton(100, 290, 200, 50, font, "Modify a Book", sf::Color::Cyan, sf::Color::Green, sf::Color::Blue);
    Button generateReportButton(100, 360, 200, 50, font, "Generate Report", sf::Color::Cyan, sf::Color::Green, sf::Color::Blue);
    Button backToMainMenuButton(300, 500, 200, 50, font, "Back to Main Menu", sf::Color::Cyan, sf::Color::Green, sf::Color::Red);

    // Initialize buttons for Create Book
    Button backButtonCreateBook(650, 500, 100, 40, font, "Back", sf::Color::Red, sf::Color::Yellow, sf::Color::Green);

    // Initialize buttons for Show Books
    Button backButtonShowBooks(650, 500, 100, 40, font, "Back", sf::Color::Red, sf::Color::Yellow, sf::Color::Green);

    // Initialize buttons for Modify Book
    Button backButtonModifyBook(650, 500, 100, 40, font, "Back", sf::Color::Red, sf::Color::Yellow, sf::Color::Green);
    Button submitModifyBookButton(300, 500, 200, 50, font, "Submit Changes", sf::Color::Cyan, sf::Color::Green, sf::Color::Blue);

    // Initialize buttons for Generate Report
    Button backButtonGenerateReport(650, 500, 100, 40, font, "Back", sf::Color::Red, sf::Color::Yellow, sf::Color::Green);

    // Application state
    AppState state = WELCOME;

    // Variables for Create Book
    std::string bookNo = "";
    std::string bookName = "";
    std::string authorName = "";
    int activeField = 0; // 0: Book No, 1: Book Name, 2: Author Name

    // Input field rectangles for better visibility
    sf::RectangleShape inputBox1(sf::Vector2f(200, 30));
    inputBox1.setPosition(320, 150);
    inputBox1.setFillColor(sf::Color::Black);
    inputBox1.setOutlineThickness(2);
    inputBox1.setOutlineColor(sf::Color::White);

    sf::RectangleShape inputBox2(sf::Vector2f(200, 30));
    inputBox2.setPosition(320, 220);
    inputBox2.setFillColor(sf::Color::Black);
    inputBox2.setOutlineThickness(2);
    inputBox2.setOutlineColor(sf::Color::White);

    sf::RectangleShape inputBox3(sf::Vector2f(200, 30));
    inputBox3.setPosition(320, 290);
    inputBox3.setFillColor(sf::Color::Black);
    inputBox3.setOutlineThickness(2);
    inputBox3.setOutlineColor(sf::Color::White);

    // Variables for Modify Book
    std::vector<Book> books = readAllBooks();
    std::string searchBno = "";
    bool bookFound = false;
    Book selectedBook;
    int modifyActiveField = -1; // -1: No active field, 0: Search, 1: Book Name, 2: Author Name

    // Input field rectangles for Modify Book
    sf::RectangleShape modifyInputBox1(sf::Vector2f(200, 30));
    modifyInputBox1.setPosition(320, 300);
    modifyInputBox1.setFillColor(sf::Color::Black);
    modifyInputBox1.setOutlineThickness(2);
    modifyInputBox1.setOutlineColor(sf::Color::White);

    sf::RectangleShape modifyInputBox2(sf::Vector2f(200, 30));
    modifyInputBox2.setPosition(320, 370);
    modifyInputBox2.setFillColor(sf::Color::Black);
    modifyInputBox2.setOutlineThickness(2);
    modifyInputBox2.setOutlineColor(sf::Color::White);

    // Main loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle key presses based on state
            if (event.type == sf::Event::KeyPressed) {
                // Exit application
                if(state == EXIT_APP){
                    window.close();
                }

                // Handle Escape key in various states
                if((state == CREATE_BOOK || state == MODIFY_BOOK || state == SHOW_BOOKS || state == GENERATE_REPORT) 
                   && event.key.code == sf::Keyboard::Escape){
                    state = ADMIN_MENU;
                    // Reset input fields
                    bookNo = "";
                    bookName = "";
                    authorName = "";
                    activeField = 0;

                    // Reset modify book variables
                    searchBno = "";
                    bookFound = false;
                    selectedBook = Book();
                    modifyActiveField = -1;
                    std::cout << "Transition back to ADMIN_MENU\n";
                }
            }

            // Handle mouse clicks and text input based on state
            switch(state){
                case WELCOME:
                    if (event.type == sf::Event::MouseButtonPressed){
                        if(event.mouseButton.button == sf::Mouse::Left){
                            state = MAIN_MENU;
                            std::cout << "Transition to MAIN_MENU\n";
                        }
                    }
                    break;
                case MAIN_MENU:
                    // Handle button clicks
                    if(event.type == sf::Event::MouseButtonPressed){
                        sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        if(adminButton.isClicked(mousePosF)){
                            state = ADMIN_MENU;
                            std::cout << "Transition to ADMIN_MENU\n";
                        }
                        if(exitButton.isClicked(mousePosF)){
                            state = EXIT_APP;
                            std::cout << "Transition to EXIT_APP\n";
                        }
                    }
                    break;
                case ADMIN_MENU:
                    // Handle button clicks
                    if(event.type == sf::Event::MouseButtonPressed){
                        sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        if(createBookButton.isClicked(mousePosF)){
                            state = CREATE_BOOK;
                            std::cout << "Transition to CREATE_BOOK\n";
                        }
                        if(showBooksButton.isClicked(mousePosF)){
                            state = SHOW_BOOKS;
                            std::cout << "Transition to SHOW_BOOKS\n";
                        }
                        if(modifyBookButton.isClicked(mousePosF)){
                            state = MODIFY_BOOK;
                            std::cout << "Transition to MODIFY_BOOK\n";
                        }
                        if(generateReportButton.isClicked(mousePosF)){
                            state = GENERATE_REPORT;
                            std::cout << "Transition to GENERATE_REPORT\n";
                        }
                        if(backToMainMenuButton.isClicked(mousePosF)){
                            state = MAIN_MENU;
                            std::cout << "Transition back to MAIN_MENU\n";
                        }
                    }
                    break;
                case CREATE_BOOK:
                    // Handle mouse clicks for input fields
                    if(event.type == sf::Event::MouseButtonPressed){
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);
                        // Check which field is clicked
                        if(inputBox1.getGlobalBounds().contains(mousePosF)){
                            activeField = 0;
                            std::cout << "Active Field: Book No\n";
                        }
                        else if(inputBox2.getGlobalBounds().contains(mousePosF)){
                            activeField = 1;
                            std::cout << "Active Field: Book Name\n";
                        }
                        else if(inputBox3.getGlobalBounds().contains(mousePosF)){
                            activeField = 2;
                            std::cout << "Active Field: Author Name\n";
                        }
                        else{
                            activeField = -1; // No active field
                        }
                    }

                    // Handle text input
                    if(event.type == sf::Event::TextEntered){
                        if(state == CREATE_BOOK){
                            if(activeField == 0){
                                if(event.text.unicode == '\b'){ // Handle backspace
                                    if(!bookNo.empty()){
                                        bookNo.pop_back();
                                    }
                                }
                                else if(event.text.unicode < 128 && bookNo.size() < 5){
                                    bookNo += static_cast<char>(event.text.unicode);
                                }
                            }
                            else if(activeField == 1){
                                if(event.text.unicode == '\b'){ // Handle backspace
                                    if(!bookName.empty()){
                                        bookName.pop_back();
                                    }
                                }
                                else if(event.text.unicode < 128 && bookName.size() < 49){
                                    bookName += static_cast<char>(event.text.unicode);
                                }
                            }
                            else if(activeField == 2){
                                if(event.text.unicode == '\b'){ // Handle backspace
                                    if(!authorName.empty()){
                                        authorName.pop_back();
                                    }
                                }
                                else if(event.text.unicode < 128 && authorName.size() < 19){
                                    authorName += static_cast<char>(event.text.unicode);
                                }
                            }

                            // Handle Enter key to submit
                            if(event.text.unicode == '\r'){
                                if(bookNo.empty() || bookName.empty() || authorName.empty()){
                                    std::cout << "All fields are required!\n";
                                }
                                else{
                                    // Check for duplicate Book No
                                    bool duplicate = false;
                                    for(const auto& book : books){
                                        if(strcmpi(trim(book.bno).c_str(), trim(bookNo).c_str()) == 0){
                                            duplicate = true;
                                            break;
                                        }
                                    }
                                    if(duplicate){
                                        std::cout << "Book Number already exists!\n";
                                    }
                                    else{
                                        // Create Book object and add to vector
                                        Book newBook;
                                        std::strncpy(newBook.bno, bookNo.c_str(), sizeof(newBook.bno) - 1);
                                        newBook.bno[sizeof(newBook.bno) - 1] = '\0';
                                        std::strncpy(newBook.bname, bookName.c_str(), sizeof(newBook.bname) - 1);
                                        newBook.bname[sizeof(newBook.bname) - 1] = '\0';
                                        std::strncpy(newBook.aname, authorName.c_str(), sizeof(newBook.aname) - 1);
                                        newBook.aname[sizeof(newBook.aname) - 1] = '\0';

                                        books.push_back(newBook);
                                        writeAllBooks(books); // Save to file

                                        // Reset fields
                                        bookNo = "";
                                        bookName = "";
                                        authorName = "";
                                        activeField = -1;
                                        std::cout << "Book Created Successfully!\n";
                                    }
                                }
                            }
                        }
                    }

                    // Handle back button click
                    if(event.type == sf::Event::MouseButtonPressed){
                        sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        if(backButtonCreateBook.isClicked(mousePosF)){
                            state = ADMIN_MENU;
                            // Reset input fields
                            bookNo = "";
                            bookName = "";
                            authorName = "";
                            activeField = 0;
                            std::cout << "Transition back to ADMIN_MENU\n";
                        }
                    }

                    break;
                case SHOW_BOOKS:
                    // Handle back button click
                    if(event.type == sf::Event::MouseButtonPressed){
                        sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        if(backButtonShowBooks.isClicked(mousePosF)){
                            state = ADMIN_MENU;
                            std::cout << "Transition back to ADMIN_MENU\n";
                        }
                    }
                    break;
                case MODIFY_BOOK:
                    // Handle mouse clicks for input fields
                    if(event.type == sf::Event::MouseButtonPressed){
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);
                        // Check which field is clicked
                        if(!bookFound){
                            // Only search box is active
                            sf::FloatRect searchBoxBounds(450, 150, 200, 30);
                            if(searchBoxBounds.contains(mousePosF)){
                                modifyActiveField = 0;
                                std::cout << "Active Field: Search Book No\n";
                            } else {
                                modifyActiveField = -1;
                            }
                        } else {
                            if(modifyInputBox1.getGlobalBounds().contains(mousePosF)){
                                modifyActiveField = 1;
                                std::cout << "Active Field: Book Name\n";
                            }
                            else if(modifyInputBox2.getGlobalBounds().contains(mousePosF)){
                                modifyActiveField = 2;
                                std::cout << "Active Field: Author Name\n";
                            }
                            else{
                                modifyActiveField = -1; // No active field
                            }
                        }
                    }

                    // Handle text input
                    if(event.type == sf::Event::TextEntered){
                        if(state == MODIFY_BOOK){
                            if(!bookFound){
                                // Handle text input for searchBno
                                if(event.text.unicode == '\b'){ // Handle backspace
                                    if(!searchBno.empty()){
                                        searchBno.pop_back();
                                    }
                                }
                                else if(event.text.unicode < 128 && searchBno.size() < 5){
                                    searchBno += static_cast<char>(event.text.unicode);
                                }
                                // Handle Enter key to trigger search
                                if(event.text.unicode == '\r'){
                                    // Refresh books data
                                    books = readAllBooks();
                                    // Perform the search
                                    bookFound = false;
                                    for(const auto& book : books){
                                        std::string storedBno = trim(book.bno);
                                        std::string inputBno = trim(searchBno);
                                        if(toLower(storedBno) == toLower(inputBno)){
                                            selectedBook = book;
                                            bookFound = true;
                                            break;
                                        }
                                    }
                                    if(bookFound){
                                        std::cout << "Book Found: " << selectedBook.bno << "\n";
                                    }
                                    else{
                                        std::cout << "Book Not Found!\n";
                                    }
                                }
                            } else {
                                // Handle text input when modifying book details
                                if(modifyActiveField == 1){
                                    if(event.text.unicode == '\b'){ // Handle backspace
                                        if(strlen(selectedBook.bname) != 0){
                                            selectedBook.bname[strlen(selectedBook.bname)-1] = '\0';
                                        }
                                    }
                                    else if(event.text.unicode < 128 && strlen(selectedBook.bname) < 49){
                                        size_t len = strlen(selectedBook.bname);
                                        selectedBook.bname[len] = static_cast<char>(event.text.unicode);
                                        selectedBook.bname[len + 1] = '\0';
                                    }
                                }
                                else if(modifyActiveField == 2){
                                    if(event.text.unicode == '\b'){ // Handle backspace
                                        if(selectedBook.aname[0] != '\0'){
                                            selectedBook.aname[strlen(selectedBook.aname)-1] = '\0';
                                        }
                                    }
                                    else if(event.text.unicode < 128 && strlen(selectedBook.aname) < 19){
                                        size_t len = strlen(selectedBook.aname);
                                        selectedBook.aname[len] = static_cast<char>(event.text.unicode);
                                        selectedBook.aname[len + 1] = '\0';
                                    }
                                }

                                // Handle Enter key to submit changes
                                if(event.text.unicode == '\r'){
                                    // Update the book in the vector
                                    for(auto &book : books){
                                        std::string storedBno = trim(book.bno);
                                        std::string selectedBno = trim(selectedBook.bno);
                                        if(toLower(storedBno) == toLower(selectedBno)){
                                            std::strncpy(book.bname, selectedBook.bname, sizeof(book.bname) - 1);
                                            book.bname[sizeof(book.bname) - 1] = '\0';
                                            std::strncpy(book.aname, selectedBook.aname, sizeof(book.aname) - 1);
                                            book.aname[sizeof(book.aname) - 1] = '\0';
                                            break;
                                        }
                                    }
                                    writeAllBooks(books); // Save to file
                                    std::cout << "Book details modified successfully!\n";
                                    // Reset variables and go back to ADMIN_MENU
                                    state = ADMIN_MENU;
                                    searchBno = "";
                                    bookFound = false;
                                    selectedBook = Book();
                                    modifyActiveField = -1;
                                    std::cout << "Transition back to ADMIN_MENU\n";
                                }
                            }
                        }
                    }

                    // Handle back button click
                    if(event.type == sf::Event::MouseButtonPressed){
                        sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        if(backButtonModifyBook.isClicked(mousePosF)){
                            state = ADMIN_MENU;
                            // Reset modify book variables
                            searchBno = "";
                            bookFound = false;
                            selectedBook = Book();
                            modifyActiveField = -1;
                            std::cout << "Transition back to ADMIN_MENU\n";
                        }
                    }

                    // Handle submit modify button
                    if(event.type == sf::Event::MouseButtonPressed){
                        sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        if(submitModifyBookButton.isClicked(mousePosF) && bookFound){
                            // Update the book in the vector
                            for(auto &book : books){
                                std::string storedBno = trim(book.bno);
                                std::string selectedBno = trim(selectedBook.bno);
                                if(toLower(storedBno) == toLower(selectedBno)){
                                    std::strncpy(book.bname, selectedBook.bname, sizeof(book.bname) - 1);
                                    book.bname[sizeof(book.bname) - 1] = '\0';
                                    std::strncpy(book.aname, selectedBook.aname, sizeof(book.aname) - 1);
                                    book.aname[sizeof(book.aname) - 1] = '\0';
                                    break;
                                }
                            }
                            writeAllBooks(books); // Save to file
                            std::cout << "Book details modified successfully!\n";
                            // Reset variables and go back to ADMIN_MENU
                            state = ADMIN_MENU;
                            searchBno = "";
                            bookFound = false;
                            selectedBook = Book();
                            modifyActiveField = -1;
                            std::cout << "Transition back to ADMIN_MENU\n";
                        }
                    }

                    break;
                case GENERATE_REPORT:
                    // Handle back button click
                    if(event.type == sf::Event::MouseButtonPressed){
                        sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        if(backButtonGenerateReport.isClicked(mousePosF)){
                            state = ADMIN_MENU;
                            std::cout << "Transition back to ADMIN_MENU\n";
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        // Clear screen
        window.clear(sf::Color::Black);

        // Draw background if available
        if(bgTexture.getSize().x > 0 && bgTexture.getSize().y > 0) {
            window.draw(background);
        }

        // Get mouse position
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

        // Draw based on state
        switch(state){
            case WELCOME:
                drawTextWithBackground(window, welcomeText);
                drawTextWithBackground(window, startText);
                break;
            case MAIN_MENU:
                // Update and draw buttons
                adminButton.update(mousePosF);
                exitButton.update(mousePosF);

                adminButton.render(window);
                exitButton.render(window);
                break;
            case ADMIN_MENU:
                // Display Admin Menu title
                {
                    sf::Text adminMenuText("Administrator Menu", font, 28);
                    adminMenuText.setFillColor(sf::Color::Green);
                    adminMenuText.setPosition(300, 50);
                    adminMenuText.setStyle(sf::Text::Bold);
                    adminMenuText.setOutlineThickness(1);
                    adminMenuText.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, adminMenuText);
                }

                // Update and draw buttons
                createBookButton.update(mousePosF);
                showBooksButton.update(mousePosF);
                modifyBookButton.update(mousePosF);
                generateReportButton.update(mousePosF);
                backToMainMenuButton.update(mousePosF);

                createBookButton.render(window);
                showBooksButton.render(window);
                modifyBookButton.render(window);
                generateReportButton.render(window);
                backToMainMenuButton.render(window);
                break;
            case CREATE_BOOK:
                // Display title
                {
                    sf::Text title("Create a New Book", font, 24);
                    title.setFillColor(sf::Color::Yellow);
                    title.setPosition(300, 50);
                    title.setStyle(sf::Text::Bold);
                    title.setOutlineThickness(1);
                    title.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, title);
                }

                // Display labels
                {
                    sf::Text label1("Book No.: ", font, 20);
                    label1.setFillColor(sf::Color::White);
                    label1.setPosition(200, 150);
                    label1.setStyle(sf::Text::Bold);
                    label1.setOutlineThickness(1);
                    label1.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, label1);

                    sf::Text label2("Book Name: ", font, 20);
                    label2.setFillColor(sf::Color::White);
                    label2.setPosition(150, 220);
                    label2.setStyle(sf::Text::Bold);
                    label2.setOutlineThickness(1);
                    label2.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, label2);

                    sf::Text label3("Author Name: ", font, 20);
                    label3.setFillColor(sf::Color::White);
                    label3.setPosition(150, 290);
                    label3.setStyle(sf::Text::Bold);
                    label3.setOutlineThickness(1);
                    label3.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, label3);
                }

                // Display input boxes
                window.draw(inputBox1);
                window.draw(inputBox2);
                window.draw(inputBox3);

                // Display input texts
                {
                    sf::Text inputText1(bookNo, font, 20);
                    inputText1.setFillColor(sf::Color::Cyan);
                    inputText1.setPosition(320, 150);
                    inputText1.setOutlineThickness(1);
                    inputText1.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, inputText1);

                    sf::Text inputText2(bookName, font, 20);
                    inputText2.setFillColor(sf::Color::Cyan);
                    inputText2.setPosition(320, 220);
                    inputText2.setOutlineThickness(1);
                    inputText2.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, inputText2);

                    sf::Text inputText3(authorName, font, 20);
                    inputText3.setFillColor(sf::Color::Cyan);
                    inputText3.setPosition(320, 290);
                    inputText3.setOutlineThickness(1);
                    inputText3.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, inputText3);
                }

                // Display instructions
                {
                    sf::Text instructions("Press Enter to Submit or Esc to Go Back", font, 15);
                    instructions.setFillColor(sf::Color::White);
                    instructions.setPosition(250, 400);
                    instructions.setStyle(sf::Text::Bold);
                    instructions.setOutlineThickness(1);
                    instructions.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, instructions);
                }

                // Draw back button
                {
                    backButtonCreateBook.update(mousePosF);
                    backButtonCreateBook.render(window);
                }
                break;
            case SHOW_BOOKS:
                // Display title
                {
                    sf::Text title("All Books", font, 24);
                    title.setFillColor(sf::Color::Yellow);
                    title.setPosition(350, 50);
                    title.setStyle(sf::Text::Bold);
                    title.setOutlineThickness(1);
                    title.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, title);
                }

                // Display table headers
                {
                    sf::Text headers("BNo", font, 20);
                    headers.setFillColor(sf::Color::White);
                    headers.setPosition(200, 100);
                    headers.setStyle(sf::Text::Bold);
                    headers.setOutlineThickness(1);
                    headers.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, headers);

                    sf::Text headers2("Book Name", font, 20);
                    headers2.setFillColor(sf::Color::White);
                    headers2.setPosition(300, 100);
                    headers2.setStyle(sf::Text::Bold);
                    headers2.setOutlineThickness(1);
                    headers2.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, headers2);

                    sf::Text headers3("Author Name", font, 20);
                    headers3.setFillColor(sf::Color::White);
                    headers3.setPosition(550, 100);
                    headers3.setStyle(sf::Text::Bold);
                    headers3.setOutlineThickness(1);
                    headers3.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, headers3);
                }

                // Display all books
                {
                    float yOffset = 130;
                    for(const auto& book : books){
                        sf::Text bookText;
                        bookText.setFont(font);
                        bookText.setCharacterSize(18);
                        bookText.setFillColor(sf::Color::Cyan);
                        std::string display = std::string(book.bno) + " | " + std::string(book.bname) + " | " + std::string(book.aname);
                        bookText.setString(display);
                        bookText.setPosition(200, yOffset);
                        bookText.setOutlineThickness(1);
                        bookText.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, bookText);
                        yOffset += 30;
                        if(yOffset > 550) break; // Prevent overflow
                    }
                }

                // Draw back button
                {
                    backButtonShowBooks.update(mousePosF);
                    backButtonShowBooks.render(window);
                }
                break;
            case MODIFY_BOOK:
                // Display title
                {
                    sf::Text title("Modify a Book", font, 24);
                    title.setFillColor(sf::Color::Yellow);
                    title.setPosition(320, 50);
                    title.setStyle(sf::Text::Bold);
                    title.setOutlineThickness(1);
                    title.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, title);
                }

                if(!bookFound){
                    // Display search prompt
                    {
                        sf::Text searchPrompt("Enter Book No to Modify:", font, 20);
                        searchPrompt.setFillColor(sf::Color::White);
                        searchPrompt.setPosition(200, 150);
                        searchPrompt.setStyle(sf::Text::Bold);
                        searchPrompt.setOutlineThickness(1);
                        searchPrompt.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, searchPrompt);
                    }

                    // Display search input box
                    {
                        sf::RectangleShape searchBox(sf::Vector2f(200, 30));
                        searchBox.setPosition(450, 150);
                        searchBox.setFillColor(sf::Color::Black);
                        searchBox.setOutlineThickness(2);
                        searchBox.setOutlineColor(sf::Color::White);
                        window.draw(searchBox);

                        sf::Text searchText(searchBno, font, 20);
                        searchText.setFillColor(sf::Color::Cyan);
                        searchText.setPosition(450, 150);
                        searchText.setOutlineThickness(1);
                        searchText.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, searchText);
                    }

                    // Draw back button
                    backButtonModifyBook.update(mousePosF);
                    backButtonModifyBook.render(window);
                } else {
                    // If book is found, display its details for modification
                    // Display book details
                    {
                        sf::Text bookDetails("Book Details:", font, 20);
                        bookDetails.setFillColor(sf::Color::White);
                        bookDetails.setPosition(200, 220);
                        bookDetails.setStyle(sf::Text::Bold);
                        bookDetails.setOutlineThickness(1);
                        bookDetails.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, bookDetails);
                    }

                    // Display labels
                    {
                        sf::Text label1("Book Name: ", font, 20);
                        label1.setFillColor(sf::Color::White);
                        label1.setPosition(200, 300);
                        label1.setStyle(sf::Text::Bold);
                        label1.setOutlineThickness(1);
                        label1.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, label1);

                        sf::Text label2("Author Name: ", font, 20);
                        label2.setFillColor(sf::Color::White);
                        label2.setPosition(200, 370);
                        label2.setStyle(sf::Text::Bold);
                        label2.setOutlineThickness(1);
                        label2.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, label2);
                    }

                    // Display input boxes
                    window.draw(modifyInputBox1);
                    window.draw(modifyInputBox2);

                    // Display input texts
                    {
                        sf::Text inputText1(selectedBook.bname, font, 20);
                        inputText1.setFillColor(sf::Color::Cyan);
                        inputText1.setPosition(320, 300);
                        inputText1.setOutlineThickness(1);
                        inputText1.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, inputText1);

                        sf::Text inputText2(selectedBook.aname, font, 20);
                        inputText2.setFillColor(sf::Color::Cyan);
                        inputText2.setPosition(320, 370);
                        inputText2.setOutlineThickness(1);
                        inputText2.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, inputText2);
                    }

                    // Instructions
                    {
                        sf::Text instructions("Press Enter to Submit Changes or Esc to Go Back", font, 15);
                        instructions.setFillColor(sf::Color::White);
                        instructions.setPosition(200, 450);
                        instructions.setStyle(sf::Text::Bold);
                        instructions.setOutlineThickness(1);
                        instructions.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, instructions);
                    }

                    // Draw modify buttons
                    {
                        submitModifyBookButton.update(mousePosF);
                        submitModifyBookButton.render(window);
                        backButtonModifyBook.update(mousePosF);
                        backButtonModifyBook.render(window);
                    }
                }
                break;
            case GENERATE_REPORT:
                // Display title
                {
                    sf::Text title("Library Report", font, 24);
                    title.setFillColor(sf::Color::Yellow);
                    title.setPosition(350, 50);
                    title.setStyle(sf::Text::Bold);
                    title.setOutlineThickness(1);
                    title.setOutlineColor(sf::Color::Black);
                    drawTextWithBackground(window, title);
                }

                // Display report content
                {
                    std::vector<Book> reportBooks = readAllBooks(); // Refresh data

                    float yOffset = 100;
                    for(const auto& book : reportBooks){
                        sf::Text reportText;
                        reportText.setFont(font);
                        reportText.setCharacterSize(18);
                        reportText.setFillColor(sf::Color::Cyan);
                        std::stringstream ss;
                        ss << std::left << std::setw(6) << book.bno 
                           << " | " << std::setw(30) << book.bname 
                           << " | " << std::setw(20) << book.aname;
                        reportText.setString(ss.str());
                        reportText.setPosition(150, yOffset);
                        reportText.setOutlineThickness(1);
                        reportText.setOutlineColor(sf::Color::Black);
                        drawTextWithBackground(window, reportText);
                        yOffset += 30;
                        if(yOffset > 550) break; // Prevent overflow
                    }
                }

                // Draw back button
                {
                    backButtonGenerateReport.update(mousePosF);
                    backButtonGenerateReport.render(window);
                }
                break;
            case EXIT_APP:
                // Do nothing; window will close
                break;
            default:
                break;
        }

        // Update the window
        window.display();
    }

    return 0;
}



