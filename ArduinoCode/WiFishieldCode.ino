#include <SPI.h>
#include <WiFi.h>

char ssid[] = "Your SSID";      // your network SSID (name)
char pass[] = "Your PASSWORD";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
const char* fcmServer = "fcm.googleapis.com";
int status = WL_IDLE_STATUS;
bool flag = false;
WiFiServer server(80);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() {
  if(!flag){
    String data = jsonString();
    //Serial.print("in loop data: ");
    //Serial.println(data);
    sendDataToFirebase(data);
    flag = true;
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

String jsonString(){
  String body = "GG";
  /*
  String data0 = "{";
  Serial.print("data0 : ");
  Serial.println(data0);
  String data1 = String(data0 + "\"to\": \"");
  Serial.print("data1 : ");
  Serial.println(data1);
  
  String data1_1 = String(data1 + "/topics/news");
  Serial.print("data1_1 : ");
  Serial.println(data1_1);
  String data2 = String(data1_1+"\",\"data\": {");
  Serial.print("data2 : ");
  Serial.println(data2);
  String data3 = String(data2 + "\"title\" : \"집가고싶다\",\"message\": \"GG\"} }");
  Serial.print("data3 : ");
  Serial.println(data3);
  */
  
  /*
  String data4 = String( "\"message\": \"GG\",");
  Serial.print("data4 : ");
  Serial.println(data4);
  String data5 = String(data4 + "} }");
  Serial.print("data5 : ");
  Serial.println(data5);
  String data6 = String(data3 + data5);
  Serial.print("data6 : ");
  Serial.println(data6);
  */

  String data0 = "{";
  Serial.print("data0 : ");
  Serial.println(data0);
  String data1 = String(data0 + "\"to\": \"/topics/news\",");
  Serial.print("data1 : ");
  Serial.println(data1);
  String data2 = String(data1 + "\"data\": {");
  Serial.print("data2 : ");
  Serial.println(data2);
  String data3 = String(data2 + "\"title\" : \"[마블]출퇴근감시시스템\",\"message\": \"GG\"} }");
  Serial.print("data3 : ");
  Serial.println(data3);

  return data3;
}

void sendDataToFirebase(String data){

  Serial.println(data);
  // =================================================================
  WiFiClient client;

  if (client.connect(fcmServer,443)) {
    Serial.println("Connected to the server..");
    client.println("POST /fcm/send HTTP/1.1");
    client.println("Host: fcm.googleapis.com");
    client.println("Authorization: key=YOUR KEY");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);

    Serial.println("Data sent...");   
  }
  else {
    Serial.println("FCM connection failed!");
    client.stop();
  }

  Serial.println("Reading response...");
  while(client.available()){
    char c = client.read();
    Serial.print(c);
  }
  Serial.println("Finished");
  client.flush();  //wait until all outgoing characters in buffer have been sent.
  client.stop(); // Disconnect from the server.
  
}
