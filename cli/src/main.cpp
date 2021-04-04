#include <iostream>
#include <readMRZ/readMRZ.h>

int main(int argc, char **argv)
{
	if (argc < 3) {
		std::cout << "Usage: readMRZcli <DataFolderPath> <ImagePath>\n";
		exit(1);
	}

	readMRZ::readMRZ mrz{ argv[1] };
	std::cout << mrz.processImage(argv[2]);

	return 0;
}