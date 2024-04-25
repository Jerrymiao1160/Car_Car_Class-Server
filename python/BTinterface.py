import logging
import time
import threading
from typing import Optional

from BT import Bluetooth

log = logging.getLogger(__name__)

# hint: You may design additional functions to execute the input command,
# which will be helpful when debugging :)


class BTInterface:
    def __init__(self, port: Optional[str] = None):
        log.info("Arduino Bluetooth Connect Program.")
        self.bt = Bluetooth()
        if port is None:
            port = input("PC bluetooth port name: ")
        while not self.bt.do_connect(port):
            if port == "quit":
                self.bt.disconnect()
                quit()
            port = input("PC bluetooth port name: ")

    def start(self):
        input("Press enter to start.")
        self.bt.serial_write_string("s")

    def get_UID(self):
        return self.bt.serial_read_byte()

    def end_process(self):
        self.bt.serial_write_string("e")
        self.bt.disconnect()


if __name__ == "__main__":
    test = BTInterface()
    test.start()
    while True:
        readThread = threading.Thread(target=test.bt.serial_read_byte)
        readThread.daemon = True
        readThread.start()
    test.end_process()
