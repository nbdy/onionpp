from os import environ

from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = "1.0.0"

ext_modules = [
    Pybind11Extension(
        "onionpy",
        ["bindings/python_binding.cpp"],
    ),
]

setup(
    name="onionpy",
    version=__version__,
    author="Pascal Eberlein",
    author_email="pascal@eberlein.io",
    url="https://github.com/nbdy/onionpy",
    description="Python bindings for onionpp",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.8",
)
