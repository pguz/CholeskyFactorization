#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <cassert>

bool readConfig(
	const int argc,
	char** argv,
	std::string& input_file, 
	std::string& output_file) {
		
	if(argc != 3) {
		std::cout << "Usage: ./" << argv[0] << " input_filename output_filename" << std::endl;
		return false;
	}
	

	input_file = argv[1];
	output_file = argv[2];

	return true;
}

int main(int argc, char* argv[]) {
	
	std::string input_file, output_file;
	
	if(!readConfig(argc, argv, input_file, output_file))
		return 1;
	
	std::ifstream finput;
	std::ofstream foutput;
	finput.open(input_file.c_str());
	foutput.open(output_file.c_str());

	std::string line;
	while (finput >> line) {
		
		bool isDigit = false;
		for(char& c : line) {
			if('0' <= c && c <= '9') {
				if(!isDigit)
					foutput << ' ';
				foutput << c;
				isDigit = true;
			} else {
				isDigit = false;
			}
		}
		
		foutput << std::endl;
	}
	
	finput.close();
	foutput.close();
	
	return 0;
}
