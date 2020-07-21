#include <ESP8266WiFi.h>
//#define BLYNK_PRINT Serial
// #include <BlynkSimpleEsp8266.h>
#include <DHT.h>
// #include <EEPROM.h>
// #include <GravityTDS.h>
#include <SocketIoClient.h>

#define DHTTYPE DHT11
#define dhtPin D4
#define relay1 D3
#define trigPin D5
#define echoPin D6
// #define TdsSensorPin A0

String id = "1471984882";

long duration ;
float distance;
const int max_hight = 14;
// float temperature = 25,tdsValue = 0;

// char SocketServer[] = "139.180.189.208";
char SocketServer[] = "192.168.43.47";
int port = 4000;


// Setting WiFi
char ssid[] = "B";
char pass[] = "punyaucup11299";

//SimpleTimer timer;
SocketIoClient socket;
// GravityTDS gravityTds;
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

void konek(const char * payload, size_t length) {
  socket.emit("new user", "\"P1471984882\"");
    Serial.println("berhasil connect");
}

void event(const char * payload, size_t length) {
  Serial.print(payload);
  // if (payload != NULL && (strcmp(payload, "true") == false)){
  //   digitalWrite(relay1, HIGH);
  //   // socket.emit("resTemp", "{\"_id\":\"" + id + "\",\"_val\":\" true \"}");
  // } else {
  //   digitalWrite(relay1, LOW);
    // socket.emit("resTemp", "{\"_id\":\"" + id + "\",\"_val\":\" true \"}");
  // }
  Serial.println("berhasil");
}
void test(const char * payload, size_t length) {
  Serial.print(payload);
  Serial.println("berhasil dong");

}
// void socket_event(const char * payload, size_t length) {
//   Serial.print("got message: ");
//   Serial.println(payload);

// }

void setup() {

  pinMode(relay1, OUTPUT);
  pinMode(relay1, HIGH);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }

  // gravityTds.setPin(TdsSensorPin);
  // gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  // gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  // gravityTds.begin();

  socket.begin(SocketServer, port);
  socket.on("connect",konek);
  // socket.emit("new user", "\"P1471984882\"");
  //  Blynk.begin(auth,ssid,pass);

  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  socket.on("rhum",test);
  // socket.on("rtemp",event);
}

void loop() {
//  Blynk.run();
//  timer.run();
  socket.loop();  
socket.on("rhum",test);
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
    socket.on("rtemp",event);

// sensor ultrasound ketinggian air
  digitalWrite(trigPin, LOW); // Added this line
  delayMicroseconds(2);       // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  distance = max_hight - distance;
  distance = (distance / max_hight) * 100;

	char wl[10];
	String wlAsString;
  	// perform conversion
  dtostrf(distance,1,2,wl);
  	// create string object
	wlAsString = String(wl);
  String inputWl;
  inputWl = distance;
  String dataWl = "{\"_id\":\"" + id + "\",\"_val\":\"" + inputWl + "\"}";
  socket.emit("wl", dataWl.c_str());

// tds sensor
  // gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  // gravityTds.update();  //sample and calculate
  // tdsValue = gravityTds.getTdsValue();  // then get the value
  // Serial.print(tdsValue);
  // Serial.println("ppm");



  delay(1000);
}

