#include <iostream>
#define SHARED_MODULE
// #define HAS_NG_BOOL

// typedef bool NG_BOOL;
#include "ngspice/sharedspice.h"

using namespace std;

int cbSendChar( char* what, int id, void* user )
{
//     NGSPICE* sim = reinterpret_cast<NGSPICE*>( user );

//     if( sim->m_reporter )
//     {
//         // strip stdout/stderr from the line
//         if( ( strncasecmp( what, "stdout ", 7 ) == 0 )
//                 || ( strncasecmp( what, "stderr ", 7 ) == 0 ) )
//             what += 7;

//         sim->m_reporter->Report( what );
//     }
std::cout << "cbSendChar" << std::endl;
    return 0;
}


int cbControlledExit( int status, bool immediate, bool exit_upon_quit, int id, void* user )
{
    // Something went wrong, reload the dll
    //NGSPICE* sim = reinterpret_cast<NGSPICE*>( user );
    //sim->m_initialized = false;
    //printf("stat %d immed %d quit %d\n", status, !!immediate, !!exit_upon_quit);
std::cout << "cbControlledExit" << std::endl;
    return 0;
}

int cbSendStat( char* what, int id, void* user )
{
/*    NGSPICE* sim = reinterpret_cast<NGSPICE*>( user );
    if( sim->m_consoleReporter )
        sim->m_consoleReporter->Report( what );*/
std::cout << "cbSendStat" << std::endl;
    return 0;
}

int cbBGThreadRunning( bool is_running, int id, void* user )
{
    // NGSPICE* sim = reinterpret_cast<NGSPICE*>( user );

    // if( sim->m_reporter )
    //     // I know the test below seems like an error, but well, it works somehow..
    //     sim->m_reporter->OnSimStateChange( sim, is_running ? SIM_IDLE : SIM_RUNNING );
    std::cout << "cbBGThreadRunning" << std::endl;
    return 0;
}

int Command( const std::string& aCmd )
{
    //LOCALE_IO c_locale;               // ngspice works correctly only with C locale
    return ngSpice_Command( (char*) aCmd.c_str() );
}

pvector_info Get_Vec_Info( const std::string& vecname ) {
    return ngGet_Vec_Info((char*) vecname.c_str());
}

int main(int argc, char** argv) {
    /*Here is an example C++ code using NGSPICE to parse and simulate a SPICE netlist file of a circuit using a random BJT transistor:*/
  
    // Initialize the NGSPICE shared library
    //LOCALE_IO c_locale;               // ngspice works correctly only with C locale
    ngSpice_Init( &cbSendChar, &cbSendStat, &cbControlledExit, NULL, NULL, &cbBGThreadRunning, /*this*/ NULL );

    // Load the netlist file
    Command("v1 1 0 dc 5\nr1 1 2 1k\nr2 2 0 2k\n.tran 1us 10us");
    

    //ngSpice_Circ();

    // // // Run the simulation
    // Command("run");

    // // Get the simulation results
    auto vector_info = Get_Vec_Info("V(out)");
    // double* time = new double[num_points];
    // double* voltage = new double[num_points];
    // ngGet_Vec_Time("V(out)", time);
    // ngGet_Vec_Values("V(out)", voltage);

    // Print the simulation results
    std::cout << "Time (ns)\tVoltage (V)\n" << std::endl;
    std::cout << vector_info->v_name << std::endl;
    // for (int i = 0; i < num_points; i++) {
    //     std::cout << time[i] << ", " << voltage[i] << std::endl;
    // }

    // // Clean up memory and close NGSPICE
    // delete[] time;
    // delete[] voltage;
    Command("quit();");
    return 0;
}

/*
In this example code, we first initialize the NGSPICE shared library using `ngSpice_Init()`. We then load the SPICE netlist file using `ngSpice_Command()`.
Next, we set up the simulation parameters using `ngSpice_Command()`. In this example, we set up a transient analysis, which simulates the circuit behavior over time.
We then run the simulation using `ngSpice_Command()`. After the simulation is complete, we retrieve the simulation results using `ngGet_Vec_Info()`, `ngGet_Vec_Time()`, and `ngGet_Vec_Values()`. In this example, we retrieve the voltage at the output node of the circuit.
Finally, we print the simulation results to the console and clean up memory using `delete[]` and `ngSpice_Command()`.
Note that the example code assumes that the SPICE netlist file is named `my_circuit.net` and is located in the current directory. Additionally, the example code assumes that the netlist file includes a BJT transistor named `Q1` and an output node named `out`. You will need to modify the code to match the specific circuit you are simulating.
*/
