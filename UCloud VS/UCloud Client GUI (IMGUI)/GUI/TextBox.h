#pragma once

#include "includes.h"

class TextBox
{
private:


	//Attibutes
	sf::Font _font;
	sf::Vector2f _padding;
	bool _focus;
	bool _isPassword;
	bool _paste;

	//Content
	std::string _string;

	//Componenets
	sf::RectangleShape _box;
	sf::Text _text;



private:

	void initVariables();


public:

	///Cons/Decs

	TextBox();

	TextBox(std::string string);
	
	~TextBox();

	//-------------------------------------

	///Text Functions
	
	void setFont(sf::Font font);
	
	void setTextSize(float size);
	
	void setFillColor(sf::Color color);               
	
	void setString(std::string string);

	void clearString();

	void setPasswordField(bool isPassword);

	bool getPasswordField();

	std::string getString();

	//-------------------------------------
	
	///Box Functions
	
	void setBoxSize(float width, float height);
	
	void setBoxColor(sf::Color color);
	
	void setBoxOutline(float thinkness, sf::Color color);

	//-------------------------------------

	///Other Functions
	
	void setPosition(float x, float y);
	
	const sf::FloatRect getGlobalBounds() const;
	
	void setPadding(float x, float y);

	void setFocus(bool focus);
	
	//-------------------------------------

	///Loop Functions

	void eventHandler(sf::RenderWindow* window, sf::Event* event);

	void Render(sf::RenderWindow* window);

	//-------------------------------------

};

