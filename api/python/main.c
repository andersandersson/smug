#include "module.h"
#include "Python.h"
#include <stdio.h>

PyObject* gName;
PyObject* gModule;
PyObject* gDict;
PyObject* gValue;
PyObject* gMain;

int init(const char* scriptfile)
{
    Py_Initialize();
    
    initPysmug();
  
    gName = PyString_FromString(scriptfile);
    
    if (!(gModule = PyImport_Import(gName)))
    {
        fprintf(stderr, "Error importing python module.");
        PyErr_Print();
        return 0;
    }
    gDict = PyModule_GetDict(gModule);

    gMain = PyDict_GetItemString(gDict, "main");
    
    if (!PyCallable_Check(gMain)) 
    {
        PyErr_Print();
        fprintf(stderr, "Could not find 'main' python function.\n");
    }

    return 1;
}

void terminate()
{
    if (gModule) 
    {
        Py_DECREF(gModule);
    }
    if (gName)
    {
        Py_DECREF(gName);
    }
    
    Py_Finalize();
}

void run()
{
    PyObject_CallObject(gMain, NULL);
}


int main(int argc, char* argv[])
{
    // Not the cutest solution, but works for me?
    // Should probably be removed or replaced with some
    // define from configure
    putenv("PYTHONPATH=../../python/tests:../../python/games");
    
    if (argc != 2)
    {
        fprintf(stderr, "Please provide script file\n");
        return 0;    
    }
    
    if (!init(argv[1]))
    {
        fprintf(stderr, "Could not initialize script, exiting...\n");
        terminate();
        return 0;
    }

    run();
    terminate();
    
    return 0;
}

/*! @mainpage Smug - Scripted Multithreaded Utility for Games
 *
 * @section intro_sec Introduction
 *
 * This is the introduction.
 *
 * @section install_sec Installation
 *
 * @subsection step1 Step 1: Opening the box
 *  
 * etc...
 */

