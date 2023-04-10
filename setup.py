from setuptools import setup
from os import getcwd, system
from os import makedirs, chdir
from os.path import isdir
from pybind11.setup_helpers import Pybind11Extension, build_ext


def build_tor(build_directory: str):
    if not isdir(build_directory):
        makedirs(build_directory)

    chdir(build_directory)

    system("cmake ..")
    system("make -j$(nproc)")

    chdir("..")


class OnionPPExtension(Pybind11Extension):
    def __init__(self, build_directory: str = "build"):
        build_tor(build_directory)
        include_directory = build_directory + "/tor/src/tor/src"

        Pybind11Extension.__init__(
            self,
            "onionpp",
            ["python_binding.cpp"],
            extra_compile_args=["-fPIC"],
            extra_link_args=["-fPIC"],
            include_dirs=[
                include_directory
            ],
            library_dirs=[
                include_directory + "/core",
                include_directory + "/lib",
                include_directory + "/ext/keccak-tiny",
                include_directory + "/ext/ed25519/ref10",
                include_directory + "/ext/ed25519/donna",
                include_directory + "/trunnel"
            ],
            libraries=[
                "tor-app",
                "tor-compress", "tor-evloop", "tor-tls",
                "tor-crypt-ops", "curve25519_donna", "tor-geoip",
                "tor-time", "tor-fs", "tor-encoding", "tor-sandbox",
                "tor-net", "tor-memarea", "tor-math", "tor-meminfo",
                "tor-osinfo", "tor-log", "tor-lock", "tor-fdio",
                "tor-string", "tor-term", "tor-smartlist-core",
                "tor-malloc", "tor-wallclock", "tor-err",
                "tor-intmath", "tor-ctime", "tor-trace",
                "tor-buf", "tor-confmgt", "tor-pubsub",
                "tor-metrics", "tor-dispatch", "tor-version",
                "tor-thread", "tor-container", "tor-process",
                "tor-llharden", "keccak-tiny", "ed25519_ref10",
                "ed25519_donna", "or-trunnel",
                "ssl", "crypto", "event", "lzma"
            ],
            language="c++"
        )


setup(
    name="onionpp",
    version="0.12",
    author="Pascal Eberlein",
    author_email="pascal@eberlein.io",
    url="https://github.com/nbdy/onionpp",
    description="embed tor into your application",
    long_description="",
    ext_modules=[OnionPPExtension()],
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.6",
    headers=["onionpp.h"]
)
