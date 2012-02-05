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

EthernetServer server = EthernetServer(80);

void setup()
{
  Ethernet.begin(mac);
  /*EthernetBonjour.begin("x10");
  EthernetBonjour.addServiceRecord("Arduino Bonjour Webserver Example._http",
                                   80,
                                   MDNSServiceTCP);*/
  X10.init( RTS_PIN, DTR_PIN );
}

void on()
{
  X10.sendCmd( hcN, 2, cmdOn );
}

void off()
{
  X10.sendCmd( hcN, 2, cmdOff );
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
          if (buffer.startsWith("POST /N/2/on")) {
            on();
            break;
          } else if (buffer.startsWith("POST /N/2/off")) {
            off();
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
            client.println(buffer);
            client.println("{");
            client.print("  \"ip\": \"");
            client.print(Ethernet.localIP());
            client.println("\"");
            client.println("}");
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
