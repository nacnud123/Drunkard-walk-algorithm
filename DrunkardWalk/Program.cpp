// Duncan Armstrong - This is an implementation of the drunkard walk algorithm thing. Originally made in GDScript then ported to C# for use in Unity then ported to C++.

#include <iostream>
#include "DrunkardWalk.hpp"

int widthIn = 10, heightIn = 10, walkersIn = 30;

void handleBadInput() // Clear the input so it does not loop.
{
	std::cout << "Please enter a number!" << std::endl;
	std::cin.clear();
	char c;
	while (std::cin.get(c) && c != '\n')
	{
		// This loop will discard all characters till newline
	}
}

int main(int arc, char* argv[])
{
	while (true)
	{
		std::cout << "Please enter the world width (anything over 500 may take a while): ";
		std::cin >> widthIn;
		if (std::cin.fail()) { handleBadInput(); continue; } // If it is a bad input, tell user to try again and clear the cin stuff.
		std::cout << std::endl;

		std::cout << "Please enter the world height (anything over 500 may take a while): ";
		std::cin >> heightIn;
		if (std::cin.fail()) { handleBadInput(); continue; } // If it is a bad input, tell user to try again and clear the cin stuff.
		std::cout << std::endl;

		std::cout << "Please enter the number of walkers: ";
		std::cin >> walkersIn;
		if (std::cin.fail()) { handleBadInput(); continue; } // If it is a bad input, tell user to try again and clear the cin stuff.
		std::cout << std::endl;

		// Start algorithm.
		DrunkWalk dw = DrunkWalk(widthIn, heightIn, walkersIn);
	}
}