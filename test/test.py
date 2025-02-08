from onionpp import Tor

def main():
    tor = Tor()
    tor.start(True)


if __name__ == '__main__':
    main()
