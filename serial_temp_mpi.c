#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

void initialise_vector(double vector[], int size, double initial);
double T_equilibrium_global(double temperatures[], int size);
double T_equilibrium(double T0, double T1);
double T_time(double T_eq, double T0, double step_size);
int check_args(int argc, char **argv);
void print_header(FILE** p_out_file, int points);
void update_temperatures(double* temperatures, int points, double time_step, double T_eq);
int generate_timestamps(double* timestamps, int time_steps, double step_size);
double sum_vector(double vector[], int size);
void root_task(int uni_size, int num_arg, double T0, double time_stamps[]);
void client_task(int my_rank, int num_arg, int uni_size, double temp[], double step_size, double T_global);
void check_task(int uni_size, int my_rank, int num_arg, double vector[], double T0, double step_size, double T_global, double time_stamps[], int time_steps);

int main(int argc, char **argv)
{
	// declare and initialise error handling variable
	int ierror = 0;

	int points = check_args(argc, argv);
	// intitalise MPI
	ierror = MPI_Init(&argc, &argv);
	
	double x0, x1, x2, T0, T1, T2, T_eq, T_global, T_t;
	T0 = 400;
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
	

	// declare and initialise rank and size varibles
	int my_rank, uni_size;
	my_rank = uni_size = 0;

	// gets the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
	
	// checks the universe size is correct
	//check_uni_size(uni_size);

	// creates a vector variable for the current positions
	double* temperatures = (double*) malloc(points * sizeof(double));
	// and initialises every element to zero
	initialise_vector(temperatures, points, 270.0);
	temperatures[0] = T0;

	T_global = T_equilibrium_global(temperatures, points);
	T_eq = T_equilibrium(T0, T1);
	//T_t = T_time(T_eq, T0, step_size);

	//printf("Global equilibrim: %f\n", T_global);
	//printf("Equilibrim temp: %f\n", T_eq);
	//printf("temperature after one time step: %f\n", T_t);
	double* casa;
	casa = temperatures;
	
	check_task(uni_size, my_rank, points, temperatures, T0, step_size, T_global, time_stamps, time_steps);		


	// finalise MPI
	ierror = MPI_Finalize();

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


double T_equilibrium_global(double temperatures[], int size)
{
	double T_global = 0;
	double tot_temp= sum_vector(temperatures, size);
	T_global = tot_temp / size;
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

// defines a function to sum a vector of ints into another int
double sum_vector(double vector[], int size)
{
	// creates a variable to hold the sum
	int sum = 0;

	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		sum += vector[i];
	}

	// returns the sum
	return sum;
}

void root_task(int uni_size, int points, double T0, double time_stamps[])
{
	
	// creates and initialies transmission variables
	double* recv_message1 = (double*) malloc((points+2) * sizeof(double));
	// and initialises every element to zero
	initialise_vector(recv_message1, points, 0.0);
	double recv_message;
	int count, source, tag;
	recv_message = source = tag = 0;
	count = 1;
	MPI_Status status;
	
	int time_steps = 20; // total timesteps
	// creates a vector variable for the current positions
	double* temperatures = (double*) malloc(points * sizeof(double));
	// and initialises every element to zero
	initialise_vector(temperatures, points, 270.0);
	temperatures[0] = T0;

	// creates a file
	FILE* out_file;
     	out_file = fopen("data/temperatures_over_time_mpi2.csv","w");
	print_header(&out_file, points);
	
	//prints temperatures at t=0
	
	fprintf(out_file, "%d, %lf", 0, time_stamps[0]);
	for (int j = 0; j < points; j++)
		{
			// prints each y-position to a file
			fprintf(out_file, ", %lf", temperatures[j]);
		}
	fprintf(out_file, "\n");
	int chunk, start, stop;
	chunk = start = stop = 0;
	chunk = points/(uni_size-1);
	start = (source-1) * chunk;
	stop =  source *chunk;

	for (source = 1; source < uni_size; source++)
	{
		// receives the messages
		for (int j = 0; j < chunk; j++)
		{
			MPI_Recv(&recv_message, count, MPI_DOUBLE, source, j, MPI_COMM_WORLD, &status);
			start = (source-1) * chunk;
			temperatures[j+start] = recv_message;
		
			printf("%fmy rank %d\n", temperatures[j+start], source);
			//printf('d/n', source);
		}


	} // end for (source = 1; source < uni_size; source++)

	// updates the position using a function
	//update_temperatures(temperatures, points, step_size, T_global);

	// prints an index and time stamp
	fprintf(out_file, "%d, %lf", 1, time_stamps[1]);

	// iterates over all of the points on the line
	for (int j = 0; j < points; j++)
	{
		// prints each y-position to a file
		fprintf(out_file, ", %lf", temperatures[j]);
	}
	// prints a new line
	fprintf(out_file, "\n");	

	free(temperatures);
	free(recv_message1);

}

void client_task(int my_rank, int num_arg, int uni_size, double temp[], double step_size, double T_global)
{

	// creates and initialies transmission variables
	double send_message, recv_message;
	int count, dest, tag;
	send_message = recv_message = 0;
	dest = tag = 0;
	count = 1;
	MPI_Status status;

	// sets the destination for the message
	dest = 0; // destination is root
	int chunk, start, stop;
	start = stop = 0;
	chunk = num_arg/(uni_size-1);
	start = (my_rank-1) * chunk;
	stop =  my_rank *chunk;

	double* temperatures = (double*) malloc((chunk+1) * sizeof(double));
	// and initialises every element to zero
	initialise_vector(temperatures, chunk, 0.0);
	if ((uni_size-1)==my_rank)
	{
		for (int j = 0; j <= chunk; j++)
		{
			// prints each y-position to a file
			temperatures[j]=temp[j+start-1];
		}
		for (int w= 0; w < 100; w++)
		{
			update_temperatures(temperatures, (chunk+1), step_size, T_global);
			MPI_Recv(&recv_message, 1, MPI_DOUBLE,(my_rank-1), 0, MPI_COMM_WORLD, &status);
			temperatures[0]=recv_message; 
		}
		

		for (int i = 0; i < chunk; i++)
		{	
			tag = i;
			send_message = temperatures[i+1];
			MPI_Send(&send_message, count, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
		}

	}
	else
	{
		for (int j = 0; j <= chunk; j++)
		{
			// prints each y-position to a file
			temperatures[j]=temp[j+start];
		}
		for (int w=0; w < 100; w++)
		{
			if (my_rank == 1)
			{
				update_temperatures(temperatures, (chunk+1), step_size, T_global);
				MPI_Send(&temperatures[3], 1, MPI_DOUBLE, (my_rank+1), 0, MPI_COMM_WORLD);
			}
			else
			{
				update_temperatures(temperatures, (chunk+1), step_size, T_global);
				MPI_Send(&temperatures[3], 1 , MPI_DOUBLE, (my_rank+1), 0, MPI_COMM_WORLD);
				MPI_Recv(&recv_message, 1, MPI_DOUBLE, (my_rank-1), 0, MPI_COMM_WORLD, &status);
				temperatures[0]=recv_message;
			}
		}
		for (int i = 0; i < chunk; i++)
		{	
				tag = i;
				send_message = temperatures[i];
				MPI_Send(&send_message, count, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
		}

	}

	free(temperatures);	
	//free(send_message);
}

void check_task(int uni_size, int my_rank, int num_arg, double vector[], double T0, double step_size, double T_global, double time_stamps[], int time_steps)
{
	double* temperatures;
	temperatures =vector;

	// checks which process is running and calls the appropriate task
	if (0 == my_rank)
	{
		root_task(uni_size, num_arg, T0, time_stamps);
	} // end if (0 == my_rank)
	else // i.e. (0 != my_rank)
	{
		client_task(my_rank, num_arg, uni_size, temperatures, step_size, T_global);
	} // end else // i.e. (0 != my_rank)

}

