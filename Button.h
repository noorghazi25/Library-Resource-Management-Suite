// include/Button.h

#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    bool isPressed;

public:
    // Constructor
    Button(float x, float y, float width, float height, sf::Font &font, std::string textStr, 
           sf::Color idle, sf::Color hover, sf::Color active);
    
    // Update the button's state based on mouse position
    void update(const sf::Vector2f &mousePos);
    
    // Render the button to the window
    void render(sf::RenderWindow &window);
    
    // Check if the button is clicked
    bool isClicked(const sf::Vector2f &mousePos);
};

#endif
