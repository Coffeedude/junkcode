##
## Simple demo module for experiementing with Python extension libraries
##
## Demo1
##
## Author: jerry@plainjoe.org
##

from distutils.core import setup, Extension

demo1 = Extension(
            'demo1',
            sources = [ 'demo1.c' ])

setup(
    name = 'FirstPythonExtensionDemo',
    version = '0.01',
    description = 'Simple demonstration of a Python extension library',
    ext_modules = [demo1])
