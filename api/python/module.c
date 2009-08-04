#include "module.h"
#include "Python.h"

#include "smug.h"

/**** Exported Function Declarations ****/

static PyObject* Pysmug_initialize(PyObject* self, PyObject* args)
{
    if (!smugInit())
    {
        fprintf(stderr, "Could not initialize engine, exiting...\n");
        return NULL;
    }
    return Py_BuildValue("");
}


static PyObject* Pysmug_run(PyObject* self, PyObject* args)
{
    smugRun();
    return Py_BuildValue("");
}


static PyObject* Pysmug_terminate(PyObject* self, PyObject* args)
{
    smugTerminate();
    return Py_BuildValue("");
}

static PyObject* Pysmug_testing(PyObject* self, PyObject* args)
{
    return Py_BuildValue("");
}

/**** Interface declaration ****/

static PyMethodDef exportPythonMethods[] = 
{
    {"initialize",  Pysmug_initialize, METH_VARARGS, "Initialize the engine."},
    {"run",  Pysmug_run, METH_VARARGS, "Run the engine, will return when engine is killed."},
    {"terminate",  Pysmug_terminate, METH_VARARGS, "Cleans up resources."},
    {NULL, NULL, 0, NULL}
};

static PyMethodDef exportPrivateMethods[] = 
{
    {"iamprivate", Pysmug_testing, METH_VARARGS, "Testing some stuff."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initsmug()
{
    PyObject* m;

    // Do not fucking forget these, Python won't motherfucking tell you what is wrong if you do.
//    if (PyType_Ready(&Pysmug_Color_type) < 0)
//        return;
    // if (PyType_Ready(&Pysmug_InputData_type) < 0)
        // return;
    // if (PyType_Ready(&Pysmug_CollisionData_type) < 0)
        // return;

    m = Py_InitModule("private", exportPrivateMethods);
    m = Py_InitModule("smug", exportPythonMethods);

//    Pysmug_GameObject_type.tp_new = PyType_GenericNew;
//    Py_INCREF(&Pysmug_Color_type);
//    PyModule_AddObject(m, "Color", (PyObject*)&Pysmug_Color_type);
    // Py_INCREF(&Pysmug_InputData_type);
    // PyModule_AddObject(m, "InputData", (PyObject*)&Pysmug_InputData_type);
    // Py_INCREF(&Pysmug_CollisionData_type);
    // PyModule_AddObject(m, "CollisionData", (PyObject*)&Pysmug_CollisionData_type);

}

int initPysmug()
{
    initsmug();
    return 0;
}
