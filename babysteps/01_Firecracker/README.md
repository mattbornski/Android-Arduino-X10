To prove that the X10 system and "firecracker" controller work (before involving an Arduino, Android, and network interfaces), do this:

1. Plug in a lamp that you can see to an X10 blob.

2. Plug in the X10 transceiver blob somewhere close by.  To minimize complications, it should preferably be nearby to you and the other blob, and possibly on the same circuit.

3. Install the python-serial package on your development machine (pip install pyserial worked for me)

4. Plug in the "firecracker" module to your development machine, and get it operational.  I was using a USB to serial converter on Mac OS X, and had to install a third-party driver from http://www.prolific.com.tw/eng/downloads.asp?id=31 and restart.

5. Tweak "lamp.py" to specify your serial port and lamp module number.  Run it.  If the lamp flips state, your firecracker module and X10 modules work.