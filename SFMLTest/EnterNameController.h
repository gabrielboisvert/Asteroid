#pragma once
#include "Window.h"
#include "Text.h"

namespace Asteroid
{
	class EnterNameController
	{
	private:
		const static int MAX_LETTER = 3;

		Window* window;
		std::string name;
		Text* text;
	public:

		void init(Window& window);

		const std::string getName() const;

		~EnterNameController();
	};
}

