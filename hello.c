#define PY_SSIZE_T_CLEAN  /* Make "s#" use Py_ssize_t rather than int. */
#include <Python.h>

static PyObject *SpamError;


// Function 1: A simple 'hello world' function
static PyObject* helloworld(PyObject* self, PyObject* args)
{
    printf("Hello World\n");
    return Py_None;
}



// Function 2: A C fibonacci implementation
// this is nothing special and looks exactly
// like a normal C version of fibonacci would look
int Cfib(int n)
{
    if (n < 2)
        return n;
    else
        return Cfib(n-1)+Cfib(n-2);
}
// Our Python binding to our C function
// This will take one and only one non-keyword argument
static PyObject* fib(PyObject* self, PyObject* args)
{
    // instantiate our `n` value
    int n;
    // if our `n` value
    if(!PyArg_ParseTuple(args, "i", &n))
        return NULL;
    // return our computed fib number
    return Py_BuildValue("i", Cfib(n));
}



// Function 3
int MyPlus(int x, int y)
{
    return x + y;
}


static PyObject* myPlus(PyObject* self, PyObject* args)
{
    int x, y;
    if(!PyArg_ParseTuple(args, "ii", &x, &y))
        return NULL;
    return Py_BuildValue("i", MyPlus(x, y));
}







// Function 4
static PyObject * spam_system(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    if (sts < 0) {
        PyErr_SetString(SpamError, "System command failed");
        return NULL;
    }
    return PyLong_FromLong(sts);
}



// Fuction 5
static PyObject *my_callback = NULL;
static PyObject *my_args = NULL;

static PyObject *
my_set_callback(PyObject *self, PyObject *args)
{
    PyObject *result = NULL;
    PyObject *tempFunc;
    PyObject *tempArgs;

    if (PyArg_ParseTuple(args, "O|O:set_callback", &tempFunc, &tempArgs)) {
        if (!PyCallable_Check(tempFunc)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        /* Add a reference to new callback */
        Py_XINCREF(tempFunc);      
        Py_XINCREF(tempArgs);      
        /* Dispose of previous callback */
        Py_XDECREF(my_callback);  
        Py_XDECREF(my_args);  
        /* Remember new callback */
        my_callback = tempFunc;       
        my_args = tempArgs;       
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}




// Function 4
static PyObject * my_call_callback(PyObject *self, PyObject *args) {
    // int arg;
    // PyObject *arglist;
    PyObject *result;
    // arg = 123;
    /* Time to call the callback */
    // Py_BuildValue() returns a tuple when its format string consists of zero or more format codes between parentheses.
    // arglist = Py_BuildValue("(i)", arg);
    // The argument list must always be a tuple object, 
    // whose length is the number of arguments. 
    // To call the Python function with no arguments, pass in NULL, or an empty tuple; 
    // to call it with one argument, pass a singleton tuple.
    result = PyObject_CallObject(my_callback, my_args);
    if (result == NULL)
        return NULL; /* Pass error back */

    // Py_DECREF(arglist);
    Py_DECREF(result);
    return result;
}










// Our Module's Function Definition struct
// We require this `NULL` to signal the end of our method
// definition
static PyMethodDef myMethods[] = {
    { "helloworld", helloworld, METH_NOARGS, "Prints Hello World" },
    { "fib", fib, METH_VARARGS, "Fibonacci implementation" },
    { "plus", myPlus, METH_VARARGS, "Plus function" },
    { "system", spam_system, METH_VARARGS, "Spam system function" },
    { "set_callback", my_set_callback, METH_VARARGS, "Set Python function to be callbacked from C" },
    { "call_callback", my_call_callback, METH_NOARGS, "Call the python function from C" },
    { NULL, NULL, 0, NULL }
};






// Our Module Definition struct
static struct PyModuleDef myModule = {
    PyModuleDef_HEAD_INIT,
    "myModule", /* name of module */
    "Test Module", /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    myMethods
};

// // Initializes our module using our above struct
// PyMODINIT_FUNC PyInit_myModule(void)
// {
//     return PyModule_Create(&myModule);
// }



// The name of this initializer must contain the module name (myModule)
// This function is called by python program that imports myModule
// for the first time
PyMODINIT_FUNC PyInit_myModule(void)
{
    // The init function must return the module object to its caller, so that it then gets inserted into sys.modules.
    PyObject *m;

    // Returns a module object, and inserts built-in function objects into the newly created module
    // based upon the table (an array of PyMethodDef structures) found in the module definition.
    m = PyModule_Create(&myModule);

    if (m == NULL)
        return NULL;

    SpamError = PyErr_NewException("myModule.error", NULL, NULL);
    Py_INCREF(SpamError);
    PyModule_AddObject(m, "error", SpamError);
    return m;
}
