from setuptools import setup, Extension

setup(
    name="transversal_enumeration",
    ext_modules=[Extension("transversal_enumeration", ["test.c"])]
)