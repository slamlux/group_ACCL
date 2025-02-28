# Group_ACCL
# Basis of the model - Thermodynamic Equilibrium in Conducting Materials
The main principle of this model is to examine the processes involved in reaching thermodynamic equilibrium in a conducting material, and to output the       equilibrium temperature. This model is based on a material experiencing heating at one end, and follows the flow of heat throughout the material. The code takes an imput which will be the number of segments in which the material is examined for temperature. The purpose of segmenting the material is to observe the behaviour of the the heat transfer over time in specific regions. The final output will be the temperature of the material once thermal equilibrium has been reached. 
# Assumptions of the model
There are a number of asumptions made when modeling this process, the first of which is to assume there is perfect contact with the heat supply, whether this be another material or otherwise. It is also assumed that there is no heat or work transfer between the environment and the conducting body. 
The model will initially be examined on a macroscopic level, for this the _First Law of Thermodynamics_ will be implemented: **ΔU = Q – W** 

It will be assumed that the work done in the system is zero, therefore the equation will become: **ΔU = Q**

Before applying the heat transfer equation, the conducting particles will be assigned some mass and specific heat capacity.

The system will be examined at a microscopic level too, this is done by segmenting the length of the conductor into smaller regions of length/area _dx_. The program will initially be modeled on a 1D system and then developed to fit a 2D system. 

Each segment will be represented by a vector. Each vector will describe qualties of the segment such as position, temperature, nearest neighbour temperature, specific heat capacity, mass, and density. The macroscopic system will be modeled using equations for thermal equilibrium between two bodies:
        T<sub>eq</sub> = (m<sub>1</sub>T<sub>1</sub> + m<sub>2</sub>T<sub>2</sub>)/(m<sub>1</sub>+m<sub>2</sub>).        

# Serial implemetation 
# Parallel implementation
# Limitations
