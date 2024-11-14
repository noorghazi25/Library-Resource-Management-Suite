// src/Button.cpp

#include "Button.h"

// Constructor implementation
Button::Button(float x, float y, float width, float height, sf::Font &font, std::string textStr, 
               sf::Color idle, sf::Color hover, sf::Color active) {
    // Initialize shape
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(idle);
    idleColor = idle;
    hoverColor = hover;
    activeColor = active;
    isPressed = false;

    // Initialize text
    text.setFont(font);
    text.setString(textStr);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(20);

    // Center the text within the button
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                  textRect.top + textRect.height / 2.0f);
    text.setPosition(
        shape.getPosition().x + shape.getSize().x / 2.0f,
        shape.getPosition().y + shape.getSize().y / 2.0f
    );
}

// Update method implementation
void Button::update(const sf::Vector2f &mousePos) {
    // Reset to idle color
    shape.setFillColor(idleColor);

    // Hover state
    if (shape.getGlobalBounds().contains(mousePos)) {
        shape.setFillColor(hoverColor);
    }

    // Active state
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shape.getGlobalBounds().contains(mousePos)) {
        shape.setFillColor(activeColor);
        isPressed = true;
    } else {
        isPressed = false;
    }
}

// Render method implementation
void Button::render(sf::RenderWindow &window) {
    window.draw(shape);
    window.draw(text);
}

// isClicked method implementation
bool Button::isClicked(const sf::Vector2f &mousePos) {
    return isPressed && shape.getGlobalBounds().contains(mousePos);
}
