#pragma once
#include "MouseClickEvent.h"
#include "KeyBoardEvent.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include <functional>
#include <map>
#include <unordered_map>
#include "Window.h"
#include "EnterNameController.h"

namespace Asteroid
{
	class InputScanner
	{
	public:
		typedef std::function<void ()> KeyBoardAction;
		typedef std::function<void(sf::Mouse::Button button, int x, int y)> MouseClickAction;

		InputScanner();

		bool init(Window* renderWindow);

		bool update();

		void addKeyBoardAction(KeyBoardEvent event, KeyBoardAction action);

		void addMouseAction(MouseClickEvent event, MouseClickAction action);

		void clearInputActions();

		void updateAction();

		bool isKeyBoardEvent(sf::Event& event);

		bool isMouseClickEvent(sf::Event& event);

		bool isQuitEvent(sf::Event& event);
	
	private:
		Window* window;
		std::map<KeyBoardEvent, KeyBoardAction> keyboardActions;
		std::map<MouseClickEvent, MouseClickAction> clickActions;
		EnterNameController nameController;
	};
}