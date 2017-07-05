#include <Python.h>
#include <numpy/arrayobject.h>
#include <arome.h>

static char module_docstring[] =
    "This module provides a basic Python interface to the ARoME library.";
static char RM_docstring[] =
    "Calculate the RM velocity for CCF + iodine scenarios given orbital parameters and times";

static PyObject *pyarome_RM(PyObject *self,PyObject *args);

static PyMethodDef module_methods[] = {
    {"RM", pyarome_RM, METH_VARARGS, RM_docstring},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initpyarome(void)
{
    PyObject *m = Py_InitModule3("pyarome", module_methods, module_docstring);
    if (m == NULL)
        return;

    /* Load `numpy` functionality. */
    import_array();
}

  

static PyObject *pyarome_RM(PyObject *self,PyObject *args){

  PyObject *t_obj;
  double P, i, e, omega, t0, tc, a, lambda;
  double u1, u2;
  double beta0, Vsini, sigma0, zeta, Kmax;
  double Rp;
  int status = 0;
  int l;

  if (!PyArg_ParseTuple(args, "dOdddddddddddddd", &lambda, &t_obj, &P, &i, &e,
			&omega, &tc, &a, &u1, &u2, &beta0, &Vsini, &sigma0, &zeta, &Kmax, &Rp))
    return NULL;

  PyObject *t_array = PyArray_FROM_OTF(t_obj, NPY_DOUBLE, NPY_IN_ARRAY);

  /* If that didn't work, throw an exception. */
  if (t_array == NULL) {
    Py_XDECREF(t_array);
    return NULL;
  }

  /* How many data points are there? */
  int N = (int)PyArray_DIM(t_array, 0);
  double tab_x[N], tab_y[N], tab_z[N];
  double tab_v_CCF[N], tab_v_iodine[N];
  
  /* Get pointers to the data as C-types. */
  double *t    = (double*)PyArray_DATA(t_array);

  /* Call the ARoME C function to compute the RM */
  /* 1.- obtain x,y,z given times of transit and orbital paramaters */
  t_orbit *orbit = arome_new_orbit();
  status += arome_set_orbit_eo(P, a, e, omega, i, lambda, orbit);
  t0      = arome_get_orbit_transit_time(orbit);
  for (l=0;l<N;l++){
    /*printf("%f ",t[l]);*/
    t[l] = t[l] - tc + t0;
    /*printf("%f\n",t[l]);*/
  }
  status += arome_mget_orbit_xyz(orbit, t, N, tab_x, tab_y, tab_z);

  /*printf("%f %f %d\n",tc,t0, N);
    printf("i=%f\n",i); */


  /* Calcualte RM effect, three ways */

  t_arome *parome  = arome_alloc_quad(u1,u2);
  status += arome_set_qtrap_EPS(1.0e-4, parome);
  status += arome_set_lineprofile(beta0, Vsini, sigma0, zeta, Kmax, parome);

  if (status) exit(EXIT_FAILURE);

  status += arome_set_planet(Rp, parome);

  status += arome_init_CCF(parome);
  status += arome_init_iodine(parome);

  status += arome_malloc(N, parome);
  status += arome_mcalc_fvpbetap(tab_x, tab_y, tab_z, N, parome);

  /* get the RM signals */
  status += arome_mget_RM_CCF(parome, N, tab_v_CCF);
  status += arome_mget_RM_iodine(parome, N, tab_v_iodine);
  
  if (status) {
    PyErr_SetString(PyExc_RuntimeError,
		    "ARoME returned status != 0");
    return NULL;
  }
  
  int nd = 1;
  npy_intp *dims = PyDimMem_NEW(1);
  dims[0] = N;
				     
  /* Build return values */

  PyArrayObject *CCF_out = (PyArrayObject *) PyArray_SimpleNew(nd,dims,NPY_DOUBLE);
  PyArrayObject *iodine_out = (PyArrayObject *) PyArray_SimpleNew(nd,dims,NPY_DOUBLE);

  /* get pointers to data of Objects above */
  double *ccf_out_p    = (double*)PyArray_DATA(CCF_out);
  double *iodine_out_p    = (double*)PyArray_DATA(iodine_out);
  
  /* copy arrays */

  for (l=0;l<N;l++){
    ccf_out_p[l] = tab_v_CCF[l];
    iodine_out_p[l] = tab_v_iodine[l];
    /*printf("%f %f %f | %f %f\n",tab_x[l],tab_y[l],tab_z[l],tab_v_CCF[l],tab_v_iodine[l]);*/
  }

  /* Clean up. */
  Py_DECREF(t_array);
  PyDimMem_FREE(dims);

  arome_mfree(parome);
  arome_free(parome);
   
  /* return */
  PyObject *ret = Py_BuildValue("OOd", CCF_out,iodine_out,t0);
  return ret;
  
}
