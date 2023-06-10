#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

#define SHARED_MODULE
 
#define strdup _strdup


#if defined(__MINGW32__) ||  defined(_MSC_VER)
#undef BOOLEAN
#include <windows.h>
typedef FARPROC funptr_t;
void *dlopen (const char *, int);
funptr_t dlsym (void *, const char *);
int dlclose (void *);
char *dlerror (void);
#define RTLD_LAZY	1	/* lazy function call binding */
#define RTLD_NOW	2	/* immediate function call binding */
#define RTLD_GLOBAL	4	/* symbols in this dlopen'ed obj are visible to other dlopen'ed objs */
static char errstr[128];
#else
#include <dlfcn.h> /* to load libraries*/
#include <unistd.h>
#include <ctype.h>
typedef void *  funptr_t;
#endif
// #define HAS_NG_BOOL

#include "ngspice/sharedspice.h"

// int SendChar( char* what, int id, void* user) 
// {
// //     NGSPICE* sim = reinterpret_cast<NGSPICE*>( user );

// //     if( sim->m_reporter )
// //     {
// //         // strip stdout/stderr from the line
// //         if( ( strncasecmp( what, "stdout ", 7 ) == 0 )
// //                 || ( strncasecmp( what, "stderr ", 7 ) == 0 ) )
// //             what += 7;

// //         sim->m_reporter->Report( what );
// //     }
// std::cout << "cbSendChar" << std::endl;
//     return 0;
// }


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


/* Callback function called from bg thread in ngspice to transfer
   any string created by printf or puts. Output to stdout in ngspice is
   preceded by token stdout, same with stderr.*/
int
ng_getchar(char* outputreturn, int ident, void* userdata)
{
    printf("> %s\n", outputreturn);
    // /* setting a flag if an error message occurred */
    // if (ciprefix("stderr Error:", outputreturn))
    //     error_ngspice = true;
    return 0;
}

/* Callback function called from bg thread in ngspice to transfer
   simulation status (type and progress in percent). */
int
ng_getstat(char* outputreturn, int ident, void* userdata)
{
    //printf("%s\n", outputreturn);
    return 0;
}

/* Callback function called from ngspice upon starting (returns true) or
  leaving (returns false) the bg thread. */
int
ng_thread_runs(bool noruns, int ident, void* userdata)
{
    // no_bg = noruns;
    // if (noruns)
    //     printf("\nbg not running\n");
    // else
    //     printf("bg running\n\n");

    return 0;
}

/* Callback function called from bg thread in ngspice if fcn controlled_exit()
   is hit. Do not exit, but unload ngspice. */
int
ng_exit(int exitstatus, bool immediate, bool quitexit, int ident, void* userdata)
{
    /*
        if(quitexit) {
            printf("DNote: Returned from quit with exit status %d\n", exitstatus);
        }
        if(immediate) {
            printf("DNote: Unload ngspice\n");
            ((int * (*)(char*)) ngSpice_Command_handle)("quit");
            dlclose(ngdllhandle);
        }

        else {
            printf("DNote: Prepare unloading ngspice\n");
            will_unload = true;
        }
    */
    return exitstatus;

}

/* Callback function called from bg thread in ngspice once per accepted data point */
int
ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata)
{
    /*
        int *ret;

        v2dat = vdata->vecsa[vecgetnumber]->creal;
        if (!has_break && (v2dat > 0.5)) {
        // using signal SIGTERM by sending to main thread, alterp() then is run from the main thread,
        //(not on Windows though!)
    #ifndef _MSC_VER
            if (testnumber == 4)
                pthread_kill(mainthread, SIGTERM);
    #endif
            has_break = true;
        // leave bg thread for a while to allow halting it from main
    #if defined(__MINGW32__) || defined(_MSC_VER)
            Sleep (100);
    #else
            usleep (100000);
    #endif
    //        ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_halt");
        }
    */
    Command("bg_halt");
    return 0;
}


/* Callback function called from bg thread in ngspice once upon intialization
   of the simulation vectors)*/
int
ng_initdata(pvecinfoall intdata, int ident, void* userdata)
{
    /*
        int i;
        int vn = intdata->veccount;
        for (i = 0; i < vn; i++) {
            printf("Vector: %s\n", intdata->vecs[i]->vecname);
            // find the location of V(2)
            if (cieq(intdata->vecs[i]->vecname, "V(2)"))
                vecgetnumber = i;
        }
    */
    return 0;
}


int main(int argc, char** argv) {
    /*Here is an example C++ code using NGSPICE to parse and simulate a SPICE netlist file of a circuit using a random BJT transistor:*/
  
    // Initialize the NGSPICE shared library
    //LOCALE_IO c_locale;               // ngspice works correctly only with C locale
    // int  ngSpice_Init(SendChar* printfcn, SendStat* statfcn, ControlledExit* ngexit,
    //               SendData* sdata, SendInitData* sinitdata, BGThreadRunning* bgtrun, void* userData);

    //ngSpice_Init( &SendChar, &cbSendStat, &cbControlledExit, NULL, NULL, &cbBGThreadRunning, /*this*/ NULL );
    auto rret = ngSpice_Init(ng_getchar, ng_getstat,
                              ng_exit, NULL, ng_initdata, ng_thread_runs, NULL);
    bool no_bg = true;
    int vecgetnumber = 0;
    double v2dat;
    static bool has_break = false;
    int testnumber = 0;
    void alterp(int sig);
    static bool errorflag = false;

    int ret, i;
    char *curplot, *vecname;
    char ** circarray;
    char **vecarray;
    // Load the netlist file
    testnumber = 2;
    errorflag = false;
    //printf("\n**  Test no. %d with error during circuit parsing **\n\n", testnumber);

    /* create a circuit that fails due to missing include */
    circarray = (char**)malloc(sizeof(char*) * 7);
    circarray[0] = strdup("test array");
    circarray[1] = strdup("V1 1 0 1");
    circarray[2] = strdup("R1 1 2 1");
    circarray[3] = strdup("C1 2 0 1 ic=0");
    circarray[4] = strdup(".tran 10u 3 uic");
    circarray[5] = strdup(".end");
    circarray[6] = NULL;

    has_break = false;

    ret = ngSpice_Circ(circarray);
    ret = Command("bg_run");
    
    for(i = 0; i < 6; i++)
        free(circarray[i]);
    free(circarray);

 /* wait until simulation stops */
    for (;;) {
#if defined(__MINGW32__) || defined(_MSC_VER)
        Sleep (100);
#else
        usleep (100000);
#endif
        if (has_break) {
            ret = Command("bg_halt");
            ret = Command("listing");
            ret = Command("alter c1=2");
            ret = Command("bg_resume");
        }
        if (no_bg)
            break;
    }

    /* wait until simulation finishes */
    for (;;) {
#if defined(__MINGW32__) || defined(_MSC_VER)
        Sleep (100);
#else
        usleep (100000);
#endif
        if (no_bg)
            break;
    }
    ret = Command("write test3.raw V(2)");
    printf("rawfile testout3.raw created\n");    

// #ifndef _MSC_VER
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
