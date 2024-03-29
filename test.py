import sys
from time import sleep

sys.path.append("cmake-build-debug")

from onionpy import Tor, TorConfiguration

if __name__ == '__main__':
    tor = Tor()
    hashed_password = tor.hash_password("Secret")
    print(hashed_password)

    cfg = TorConfiguration()
    print(cfg.control_port)
    cfg.control_port_enabled = True
    cfg.control_password_authentication_enabled = True
    cfg.hashed_control_password = hashed_password

    tor.start(cfg)

    print("Sleeping 5 seconds to ensure the control port is open")
    sleep(5)

'''
    print("Using stem to control tor")
    with Controller.from_port(port=cfg.control_port) as ctrl:
        ctrl.authenticate(password="Secret")
        print("Controller version:", ctrl.get_version())
'''
