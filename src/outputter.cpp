#include <cassert>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <math.h>
#include <vector>
#include <iomanip>

std::ifstream finput;
std::ofstream foutput;

std::string title[36];
int title_counter = 0;

double cholc[5][6];
double cholboost[5][3];
double cholvecc[5][6];
double cholvecboost[5][3];
double cholmtc[5][1];
double frobcholc[5];
double frobcholvecc[5];
double frobcholmtc[5];
double frobcholboost[5];
double frobcholvecboost[5];

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

template <size_t size_x, size_t size_y>
void readChol(double (&arr)[size_x][size_y]) {
	std::string temp;
	for(int i = 0; i < size_x; ++i) {	
		finput >> title[title_counter];
		++title_counter;
		
		for(int j = 0; j < size_y; ++j) {
			finput >> temp;
			arr[i][j] = boost::lexical_cast<double>(temp);
		}
	}	
}

template <size_t size_x>
void readFrob(double (&arr)[size_x]) {
	finput >> title[title_counter];
	++title_counter;	
	std::string temp;
	for(int i = 0; i < size_x; ++i) {	
		finput >> temp;
		arr[i] = boost::lexical_cast<double>(temp);
	}
}

template <size_t size_x, size_t size_y>
void writeMain(double (&arrChol)[size_x][size_y], double (&arrFrob)[size_x], std::string desc) {
	foutput << desc << std::endl;
	for(int i = 0; i < size_x; ++i) {	
		foutput << i << ":\t" <<  std::setprecision(4) << cholc[0][0] / arrChol[i][0] << '\t' << (arrChol[0][0] / arrChol[i][0]) << '\t' << arrFrob[i] << std::endl;
	}
}

template <size_t size_x, size_t size_y>
void writeDetailedc234(double (&arrChol)[size_x][size_y]) {
	for(int i = 0; i < size_x; ++i)  {
		double coef = 100 / (arrChol[i][1] + arrChol[i][2] + arrChol[i][3]);
		for(int j = 1; j < 4; ++j)
			foutput << std::setprecision(2) << arrChol[i][j] * coef << '\t';
		foutput << std::endl;
	}
}

template <size_t size_x, size_t size_y>
void writeDetailedc56(double (&arrChol)[size_x][size_y]) {
	for(int i = 0; i < size_x; ++i) {
		double coef = 100 / (arrChol[i][4] + arrChol[i][5]);
		for(int j = 4; j < 6; ++j)
			foutput << std::setprecision(2) << arrChol[i][j] * coef  << '\t';
		foutput << std::endl;
	}
}

template <size_t size_x, size_t size_y>
void writeDetailedb23(double (&arrChol)[size_x][size_y]) {
	for(int i = 0; i < size_x; ++i) {
		double coef = 100 / (arrChol[i][1] + arrChol[i][2]);
		for(int j = 1; j < 3; ++j)
			foutput << std::setprecision(2) << arrChol[i][j] * coef  << '\t';
		foutput << std::endl;
	}
}

int main(int argc, char* argv[]) {
	
	std::string input_file, output_file;
	
	if(!readConfig(argc, argv, input_file, output_file))
		return 1;
	

	finput.open(input_file.c_str());
	foutput.open(output_file.c_str());

	std::string temp;
	
	readChol(cholc);
	readChol(cholboost);
	readChol(cholvecc);
	readChol(cholvecboost);
	readChol(cholmtc);
	readFrob(frobcholc);
	readFrob(frobcholvecc);
	readFrob(frobcholmtc);
	readFrob(frobcholboost);
	readFrob(frobcholvecboost);
	
	writeMain(cholc, frobcholc, "cholc");
	writeDetailedc234(cholc);
	writeDetailedc56(cholc);
	
	writeMain(cholboost, frobcholboost, "cholboost");
	writeDetailedb23(cholboost);

	writeMain(cholvecc, frobcholvecc, "cholvecc");
	writeDetailedc234(cholvecc);
	writeDetailedc56(cholvecc);
	
	writeMain(cholvecboost, frobcholvecboost, "cholvecboost");
	writeDetailedb23(cholvecboost);
	
	writeMain(cholmtc, frobcholmtc, "cholmtc");

	
	finput.close();
	foutput.close();
	
	return 0;
}
 
