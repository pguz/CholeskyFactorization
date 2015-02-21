#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <stddef.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>

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


bool cholesky_decompose(double* C, double* R, int n, int npes, int rank) {
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0) {
		t_1s = MPI_Wtime();
	}
	
    for (int j = 0; j < n; ++j) {		
        double s = 0;
        int jn = j * n;
        if (j % npes == rank) {
			for (int k = 0; k < j; ++k) {
				s += R[jn + k] * R[jn + k];
			}

			R[jn + j] = sqrt(C[jn + j] - s);
        }
        
        MPI_Bcast(&R[jn], n, MPI_DOUBLE, j % npes, MPI_COMM_WORLD);

		for (int i = j + 1; i < n; ++i) {
            double s = 0;
            int in = i * n;
            if (i % npes == rank )
			{
				for (int k = 0; k < j; k++) {
					s += R[in + k] * R[jn + k];
				}
				R[in + j] = (1.0 / R[jn + j] * (C[in + j] - s));
			}
        }
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0) {
		t_1e = MPI_Wtime();
	}
	
	t_1 += (t_1e - t_1s);
	
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

	int npes, rank;
	
	MPI_Init(&argc , &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
		
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
		if(cholesky_decompose(C, R, n, npes, rank)) {
			double t_end = gettime_();
			for(int i = 0; i < n; ++i) {
				for(int j = 0; j < n; ++j) {
					foutput << R[i * n + j] << ' ';
				}
				
			}
			foutput << '\n';
		} else {
			foutput << "Not proper form of matrix C" << std::endl;
		}	
				
		free(C);
		free(R);
		
	}
	MPI_Finalize();
	std::cout << t_1 << std::endl;

	finput.close();
	foutput.close();
	
	return 0;
}
