#include <time.h>
#include <iostream>
#include <fstream>
#include <cmath>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <random>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/lexical_cast.hpp>

typedef boost::mt19937	ENG;    // Mersenne Twister
typedef boost::random::uniform_real_distribution<double> DIST;   // Normal Distribution
typedef boost::variate_generator<ENG,DIST> GEN;    // Variate generator
    
bool readConfig(
	const int argc,
	char** argv,
	std::string& output_file, 
	int& dist_min, 
	int& dist_max,
	int& size, 
	double& density_coef,
	unsigned int& iters) {
		
	if(argc != 7) {
		std::cout << "Usage: ./" << argv[0] << " filename dist_min[int] dist_max[int] size[int] density_coef[int] iters[unsigned int]" << std::endl;
		return false;
	}
	
	try {
		output_file = argv[1];
		dist_min = boost::lexical_cast<int>(argv[2]);
		dist_max = boost::lexical_cast<int>(argv[3]);
		size = boost::lexical_cast<int>(argv[4]);
		density_coef = boost::lexical_cast<double>(argv[5]);
		iters = boost::lexical_cast<unsigned int>(argv[6]);
	} catch(const boost::bad_lexical_cast& e) {
		std::cout << "Not proper format argument" << std::endl;
		std::cout << "Caught bad lexical cast with error " << e.what() << std::endl;
		std::cout << "Usage: ./" << argv[0] << " filename dist_min[int] dist_max[int] iters[unsigned int]" << std::endl;
		return false;
	}
	return true;
}
    
int main(int argc, char* argv[]) {
	
	std::string output_file;
	int dist_min, dist_max, size;
	double density_coef;
	unsigned int iters;
	
	if(!readConfig(argc, argv, output_file, dist_min, dist_max, size, density_coef, iters))
		return 1;

	std::ofstream foutput;
	foutput.open(output_file.c_str());
  
	std::cout << "dist_min: " << dist_min << " dist_max: " << dist_max << std::endl;
	ENG  eng;
	eng.seed(time(0));
    DIST dist(dist_min, dist_max);
    GEN gen(eng, dist);
    
    std::cout << "size: " << size << std::endl;
    ENG  engSize;
	engSize.seed(time(0));
    DIST distSize(2, size);
    GEN genSize(engSize, distSize);
    
    std::cout << "density_coef: " << density_coef << std::endl;
    ENG engDens;
    engDens.seed(time(0));
    DIST distDens(0, 100);
    GEN genDens(engDens, distDens);

	int i = iters;
	while(i--) {
		int rows = ceil(size); 
		int cols = ceil(size); 
		
		boost::numeric::ublas::matrix<double> X(rows, cols); 
		for (std::size_t i_row = 0; i_row != rows; ++i_row)
			for (std::size_t i_col = 0; i_col != cols; ++i_col) 
				X(i_row, i_col) = static_cast<int>(gen());
		boost::numeric::ublas::matrix<int> C(cols, cols);
		C = prod(trans(X), X);
		foutput << C << std::endl; 
	}
	
	foutput.close();
	return 0;
}
