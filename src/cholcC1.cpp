#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <stddef.h>
#include <sys/time.h>
#include <math.h>

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


bool cholesky_decompose(double* C, double* R, int n) {

	//t_1s = gettime_();
    for (int j = 0; j < n; ++j) {
        double s = 0;
        t_2s = gettime_();
        int jn = j * n;
		for (int k = 0; k < j; ++k) {
            s += R[jn + k] * R[jn + k];
        }
        t_2e = gettime_();
		t_2 += (t_2e - t_2s);
        t_3s = gettime_();
        R[jn + j] = sqrt(C[jn + j] - s);
        t_3e = gettime_();
		t_3 += (t_3e - t_3s);
        t_4s = gettime_();
		for (int i = j + 1; i < n; ++i) {
            double s = 0;
            int in = i * n;
            //t_5s = gettime_();
			for (int k = 0; k < j; k++) {
                s += R[in + k] * R[jn + k];
            }
            //t_5e = gettime_();
            //t_5 += (t_5e - t_5s);
            //t_6s = gettime_();
            R[in + j] = (1.0 / R[jn + j] * (C[in + j] - s));
			//t_6e = gettime_();
			//t_6 += (t_6e - t_6s);
        }
        t_4e = gettime_();
        t_4 += (t_4e - t_4s);
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
		int n, m;
		finput >> n >> m;
		assert(n == m);
		double *C = (double*)malloc(n * n * sizeof(double));
		if (C == NULL)
			return 1;
			
		double *R = (double*)calloc(n * n, sizeof(double));
		if (R == NULL)
			return 1;
			
		for(int i = 0; i < n * n; ++i)
			finput >> C[i];
			
		double t_start = gettime_();
		if(cholesky_decompose(C, R, n)) {
			double t_end = gettime_();
			for(int i = 0; i < n; ++i) {
				for(int j = 0; j < n; ++j)
					foutput << R[i * n + j] << ' ';
			}
			foutput << '\n';
		} else {
			foutput << "Not proper form of matrix C" << std::endl;
		}	
				
		free(C);
		free(R);
		
	}
	
	//std::cout << "Całkowity czas operacji: " << t_global << std::endl;
	//std::cout << "Operacja 1: " << t_1 << std::endl;
	std::cout << t_2 << std::endl;
	std::cout << t_3 << std::endl;
	std::cout << t_4 << std::endl;
	//std::cout << "Operacja 5: " << t_5 << " " << t_5/(t_5+t_6) << std::endl;
	//std::cout << "Operacja 6: " << t_6 << " " << t_6/(t_5+t_6) << std::endl;

	finput.close();
	foutput.close();
	
	return 0;
}
