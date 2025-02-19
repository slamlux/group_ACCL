#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initialise_vector(double vector[], int size, double initial);
double T_equilibrium_global(int T0, int T1, int T2);
double T_equilibrium(int T0, int T1);
double T_time(double T_eq, double T0, double step_size);
int check_args(int argc, char **argv);

int main(int argc, char **argv)
{

	int points = check_args(argc, argv);
	
	double x0, x1, x2, T0, T1, T2, T_eq, T_global, T_t;
	T0 = 300;
	T1 =270; 
	T2 = 270;
	x0 = 0;
	x1 = 1;
	x2 = 2;
	T_global, T_eq, T_t = 0;
	int time_steps = 20; // total timesteps
	double step_size = 1.0/2.0;

	// creates a vector for the time stamps in the data
	double* time_stamps = (double*) malloc(time_steps * sizeof(double));
	initialise_vector(time_stamps, time_steps, 0.0);
	//generate_timestamps(time_stamps, time_steps, step_size);

	// creates a vector variable for the current positions
	double* positions = (double*) malloc(points * sizeof(double));
	// and initialises every element to zero
	initialise_vector(positions, points, 0.0);

	T_global = T_equilibrium_global(T0, T1, T2);
	T_eq = T_equilibrium(T0, T1);
	T_t = T_time(T_eq, T0, step_size);

	printf("Global equilibrim: %f\n", T_global);
	printf("Equilibrim temp: %f\n", T_eq);
	printf("temperature after one time step: %f\n", T_t);

	// if we use malloc, must free when done!
	free(time_stamps);
	free(positions);

	return 0;
	
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(double vector[], int size, double initial)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		vector[i] = initial;
	}
}


double T_equilibrium_global(int T0, int T1, int T2)
{
	double T_global = 0;
	T_global = (T0 + T1 + T2) / 3;
	return T_global;

}


double T_equilibrium(int T0, int T1)
{	
	double T_eq = 0;	
	T_eq = (T0 + T1) / 2;
	return T_eq;
}

double T_time(double T_eq, double T0, double step_size)
{

	double T_t = 0;
	T_t = T_eq + (T0 - T_eq) * exp(-step_size);
	return T_t;

}

int check_args(int argc, char **argv)
{
	// declare and initialise the numerical argument
	int num_arg = 0;

	// check the number of arguments
	if (argc == 2) // program name and numerical argument
	{
		// declare and initialise the numerical argument
		num_arg = atoi(argv[1]);
	}
	else // the number of arguments is incorrect
	{
		// raise an error
		fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
		fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

		// and exit COMPLETELY
		exit (-1);
	}
	return num_arg;
}

