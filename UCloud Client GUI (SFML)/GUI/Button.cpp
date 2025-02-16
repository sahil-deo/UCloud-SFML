#include "Button.h"
#include <iostream>

///Private Functions

void Button::initVariables()
{
    _padding = sf::Vector2f(0, 0);
    _font.loadFromFile("Font/Roboto-Regular.ttf");
    _boxColor = sf::Color::White;
    _boxDownColor = sf::Color(168, 168, 168);
    _enabled = true;


    ///initialize the box

    _box.setPosition(10, 10);
    _box.setFillColor(_boxColor);
    _box.setSize(sf::Vector2f(100, 20));
    
    setBoxDownColor(_boxDownColor);
    setBoxOutline(1, sf::Color::Black);
    
    
    ///initialize the text

    _text.setPosition(10, 10);
    _text.setFillColor(sf::Color::Black);
    _text.setFont(_font);
    _text.setCharacterSize(20);
    

    state = false;
    _released = false;
    _temp = false;
}



///Cons/Desc

Button::Button()
{
    initVariables();
}

Button::Button(std::string string)
{
    initVariables();
    _string = string;
    _text.setString(_string);
}

Button::~Button() = default;



///Public Functinos


//Text Functions
void Button::setFont(sf::Font font)
{
    _font = font;
}

void Button::setTextSize(float size)
{
    _text.setCharacterSize(static_cast<unsigned int>(size));
}

void Button::setFillColor(sf::Color color)
{
    _text.setFillColor(color);
}

void Button::setString(std::string string)
{
    _string = string;
    _text.setString(_string);
}



//Box Functions

void Button::setBoxSize(float width, float height)
{
    _box.setSize(sf::Vector2f(width, height));
}

void Button::setBoxColor(sf::Color color)
{
    _boxColor = color;
    _box.setFillColor(color);
}

void Button::setBoxDownColor(sf::Color color)
{
    _boxDownColor = color;
}

void Button::setBoxOutline(float thinkness, sf::Color color)
{
    _box.setOutlineThickness(thinkness);
    _box.setOutlineColor(color);
}




//Other Functions

void Button::setPosition(float x, float y)
{
    _box.setPosition(x, y);
    _text.setPosition(x + _padding.x, y + _padding.y);
}

const sf::FloatRect Button::getGlobalBounds() const
{
    return _box.getGlobalBounds();
}

void Button::setPadding(float x, float y)
{
    _padding = sf::Vector2f(x, y);

    sf::Vector2f temp = _box.getPosition();
    _text.setPosition(temp.x + _padding.x, temp.y + _padding.y);
}

const bool Button::getState() const
{
    return state;
}

const bool Button::isReleased() const
{
    return _released;
}

void Button::setEnabled(bool value)
{
    _enabled = value;
}


//Loop functions

void Button::eventHandler(sf::RenderWindow* window, sf::Event* event)
{
    if (!_enabled) return;

    if (event->type == sf::Event::MouseButtonPressed &&
        event->mouseButton.button == sf::Mouse::Left &&
            getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
                state = true;
    }
    if (event->type == sf::Event::MouseButtonReleased){
        
        state = false;

        if (_released == false && getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
        
            _released = true;
        }

    }
}

void Button::Render(sf::RenderWindow* window)
{
    //Button Down Color Change Functionality
    {
        if (state) {
            _box.setFillColor(_boxDownColor);
        }
        else {
            _box.setFillColor(_boxColor);
        }
    }

    //Button Release Functionality
    {

        if (_released)
        {
            _released = false;
        }

    }
    
    
    window->draw(_box);
    window->draw(_text);
}
