#include "script.h"
#include "Python.h"

#include "python/module.h"
#include "common/log.h"

PyObject* gName;
PyObject* gModule;
PyObject* gDict;
PyObject* gValue;
PyObject* gMain;

int Script_init(const char* scriptfile)
{
    Py_Initialize();
    
    initPythonModule();
  
    gName = PyString_FromString(scriptfile);
    
    if (!(gModule = PyImport_Import(gName)))
    {
        ERROR("Error importing python module.");
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

void Script_terminate()
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

void Script_run()
{
    PyObject_CallObject(gMain, NULL);
}

