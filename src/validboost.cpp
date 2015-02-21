#include <cassert>
#include <fstream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/triangular.hpp>

#include <boost/numeric/ublas/io.hpp>
#include <stddef.h>
#include <sys/time.h>

using namespace boost::numeric::ublas;

double gettime_ (void) {
	struct timeval timer;
	if (gettimeofday(&timer, NULL)) 
		return -1.0;
	return timer.tv_sec + 1.0e-6 * timer.tv_usec;
}

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

bool cholesky_decompose(matrix<double> &C, matrix<double> &R)
{
	const size_t n = C.size1();
	for (size_t k = 0 ; k < n; k++) {
		double Rkk = C(k,k) - inner_prod( project( row(R, k), range(0, k) ),project( row(R, k), range(0, k) ) );
		if(Rkk <= 0)
			return false;

		R(k,k) = sqrt( Rkk );

		
		matrix_column< matrix<double> > cLk(R, k);
		project(cLk, range(k+1, n))
			= ( project( column(C, k), range(k+1, n) )
			- prod( project(R, range(k+1, n), range(0, k)),
			project(row(R, k), range(0, k) ) ) ) / R(k,k);
	}
	
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

	while (!finput.eof()) {
		
		matrix<double> C;
		size_t dim_C;
		finput >> C;
		dim_C = C.size1();
		assert( dim_C == C.size2() );
		matrix<double> R (dim_C, dim_C);
		R = zero_matrix<double> (dim_C, dim_C);
		
		if(cholesky_decompose(C, R))
			foutput << C << std::endl;
			
	}

	finput.close();
	foutput.close();
	
	return 0;
}
 
