from distutils.core import setup, Extension
import numpy.distutils.misc_util

setup(
    ext_modules=[Extension("pyarome", ["pyarome.c"],library_dirs=['/usr/local/lib/'], libraries=['arome'])],
    include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs() + ['/usr/local/include/'],
)
