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
	if (gettimeofday(&timer,NULL)) 
		return -1.0;
	return timer.tv_sec+1.0e-6*timer.tv_usec;
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

double t_1, t_1s, t_1e;
double t_2, t_2s, t_2e;
double t_3, t_3s, t_3e;
double t_4, t_4s, t_4e;
double t_5, t_5s, t_5e;
double t_6, t_6s, t_6e;

bool cholesky_decompose(matrix<double> &C, matrix<double> &R)
{
	const size_t n = C.size1();
	//t_1s = gettime_();
	for (size_t k=0 ; k < n; k++) {
		t_2s = gettime_();
		double Rkk = C(k,k) - inner_prod( project( row(R, k), range(0, k) ),project( row(R, k), range(0, k) ) );
		if(Rkk <= 0)
			return false;

		R(k,k) = sqrt( Rkk );
		t_2e = gettime_();
		t_2 += (t_2e - t_2s);
		

		
		matrix_column< matrix<double> > cLk(R, k);
		t_3s = gettime_();
		project(cLk, range(k+1, n))
			= ( project( column(C, k), range(k+1, n) )
			- prod( project(R, range(k+1, n), range(0, k)),
			project(row(R, k), range(0, k) ) ) ) / R(k,k);
		t_3e = gettime_();
        t_3 += (t_3e - t_3s);
	}
	//t_1e = gettime_();
	//t_1 += (t_1e - t_1s);
	
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
			foutput << R << std::endl;
		else
			foutput << "Not proper form of matrix C" << std::endl;
	}
	//std::cout << "Operacja 1: " << t_1 << std::endl;
	std::cout << t_2 << std::endl;
	std::cout << t_3 << std::endl;

	finput.close();
	foutput.close();
	
	return 0;
}
 
