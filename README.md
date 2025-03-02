# Group_ACCL
# Basis of the model - Thermodynamic Equilibrium in Conducting Materials
The main principle of this model is to examine the processes involved in reaching thermodynamic equilibrium in a conducting material, and to output the equilibrium temperature. This model is based on a material experiencing heating at one end, and follows the flow of heat throughout the material. The code takes an input which will be the number of segments in which the material is examined for temperature. The purpose of segmenting the material is to observe the behaviour of the the heat transfer over time in specific regions. The final output will be the temperature of the material once thermal equilibrium has been reached. To run this code a directory titled "data" must be established. All results from this program will be stored within this directory.
# Assumptions of the model
There are a number of asumptions made when modeling this process, the first of which is to assume there is perfect contact with the heat supply, whether this be another material or otherwise. It is also assumed that there is no heat or work transfer between the environment and the conducting body. 
The model will initially be examined on a macroscopic level, for this the _First Law of Thermodynamics_ will be implemented: **ΔU = Q – W** 

It will be assumed that the work done in the system is zero, therefore the equation will become: **ΔU = Q**

Before applying the heat transfer equation, the conducting particles will be assigned some mass and specific heat capacity.

The system's topologie will be examined at a microscopic level too, this is done by partitioning the length of the conductor into smaller regions of length/area _dx_. The program will initially be modeled on a 1D system and then developed to fit a 2D system. 

Each segment will be represented by a vector. Each vector will describe qualties of the segment such as position, temperature, nearest neighbour temperature, specific heat capacity, mass, and density. The macroscopic system will be modeled using equations for thermal equilibrium between two bodies:
 **T<sub>eq</sub> = (m<sub>1</sub>T<sub>1</sub> + m<sub>2</sub>T<sub>2</sub>)/(m<sub>1</sub>+m<sub>2</sub>).**   

 Temperature transfer over time will be modelled using the following equation:
**T<sub>n(t)</sub> = T<sub>final</sub> + ( T<sub>n(0)</sub> - T<sub>final</sub>)e<sup>(-kt)</sup>**

Initial conditions such as mass and specific heat capacity can be altered to simulate a variety of conducting materials.

# Serial implemetation 
## Main steps in program 

**Initialise_vector** - Used to initialise all values in the vector to a set value. These values are then iterated through the vector size and the elements of the vector are then set to the initial vector size. 

**T_equilibrium_global** - This function takes an average of the temperature values for a set number of segments and produces the global equilibrium temperature. 

**T_equilibrium** - Finds the equilibrium temperature between two individual neighbouring segments.

**T_time** - This is used to create a timestamp for temperature measurements.

**check_args** - This function will declare and initialise the given numerical argument and determine if the argument is within the allowed range of values, if not, the program is terminated and an error is raised, otherwise num_arg is returned and the program runs as expected.

**print_header** - Prints a header to a file. A double-pointer is featured to allow movement of the file pointer by the function.

**update_temperatures** -  Used to define a function which will update the positions of the vector. A temporary vector is created for the new position of the particle which is inevitabley overwritten after implementing formulas to determine accurate position vectors. The temporary vector is freed once the new position vector is computed.

**generate_timestamps** - Loops through the step size and defines a set of timestamps.

**main()** - This function performs the main calculations. All previously defined values are called and the equilibrium temperature is defined. The equilibrium temperature is ouputted and saved as a csv file which can then be used to create a graph describing the temperature of the conductor over time.

## Bench marking
For serial implementation there is little between real, user and system times. The real time it takes for the system to run with 10 elements is 0.012 s, for user time 0.005, 0.001. For 2 elements the real, user and system times wer 0.011s, 0.005s and <0.000s respectively.For 1000 elements the the real, user and system times were 0.047s, 0.014 and 0.005s respectively. Notice the greatest increase in the real time while the system times remains low. At 200,000 elements the processing times starts to become observably slower with real, user and system times of 3.128s , 2.185s and 0.185s.A significant increase in processing time is then observed. With 1 million elements in the simulation the real, user and systems times take 11.962 s, 10.852s, and 0.744s . These times for 10 million are 118.559s, 108.096s and 8.566 s. The time doubled for 20 million with times of 233.66s, 216.489s and 16.4s.The real and user times are much greater than the system times. 16.4 seconds is long for a system time. By graphing these results, the processing time of the serial system appears to increase linearly. To conclude this serially implemented system appears to be designed for use cases when the number of elements are one-hundred-thousand or less.

# Parallel implementation
# Limitations
The main limitations to this model are due to the assumptions made when writing the program These assumptions stray from real life scenarios in which one would expect to experience when examining a body experiencing heating. The main limitation is that it is assumed there is no work done or heat loss in the system. This can happen in some circumstances but it is very rare. This means the results obtained can not be said to be fully representative of typical heating systems. 

The program is based on separate segments all experiencing the same heating, again, this is not fully representative of real life scenarios. Realistically, there are no segmented confines in which particles are being heated in a conductor. Heat transfer may actually happen faster or slower for some particles than is modelled in this program. 

The growth of the data file when increasing the number of elements being computed can limit the effectivness of this program. For most applications it is desireable to have a very fast program that allows results to be obtained in seconds regardless of the number of iterations or elements in the system. This is not the case for the serial implementation of the code as can be seen from the benchmarking testing. Large numbers of elements require much larger computational times and output much larger data files which may be impractical.
