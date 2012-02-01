Connect the CM17A "Firecracker" module to the Arduino and demonstrate that the wiring is good and the Arduino can control the X10 modules via the Firecracker.

Wire the following pins from the female DB9 connection of the Firecracker to the digital I/O pins of the Arduino:
 - pin 5 (ground) to GND
 - pin 4 (data terminal ready - DTR) to any free digital I/O pin (I used pin 4)
 - pin 7 (request to send - RTS) to any free digital I/O pin (I used pin 7)
Note that while the Firecracker uses a serial port it doesn't really use the serial protocol, and the other pins are unnecessary.

Pin layout of a female DB9 connector (stolen from http://tuxas.net/lazydoc/easyplugs/)
    ________________________
   /                       /|
  /                       / | 
 /-----------------------/  |
|    ------------------  |  |
|   /-----------------/  |  |
|   \ /5 /4 /3 /2 /1 /   |  |
| *  \              /  * |  |
|     \ /9 /8 /7 /6/     | /
|      ------------      |/
 ------------------------/

Place the modified X10 Firecracker library in the correct spot in your Arduino devtools installation.
SKETCHBOOK=~/Documents/Arduino
mkdir -p $SKETCHBOOK/libraries
cp -R libraries/* $SKETCHBOOK/libraries

Open the Arduino sketch (.ino file) in your Arduino tools.  You may have to quit and reload the devtools if they were open so that the new library is located.  Upload the sketch to your properly wired board.

Connect your Arduino to a power source and your network.  Navigate to "x10.local" from a Bonjour/Zeroconf enabled machine *on the same subnet*.  Note that many wireless routers do not bridge wireless/wired networks for multicast purposes (required for Zeroconf) and if you are experiencing difficulties, try plugging both the Arduino and your development machine into the same wired ethernet router.