#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "ngspice/sharedspice.h"

int main(int argc, char** argv) {
    /*Here is an example C++ code using NGSPICE to parse and simulate a SPICE netlist file of a circuit using a random BJT transistor:*/
  
    // Initialize the NGSPICE shared library
    char* err = NULL;
    ngSpice_Init(&err);

    // Load the netlist file
    ngSpice_Command("source my_circuit.net");

    // Set up simulation parameters
    ngSpice_Command("tran 0.1ns 10ns");

    // Run the simulation
    ngSpice_Command("run");

    // Get the simulation results
    int num_points = ngGet_Vec_Info("V(out)", NULL, NULL);
    double* time = new double[num_points];
    double* voltage = new double[num_points];
    ngGet_Vec_Time("V(out)", time);
    ngGet_Vec_Values("V(out)", voltage);

    // Print the simulation results
    printf("Time (ns)\tVoltage (V)\n");
    for (int i = 0; i < num_points; i++) {
        printf("%f\t%f\n", time[i], voltage[i]);
    }

    // Clean up memory and close NGSPICE
    delete[] time;
    delete[] voltage;
    ngSpice_Command("quit();");
    return 0;
}

/*
In this example code, we first initialize the NGSPICE shared library using `ngSpice_Init()`. We then load the SPICE netlist file using `ngSpice_Command()`.
Next, we set up the simulation parameters using `ngSpice_Command()`. In this example, we set up a transient analysis, which simulates the circuit behavior over time.
We then run the simulation using `ngSpice_Command()`. After the simulation is complete, we retrieve the simulation results using `ngGet_Vec_Info()`, `ngGet_Vec_Time()`, and `ngGet_Vec_Values()`. In this example, we retrieve the voltage at the output node of the circuit.
Finally, we print the simulation results to the console and clean up memory using `delete[]` and `ngSpice_Command()`.
Note that the example code assumes that the SPICE netlist file is named `my_circuit.net` and is located in the current directory. Additionally, the example code assumes that the netlist file includes a BJT transistor named `Q1` and an output node named `out`. You will need to modify the code to match the specific circuit you are simulating.
*/
