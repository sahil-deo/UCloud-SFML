#pragma once
#include <vector>
#include "includes.h"
#include "Button.h"
#include "TextBox.h"

class GuiHandler
{
private:
	std::vector<Button*> _buttons;
	std::vector<TextBox*> _textBoxs;

private:
public:
	
	void addToHandler(Button* b);
	void addToHandler(TextBox* t);

	void HandleGuiEvent(sf::RenderWindow *window, sf::Event *event);
	void HandleGuiRender(sf::RenderWindow* window);
};

