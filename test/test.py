import sys

sys.path.append("/home/nbdy/Dokumente/Project/CLion/onionpp/cmake-build-debug")

from onionpy import Tor

def main():
    tor = Tor()
    tor.start(True)


if __name__ == '__main__':
    main()
