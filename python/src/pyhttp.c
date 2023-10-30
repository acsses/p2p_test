#include <Python.h>


extern int requestHttpGET(char buff[], int buf_size,char * URL,int is_local);

//extern int requestHttpsGET(char buff[], int buf_size,char * URL);

//extern int requestHttpPOST(char buff[], int buf_size,char * URL,int is_local,char header[],char body[]);
//
//extern int requestHttpsPOST(char buff[], int buf_size,char * URL,char header[],char body[]);


static PyObject* reqGET(PyObject* self, PyObject* args){
    char buff[2048];

    char url[256];
    int is_local;

    if (!PyArg_ParseTuple(args, "si", &url, &is_local)){
        return NULL;
    }

    requestHttpGET(buff,2048,url,is_local);
    return Py_BuildValue("s", buff);
};

//static PyObject* reqGETs(PyObject* self, PyObject* args){
//    char buff[2048];
//
//    char url[256];
//    int is_local;
//
//    if (!PyArg_ParseTuple(args, "s", &url)){
//        return NULL;
//    }
//
//    requestHttpsGET(buff,2048,url,is_local);
//    return Py_BuildValue("s", buff);
//};
//



static PyMethodDef http_methods[] = {
    {"requestHttpGET", reqGET, METH_VARARGS},
    {NULL},
};

static struct PyModuleDef http = {
    PyModuleDef_HEAD_INIT,
    "http",
    "Python3 C API Module for http and https request",
    -1,
    http_methods
};

//module creator
PyMODINIT_FUNC PyInit_hello(void)
{
    return PyModule_Create(&http);
}