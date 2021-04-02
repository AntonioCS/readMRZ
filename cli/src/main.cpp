#include <iostream>
#include <readMRZ/readMRZ.h>

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Please provide an image\n";
		exit(1);
	}


	std::cout << argv[2];
	
	//readMRZ::readMRZ mrz{ "tessdata/" };
	
}