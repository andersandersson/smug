#include "module.h"
#include "Python.h"

#include "smug.h"
#include "engine/engine.h"

/**** Exported Function Declarations ****/

static PyObject* Smug_initialize(PyObject* self, PyObject* args)
{
    if (!Engine_init())
    {
        fprintf(stderr, "Could not initialize engine, exiting...\n");
        return NULL;
    }
    return Py_BuildValue("");
}


static PyObject* Smug_run(PyObject* self, PyObject* args)
{
    Engine_run();
    return Py_BuildValue("");
}


static PyObject* Smug_terminate(PyObject* self, PyObject* args)
{
    Engine_terminate();
    return Py_BuildValue("");
}

static PyObject* Smug_testing(PyObject* self, PyObject* args)
{
    return Py_BuildValue("");
}

/**** Interface declaration ****/

static PyMethodDef exportPythonMethods[] = 
{
    {"initialize",  Smug_initialize, METH_VARARGS, "Initialize the engine."},
    {"run",  Smug_run, METH_VARARGS, "Run the engine, will return when engine is killed."},
    {"terminate",  Smug_terminate, METH_VARARGS, "Cleans up resources."},
    {NULL, NULL, 0, NULL}
};

static PyMethodDef exportPrivateMethods[] = 
{
    {"iamprivate", Smug_testing, METH_VARARGS, "Testing some stuff."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initsmug()
{
    PyObject* m;

    // Do not fucking forget these, Python won't motherfucking tell you what is wrong if you do.
//    if (PyType_Ready(&Smug_Color_type) < 0)
//        return;
    // if (PyType_Ready(&Smug_InputData_type) < 0)
        // return;
    // if (PyType_Ready(&Smug_CollisionData_type) < 0)
        // return;

    m = Py_InitModule("private", exportPrivateMethods);
    m = Py_InitModule("smug", exportPythonMethods);

//    Smug_GameObject_type.tp_new = PyType_GenericNew;
//    Py_INCREF(&Smug_Color_type);
//    PyModule_AddObject(m, "Color", (PyObject*)&Smug_Color_type);
    // Py_INCREF(&Smug_InputData_type);
    // PyModule_AddObject(m, "InputData", (PyObject*)&Smug_InputData_type);
    // Py_INCREF(&Smug_CollisionData_type);
    // PyModule_AddObject(m, "CollisionData", (PyObject*)&Smug_CollisionData_type);

}

int initPythonModule()
{
    initsmug();
    return 0;
}
