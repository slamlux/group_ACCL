#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void initialise_vector(double vector[], int size, double initial);
double T_equilibrium_global(int T0, int T1, int T2);
double T_equilibrium(double T0, double T1);
double T_time(double T_eq, double T0, double step_size);
int check_args(int argc, char **argv);
void print_header(FILE** p_out_file, int points);
void update_temperatures(double* temperatures, int points, double time_step);
int generate_timestamps(double* timestamps, int time_steps, double step_size);

int main(int argc, char **argv)
{
    int points = check_args(argc, argv);
    double T0 = 300, T1 = 270, T2 = 270;
    int time_steps = 20;
    double step_size = 0.5;

    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    double* temperatures = (double*) malloc(points * sizeof(double));
    temperatures[0] = T0;
    initialise_vector(temperatures, points, 270.0);
    temperatures[0] = T0;

    FILE* out_file = fopen("data/temperatures_over_time.csv", "w");
    print_header(&out_file, points);

    fprintf(out_file, "%d, %lf", 0, time_stamps[0]);
    for (int j = 0; j < points; j++)
    {
        fprintf(out_file, ", %lf", temperatures[j]);
    }
    fprintf(out_file, "\n");

    for (int i = 1; i < time_steps; i++)
    {
        update_temperatures(temperatures, points, step_size);
        fprintf(out_file, "%d, %lf", i, time_stamps[i]);
        for (int j = 0; j < points; j++)
        {
            fprintf(out_file, ", %lf", temperatures[j]);
        }
        fprintf(out_file, "\n");
    }

    free(time_stamps);
    free(temperatures);
    fclose(out_file);
    return 0;
}

void initialise_vector(double vector[], int size, double initial)
{
    #pragma omp parallel for
    for (int i = 0; i < size; i++)
    {
        vector[i] = initial;
    }
}

double T_equilibrium_global(int T0, int T1, int T2)
{
    return (T0 + T1 + T2) / 3.0;
}

double T_equilibrium(double T0, double T1)
{
    return (T0 + T1) / 2.0;
}

double T_time(double T_eq, double T0, double step_size)
{
    return T_eq + (T0 - T_eq) * exp(-step_size);
}

int check_args(int argc, char **argv)
{
    if (argc == 2)
    {
        return atoi(argv[1]);
    }
    else
    {
        fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
        fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);
        exit(-1);
    }
}

void print_header(FILE** p_out_file, int points)
{
    fprintf(*p_out_file, "#, time");
    for (int j = 0; j < points; j++)
    {
        fprintf(*p_out_file, ", y[%d]", j);
    }
    fprintf(*p_out_file, "\n");
}

void update_temperatures(double* temperatures, int points, double time_step)
{
    double* new_temperatures = (double*) malloc(points * sizeof(double));
    
    #pragma omp parallel for
    for (int i = 1; i < points - 1; i++)
    {
        double T_eq1 = T_equilibrium(temperatures[i], temperatures[i + 1]);
        double T_eq2 = T_equilibrium(temperatures[i], temperatures[i - 1]);
        double T1 = T_time(T_eq1, temperatures[i], time_step);
        double T2 = T_time(T_eq2, temperatures[i], time_step);
        new_temperatures[i] = T1 + T2 - temperatures[i];
    }

    new_temperatures[0] = T_time(T_equilibrium(temperatures[0], temperatures[1]), temperatures[0], time_step);
    new_temperatures[points - 1] = T_time(T_equilibrium(temperatures[points - 2], temperatures[points - 1]), temperatures[points - 1], time_step);

    #pragma omp parallel for
    for (int i = 0; i < points; i++)
    {
        temperatures[i] = new_temperatures[i];
    }

    free(new_temperatures);
}

int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
    #pragma omp parallel for
    for (int i = 0; i < time_steps; i++)
    {
        timestamps[i] = i * step_size;
    }
    return time_steps;
}
