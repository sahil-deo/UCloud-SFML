#include "GuiHandler.h"

void GuiHandler::addToHandler(Button* b)
{
	_buttons.push_back(b);
}

void GuiHandler::addToHandler(TextBox* t)
{
	_textBoxs.push_back(t);
}

void GuiHandler::HandleGuiEvent(sf::RenderWindow* window, sf::Event* event)
{
	for (Button* b : _buttons) {
		if (b != nullptr) {
			b->eventHandler(window, event);
		}
		else {
			//Remove Object from Vector
		}
	}

	for (TextBox* t : _textBoxs) {
		if (t != nullptr) {
			t->eventHandler(window, event);
		}
		else {
			//Remove Object from Vector
		}
	}
}

void GuiHandler::HandleGuiRender(sf::RenderWindow* window)
{
	for (Button* b : _buttons) {
		if (b != nullptr) {
			b->Render(window);
		}
		else {
			//Remove Object from Vector
		}
	}

	for (TextBox* t : _textBoxs) {
		if (t != nullptr) {
			t->Render(window);
		}
		else {
			//Remove Object from Vector
		}
	}
}
