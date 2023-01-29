#include "jgl2.h"

int main(int argc, char** argv)
{
	std::vector<std::string> files = jgl::listFile("resources", ".cpp");

	jgl::cout << "Files : " << files << jgl::endl;

	return (0);
}