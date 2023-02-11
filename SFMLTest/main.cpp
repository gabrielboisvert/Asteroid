#include "pch.h"
#include "App.h"

int main(int argc, char* argv[])
{
	Asteroid::App app(argv[argc - 1]);
	app.run();

	return 0;
}