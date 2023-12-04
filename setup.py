from distutils.core import setup, Extension
import numpy.distutils.misc_util
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import numpy
class build_ext_arch(build_ext):
    def build_extensions(self):
        if self.compiler.compiler_type == 'unix':
            for e in self.extensions:
                e.extra_compile_args = ['-arch', 'arm64']
                e.extra_link_args = ['-arch', 'arm64']
        super().build_extensions()
        
        
        
setup(
    name='pyarome',
    version='1.0',
    description='This is a demo package',
    ext_modules=[Extension("pyarome", ["pyarome.c"],library_dirs=['/usr/local/lib/'], libraries=['arome'])],
    include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs() + ['/usr/local/include/'],
    cmdclass={
        'build_ext': build_ext_arch,
    }
)
