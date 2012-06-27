#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <util.h>

//#include <EthernetBonjour.h>

#include <X10Firecracker.h>

#define RTS_PIN     7                   // RTS line for C17A - DB9 pin 7
#define DTR_PIN     4                   // DTR line for C17A - DB9 pin 4

byte mac[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
// I can't configure my router to bridge Bonjour from wireless to wired subnets
// and most of the devices on my network are on the wireless, but the arduino
// is on the wired.  I furthermore don't have a local network DNS server, and
// can't get my router to assign a static IP to the same mac address.  This is
// a hack.
byte ip[] = { 192, 168, 1, 77 };

EthernetServer server = EthernetServer(80);

void setup()
{
  Ethernet.begin(mac, ip);
  /*EthernetBonjour.begin("x10");
  EthernetBonjour.addServiceRecord("Arduino Bonjour Webserver Example._http",
                                   80,
                                   MDNSServiceTCP);*/
  X10.init( RTS_PIN, DTR_PIN );
}

void loop()
{
  // Handle Bonjour maintenance
  //EthernetBonjour.run();
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    String buffer = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        buffer += c;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println();
          String prefix = "POST /";
          if (buffer.startsWith(prefix)) {
            int offset = prefix.length();
            HouseCode house = hcA;
            int device = 2;
            CommandCode cmnd = cmdOn;
            int houseInt = int(buffer.charAt(offset));
            if (houseInt >= 65 && houseInt <= 80) {
              offset += 1;
              house = (HouseCode)(houseInt - 65);
              if (buffer.charAt(offset) != '/') {
                client.println("bad separator");
                break;
              }
              offset += 1;
            } else {
              client.println("No such house code");
              break;
            }
            int deviceInt = int(buffer.charAt(offset));
            if (deviceInt >= 48 && deviceInt <= 57) {
              offset += 1;
              device = deviceInt - 48;
              int deviceInt = int(buffer.charAt(offset));
              if (deviceInt >= 48 && deviceInt <= 57) {
                offset += 1;
                device *= 10;
                device += (deviceInt - 48);
                if (device < 1 || device > 16) {
                  client.println("No such unit code");
                  break;
                }
                client.println(device);
              }
              if (buffer.charAt(offset) != '/') {
                client.println("bad separator");
                break;
              }
              offset += 1;
            } else {
              client.println("No such device code");
              break;
            }
            if (buffer.substring(offset, offset + 3) == "on ") {
              cmnd = cmdOn;
            } else if (buffer.substring(offset, offset + 4) == "off ") {
              cmnd = cmdOff;
            } else {
              client.println("No such command");
              break;
            }
            X10.sendCmd(house, device, cmnd);
            break;
          } else {
            // Send data
            /*for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
              client.print("analog input ");
              client.print(analogChannel);
              client.print(" is ");
              client.print(analogRead(analogChannel));
              client.println("<br />");
            }*/
            /*client.println(buffer);
            client.println("{");
            client.print("  \"ip\": \"");
            client.print(Ethernet.localIP());
            client.println("\"");
            client.println("}");*/
            client.println("{}");
            break;
          }
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
