#!/usr/bin/env python

import time
import x10

def main():
    x10.sendCommands('/dev/tty.usbserial', 'N2 Off')
    time.sleep(3)
    x10.sendCommands('/dev/tty.usbserial', 'N2 On')

if __name__ == '__main__':
    main()