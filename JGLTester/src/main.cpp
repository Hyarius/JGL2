#include "jgl2.h"

int main(int argc, char** argv)
{
	JSONFile data;

	data.load("configuration.json");

	jgl::cout << data.getString("serverAddress") << jgl::endl;
	jgl::cout << data.getString("Cards[0].Completion[0].test") << jgl::endl;

	return (0);
}