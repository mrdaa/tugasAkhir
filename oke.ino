#include <ESP8266WiFi.h>
//#define BLYNK_PRINT Serial
// #include <BlynkSimpleEsp8266.h>
#include <DHT.h>
//#include <SimpleTimer.h>
#include <ArduinoJson.h>
#include <SocketIoClient.h>


#define DHTTYPE DHT11
#define dhtPin D4
#define relay1 D3

// Setting Bynk
//char auth[] = "mo55ZXx3QxYskeKlQf4HW5bhBdBNmbtp";

// Setting SocketIo Connect
// char SocketServer[] = "192.168.43.154";


char SocketServer[] = "139.180.189.208";
// char path[] = "/socket/";
int port = 4000;


// Setting Wifi
// char ssid[] = "Koen_DILAN!";
char ssid[] = "Koen_DILAN!";
char pass[] = "";

//SimpleTimer timer;
SocketIoClient socket;

DHT dht(dhtPin, DHTTYPE);

// void rTemp(const bool newState) {
//   char* message = "\"OFF\"";
//   if (!newState) {
//     message = "\"ON\"";
//   } 
//   // webSocket.emit("state_change", message);
//   // LEDState = newState;
//   Serial.print("LED state has changed: ");
//   Serial.println(message);
// }


void event(const char * payload, size_t length) {
  Serial.print("Signal !");
}

void setup() {

  pinMode(relay1, OUTPUT);
  pinMode(relay1, HIGH);
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }

  socket.begin(SocketServer, port);
  socket.emit("new user", "\"P1471984882\"");
//  Blynk.begin(auth,ssid,pass);

  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  socket.on("rtemp",event);
}

void loop() {
//  Blynk.run();
//  timer.run();
  socket.loop();  

  String id = "1471984882";

// Kirim data Temperatur 
  float t = dht.readTemperature();
	char temp[10];
	String tempAsString;
  	// perform conversion
  dtostrf(t,1,2,temp);
  	// create string object
	tempAsString = String(temp);
  String inputTemp;
  inputTemp = t;
  String dataTemp = "{\"_id\":\"" + id + "\",\"_val\":\""+ tempAsString +"\"}";
  socket.emit("temp", dataTemp.c_str());
  
  // Kirim data Temperatur 
  float h = dht.readHumidity();
	char hum[10];
	String humAsString;
  	// perform conversion
  dtostrf(h,1,2,hum);
  	// create string object
	humAsString = String(hum);
  String inputHum;
  inputHum = h;
  String dataHum = "{\"_id\":\"" + id + "\",\"_val\":\"" + humAsString + "\"}";
  socket.emit("hum", dataHum.c_str());

  delay(1000);
}

