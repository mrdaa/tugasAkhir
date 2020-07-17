#include <ESP8266WiFi.h>
//#define BLYNK_PRINT Serial
// #include <BlynkSimpleEsp8266.h>
#include <DHT.h>
//#include <SimpleTimer.h>
#include <ArduinoJson.h>
#include <SocketIoClient.h>


#define DHTTYPE DHT11
#define dhtPin D4

// Setting Bynk
//char auth[] = "mo55ZXx3QxYskeKlQf4HW5bhBdBNmbtp";

// Setting SocketIo Connect
// char SocketServer[] = "192.168.43.154";
char SocketServer[] = "192.168.47.147";
int port = 4000;


// Setting Wifi
// char ssid[] = "Koen_DILAN!";
char ssid[] = "Perpustakaan@AlumniPHB";
char pass[] = "";

//SimpleTimer timer;
SocketIoClient socket;

DHT dht(dhtPin, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, pass);
  socket.begin(SocketServer,port);
  socket.emit("new user", "\"P1471984882\"");
//  Blynk.begin(auth,ssid,pass);

}

void loop() {
//  Blynk.run();
//  timer.run();
  socket.loop();  

  String id = "P1471984882";

  float t = dht.readTemperature();
	char temp[10];
	String tempAsString;
  	// perform conversion
  	dtostrf(t,1,2,temp);
  	// create string object
	tempAsString = String(temp);
  String inputString;
  inputString = t;
  String asd = "{\"_id\":\"" + id + "\",\"_val\":\"" + tempAsString + "\"}";
  socket.emit("temp", asd.c_str());
  

  // socket.emit("hum" , dataHum);
  delay(1000);
}