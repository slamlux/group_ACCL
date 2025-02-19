#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initialise_vector(double vector[], int size, double initial);
double T_equilibrium_global(int T0, int T1, int T2);
double T_equilibrium(double T0, double T1);
double T_time(double T_eq, double T0, double step_size);
int check_args(int argc, char **argv);
void print_header(FILE** p_out_file, int points);
void update_temperatures(double* temperatures, int points, double time_step, double T_eq);
int generate_timestamps(double* timestamps, int time_steps, double step_size);

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
	generate_timestamps(time_stamps, time_steps, step_size);

	// creates a vector variable for the current positions
	double* temperatures = (double*) malloc(points * sizeof(double));
	// and initialises every element to zero
	temperatures[0] = T0;
	initialise_vector(temperatures, points, 270.0);
	temperatures[0] = T0;

	T_global = T_equilibrium_global(T0, T1, T2);
	T_eq = T_equilibrium(T0, T1);
	//T_t = T_time(T_eq, T0, step_size);

	//printf("Global equilibrim: %f\n", T_global);
	//printf("Equilibrim temp: %f\n", T_eq);
	//printf("temperature after one time step: %f\n", T_t);
	
	// creates a file
	FILE* out_file;
     	out_file = fopen("data/temperatures_over_time.csv","w");
	print_header(&out_file, points);

	//prints temperatures at t=0
	fprintf(out_file, "%d, %lf", 0, time_stamps[0]);
	for (int j = 0; j < points; j++)
		{
			// prints each y-position to a file
			fprintf(out_file, ", %lf", temperatures[j]);
		}
	fprintf(out_file, "\n");

	// iterates through each time step in the collection
	for (int i = 1; i < time_steps; i++)
	{
		// updates the position using a function
		update_temperatures(temperatures, points, step_size, T_global);

		// prints an index and time stamp
		fprintf(out_file, "%d, %lf", i, time_stamps[i]);

		// iterates over all of the points on the line
		for (int j = 0; j < points; j++)
		{
			// prints each y-position to a file
			fprintf(out_file, ", %lf", temperatures[j]);
		}
		// prints a new line
		fprintf(out_file, "\n");
	}

	// if we use malloc, must free when done!
	free(time_stamps);
	free(temperatures);

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


double T_equilibrium(double T0, double T1)
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

// prints a header to the file
// double-pointer used to allow this function to move the file pointer
void print_header(FILE** p_out_file, int points)
{
	fprintf(*p_out_file, "#, time");
	for (int j = 0; j < points; j++)
	{
		fprintf(*p_out_file, ", y[%d]", j);
	}
	fprintf(*p_out_file, "\n");
}

// defines a function to update the positions
void update_temperatures(double* temperatures, int points, double time_step, double T_eq)
{
	// creates a temporary vector variable for the new positions
        double* new_temperatures = (double*) malloc(points * sizeof(double));
	
	double T_eq_i = T_equilibrium(temperatures[0],temperatures[1]);
	new_temperatures[0] = T_time(T_eq_i, temperatures[0], time_step);

	double T_eq_f = T_equilibrium(temperatures[points-2],temperatures[points-1]);
	new_temperatures[points-1] = T_time(T_eq_f, temperatures[points-1], time_step);


	// creates new positions by setting value of previous element 
	for (int i = 1; i < (points-1); i++)
	{
		double T_eq1 = T_equilibrium(temperatures[i],temperatures[i+1]);
		double T_eq2 = T_equilibrium(temperatures[i],temperatures[i-1]);
		double T1 = T_time(T_eq1, temperatures[i], time_step);
		double T2 = T_time(T_eq2, temperatures[i], time_step);

		new_temperatures[i] = T1 + T2 - temperatures[i];
	}
	// propagates these new positions to the old ones
	for (int i = 0; i < points; i++)
        {
                temperatures[i] = new_temperatures[i];
        }

	// frees the temporary vector
	free(new_temperatures);
}

// defines a set of timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
	for (int i = 0; i < time_steps ; i++)
	{
		timestamps[i]=i*step_size;
	}	
	return time_steps;
}


