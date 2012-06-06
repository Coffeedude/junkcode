// -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-
// ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4:
// Editor Settings: expandtabs and use 4 spaces for indentation


//
// Simple demo module for experiementing with Python extension libraries
//
// Demo1
//
// Author: jerry@plainjoe.org
//

#include <Python.h>

static
PyObject*
Demo1_Multiply(
    PyObject *Self,
    PyObject *Args
    );

static PyMethodDef Demo1_MethodList[] = {
    { "multiply", Demo1_Multiply, METH_VARARGS, "Multiply two integers." },
    { NULL, NULL, 0, NULL }
};


////////////////////////////////////////////////////////////////////////

PyMODINIT_FUNC
initdemo1(
    void
    )
{
    PyObject *module = NULL;

    module = Py_InitModule("demo1", Demo1_MethodList);
    if (module == NULL)
    {
        return;
    }

    return;
}

////////////////////////////////////////////////////////////////////////

static
PyObject*
Demo1_Multiply(
    PyObject *Self,
    PyObject *Args
    )
{
    int a = 0;
    int b = 0;
    int result = 0;

    if (!PyArg_ParseTuple(Args, "ii", &a, &b))
    {
        return NULL;
    }

    result = a * b;

    return Py_BuildValue("i", result);
}

