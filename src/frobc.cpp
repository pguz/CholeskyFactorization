#include <cassert>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <math.h>
#include <vector>
#include <iomanip>

bool readConfig(
	const int argc,
	char** argv,
	std::string& input1_file, 
	std::string& input2_file, 
	std::string& output_file) {
		
	if(argc != 4) {
		std::cout << "Usage: ./" << argv[0] << " input1_filename input2_filename output_filename" << std::endl;
		return false;
	}

	input1_file = argv[1];
	input2_file = argv[2];
	output_file = argv[3];
	return true;
}

int main(int argc, char* argv[]) {
	
	std::string input1_file, input2_file, output_file;
	
	if(!readConfig(argc, argv, input1_file, input2_file, output_file))
		return 1;
	
	std::ifstream finput1;
	std::ifstream finput2;
	std::ofstream foutput;
	finput1.open(input1_file.c_str());
	finput2.open(input2_file.c_str());
	foutput.open(output_file.c_str());

	std::string input1_line, input2_line;
	double global_sum = 0;
	int lines = 0;
	while (std::getline(finput1, input1_line) && std::getline(finput2, input2_line)) {
		std::vector<std::string> strs1;
		std::vector<std::string> strs2;
		boost::split(strs1, input1_line, boost::is_any_of("\t "));
		boost::split(strs2, input2_line, boost::is_any_of("\t "));
		double sum = 0;
		if(strs1.size() != strs2.size())
			continue;
		for(int i = 0; i < strs1.size() - 1; ++i) {
			double a1 = (isnan(isnan(boost::lexical_cast<double>(strs1[i]))) ? 0 : boost::lexical_cast<double>(strs1[i]));
			double a2 = (isnan(boost::lexical_cast<double>(strs2[i])) ? 0 : boost::lexical_cast<double>(strs2[i]));
			sum += pow(a1 - a2, 2);
		}
		if(isnan(sum) || isinf(sum)) 
			continue;
		
		foutput << sqrt(sum) << std::endl;
		
		global_sum += sqrt(sum);
		++lines;
	}
	std::cout << std::setprecision(2) << global_sum / lines << std::endl;
	finput1.close();
	finput2.close();
	foutput.close();
	
	return 0;
}
 
