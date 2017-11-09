#include <iostream>
#include <fstream>
#include <math.h>

int main(int argc, char const *argv[])
{
	
	std::ifstream inFile;
	inFile.open(argv[1]);
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    std::string str;

	std::ofstream output;
	output.open("ListOfReactions.txt");

    while(getline(inFile,str)) {
		int firstIndex = str.find("<RX.ID>", 0);
		if (firstIndex > 0) {
			int secondIndex = str.find("</RX.ID>", 0);
			output << str.substr(firstIndex+7, secondIndex-firstIndex-7) << ",\n";
		}
    }

    inFile.close();
    output.close();
	return 0;
}