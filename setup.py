from setuptools import setup
from os import getcwd, system
from pybind11.setup_helpers import Pybind11Extension, build_ext


def build_tor():
    print("Current working directory:", getcwd())
    system("tor_setup_unix.sh")


class OnionPPExtension(Pybind11Extension):
    def __init__(self):
        build_tor()

        Pybind11Extension.__init__(
            self,
            "onionpp",
            ["python_binding.cpp"],
            library_dirs=[
                "tor/src/core",
                "tor/src/lib",
                "tor/src/ext/keccak-tiny",
                "tor/src/ext/ed25519/ref10",
                "tor/src/ext/ed25519/donna",
                "tor/src/trunnel"
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
                "ssl", "crypto", "event", "lzma", "zstd"
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
