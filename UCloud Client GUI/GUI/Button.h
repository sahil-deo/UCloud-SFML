#pragma once
#include "includes.h"

class Button
{
private:

	//Attributes
	sf::Font _font;
	sf::Vector2f _padding;
	sf::Color _boxDownColor;
	sf::Color _boxColor;
	bool _temp;
	bool _enabled;

	//Content
	bool state;
	bool _released;
	std::string _string;
	
	//Components
	sf::Text _text;
	sf::RectangleShape _box;


private:

	void initVariables();

public:

	//-------------------------------------

	///Cons/Decs
	
	Button();
	Button(std::string string);
	~Button();

	//-------------------------------------

	///Text Functions
	
	void setFont(sf::Font font);

	void setTextSize(float size);

	void setFillColor(sf::Color color);

	void setString(std::string string);


	//-------------------------------------

	///Box Functions
	
	void setBoxSize(float width, float height);

	void setBoxColor(sf::Color color);

	void setBoxDownColor(sf::Color color);

	void setBoxOutline(float thinkness, sf::Color color);

	//-------------------------------------

	///Other Functions

	void setPosition(float x, float y);

	const sf::FloatRect getGlobalBounds() const;

	void setPadding(float x, float y);

	const bool getState() const;

	const bool isReleased() const;

	void setEnabled(bool value);
	
	//-------------------------------------

	///Loop Functions
	void eventHandler(sf::RenderWindow* window, sf::Event * event);
	void Render(sf::RenderWindow* window);

	//-------------------------------------

};

