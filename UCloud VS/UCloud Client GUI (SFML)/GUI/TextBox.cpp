#include "TextBox.h"
#include <iostream>


///Private Functions

void TextBox::initVariables()
{

    _padding = sf::Vector2f(0, 0);
    _font.loadFromFile("Font/Roboto-Regular.ttf");
    _isPassword = false;
    _focus = false;

    ///initialize the box

    _box.setSize(sf::Vector2f(100, 20));
    _box.setPosition(10, 10);
    _box.setFillColor(sf::Color::White);
    setBoxOutline(1, sf::Color::Black);

    ///initialize the text

    _text.setPosition(10, 10);
    _text.setFillColor(sf::Color::Black);
    _text.setFont(_font);
    _text.setCharacterSize(20);



}

///Constructors

TextBox::TextBox()
{

    this->initVariables();
}

TextBox::TextBox(std::string string)
{
    _string = string;
    this->initVariables();
}

TextBox::~TextBox() = default;

///Public Functions:

//Text Functions

void TextBox::setFont(sf::Font font)
{
    _font = font;
}

void TextBox::setFillColor(sf::Color color)
{
    _text.setFillColor(color);
}

void TextBox::setTextSize(float size)
{
    _text.setCharacterSize(size);
}

void TextBox::setString(std::string string)
{
    _string = string;
    _text.setString(_string + "_");

}

void TextBox::clearString()
{
    _string = "";
    _text.setString(_string);
}

void TextBox::setPasswordField(bool isPassword)
{
    _isPassword = isPassword;
}

bool TextBox::getPasswordField()
{
    return _isPassword;
}

std::string TextBox::getString()
{
    return _string;
}


//Box Functions

void TextBox::setBoxSize(float width, float height)
{
    _box.setSize(sf::Vector2f(width, height));
}

void TextBox::setBoxColor(sf::Color color)
{
    _box.setFillColor(color);
}

void TextBox::setBoxOutline(float thinkness, sf::Color color)
{
    _box.setOutlineThickness(thinkness);
    _box.setOutlineColor(color);
}




//Other Functions


void TextBox::setPosition(float x, float y)
{
    _box.setPosition(x, y);
    _text.setPosition(x + _padding.x, y + _padding.y);
}

const sf::FloatRect TextBox::getGlobalBounds() const
{
    return _box.getGlobalBounds();
}

void TextBox::setPadding(float x, float y)
{
    _padding = sf::Vector2f(x,y);
    
    sf::Vector2f temp = _box.getPosition();
    _text.setPosition(temp.x + _padding.x, temp.y + _padding.y);
}

void TextBox::setFocus(bool focus)
{
    _focus = focus;
}


//Loop functions

void TextBox::eventHandler(sf::RenderWindow* window, sf::Event* event)
{

    if (event->type == sf::Event::MouseButtonPressed &&
        event->mouseButton.button == sf::Mouse::Left)
    {
        if (getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
            setFocus(true);
        else
            setFocus(false);
    }
    
    if(_focus == true)
    {
        if (event->type == sf::Event::TextEntered) {
            if (std::isprint(event->text.unicode))
                _string += event->text.unicode;
        }
        else if (event->type == sf::Event::KeyPressed) {
            if (event->key.code == sf::Keyboard::BackSpace) {
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                    _string = "";

                else if (!_string.empty())
                    _string.pop_back();
            
            }
            if (event->key.code == sf::Keyboard::Return) {
                _string += '\n';
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                if (!_paste) {
                    if (event->key.code == sf::Keyboard::V) {
                        _string += sf::Clipboard::getString();
                        //std::cout << "Pasted";
                        //std::cout << static_cast<std::string>(sf::Clipboard::getString());
                        _paste = true;
                    }
                }
            }
        }
        _text.setString(_string + '_');
    }
    else {
        _text.setString(_string);
    }

    if (event->type == sf::Event::KeyReleased) {
        if (event->key.code == sf::Keyboard::V) {
            _paste = false;
        }
    }
}

void TextBox::Render(sf::RenderWindow* window)
{
    
    //Draw Stuff
    if (!_focus) {
        if (!_isPassword) {
            _text.setString(_string);
        }
        else {
            std::string temp;
            for (int i = 0; i < _string.length(); i++) {
                temp.push_back('*');
            }
            _text.setString(temp);
        }

    }
    else {
        if (!_isPassword) {
            _text.setString(_string + '_');
        }
        else {
            std::string temp;
            for (int i = 0; i < _string.length(); i++) {
                temp.push_back('*');
            }
            _text.setString(temp + '_');
        }
    }

    window->draw(_box);
    window->draw(_text);

}
