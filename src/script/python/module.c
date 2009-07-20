#include "module.h"
#include "Python.h"

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


/**** Interface declaration ****/

static PyMethodDef exportPythonMethods[] = 
{
	{"initialize",  Smug_initialize, METH_VARARGS, "Initialize the engine."},
	{"run",  Smug_run, METH_VARARGS, "Run the engine, will return when engine is killed."},
	{"terminate",  Smug_terminate, METH_VARARGS, "Cleans up resources."},
};

PyMODINIT_FUNC initsmug()
{
    PyObject* m;

	// Do not fucking forget these, Python won't motherfucking tell you what is wrong if you do.
    /*if (PyType_Ready(&Smug_GameObject_type) < 0)
        return;
    if (PyType_Ready(&Smug_InputData_type) < 0)
        return;
    if (PyType_Ready(&Smug_CollisionData_type) < 0)
        return;*/

    m = Py_InitModule("smug", exportPythonMethods);

/*
    Py_INCREF(&Smug_GameObject_type);
    PyModule_AddObject(m, "GameObject", (PyObject*)&Smug_GameObject_type);
    Py_INCREF(&Smug_InputData_type);
    PyModule_AddObject(m, "InputData", (PyObject*)&Smug_InputData_type);
    Py_INCREF(&Smug_CollisionData_type);
    PyModule_AddObject(m, "CollisionData", (PyObject*)&Smug_CollisionData_type);*/

}

int initPythonModule()
{
	initsmug();
	return 0;
}
