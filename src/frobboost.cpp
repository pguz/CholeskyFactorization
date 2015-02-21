#include <cassert>
#include <fstream>
#include <sstream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/triangular.hpp>

#include <boost/numeric/ublas/io.hpp>
#include <stddef.h>
#include <sys/time.h>
#include <iomanip>

using namespace boost::numeric::ublas;

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
		matrix<double> A, B;
		std::stringstream ssA;
		std::stringstream ssB;
		
		ssA << input1_line;
		ssB << input2_line;
		
		ssA >> A;
		ssB >> B;
		if(A.size1() != B.size1() || A.size2() != B.size2())
			continue;
		foutput << norm_frobenius(A - B) << std::endl;
		global_sum += norm_frobenius(A - B);
		++lines;
	}
	std::cout << std::setprecision(2) << global_sum / lines << std::endl;
	finput1.close();
	finput2.close();
	foutput.close();
	
	return 0;
}
 
