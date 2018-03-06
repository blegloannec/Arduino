#include <Ethernet.h>
#include <Adafruit_BMP085_U.h>

const int PORT = 80;
byte MAC[] = {0x90,0xA2,0xDA,0x0D,0x9E,0x3B};
IPAddress IP(192,168,1,20);  // we could use DHCP instead
EthernetServer httpsrv(PORT);

// atmospheric pressure in Pa, ambient temperature in C
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(18001);
float P,T;

void setup() {
  //Serial.begin(9600);
  
  //Ethernet.begin(MAC);  // DHCP
  Ethernet.begin(MAC,IP); // fixed IP
  httpsrv.begin();
  //Serial.print("IP ");
  //Serial.println(Ethernet.localIP());
  
  while (!bmp.begin()) {
    //Serial.print("ERROR: no BMP085/180 detected!");
    delay(1000);
  }
}

void update_measurements() {
  bmp.getPressure(&P);
  P /= 100.;
  bmp.getTemperature(&T);
}

void send_http_header(EthernetClient &client, bool html=true) {
  // standard http response header
  client.println(F("HTTP/1.1 200 OK"));
  client.print(F("Content-Type: "));
  if (html) client.println(F("text/html"));
  else      client.println(F("text/plain"));
  client.println(F("Connection: close")); // close connect. after response
  if (html) client.println(F("Refresh: 15")); // refresh every X s
  client.println();
}

void send_html_page(EthernetClient &client) {
  send_http_header(client);
  client.println(F("<!DOCTYPE HTML>"));
  client.println(F("<html>"));
  client.print(F("Ambiant temperature: "));
  client.print(T);
  client.println(F(" C<br />"));
  client.print(F("Atmospheric pressure: "));
  client.print(P);
  client.println(F(" hPa"));
  client.println(F("</html>"));
}

void send_text_data(EthernetClient &client) {
  send_http_header(client,false);
  client.print(T);
  client.print(" ");
  client.println(P);
}

void loop() {
  //Ethernet.maintain();  // maintain DHCP lease
  // listen for incoming clients
  EthernetClient client = httpsrv.available();
  if (client) {
    // an http request ends with a blank line
    bool blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (blank && c=='\n') {
	  // http request has ended
	  update_measurements();
	  //send_html_page(client);
	  send_text_data(client);
          break;
        }
        if (c=='\n') blank = true;
	else if (c!='\r') blank = false;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection
    client.stop(); 
  }
  //delay(10);
}
