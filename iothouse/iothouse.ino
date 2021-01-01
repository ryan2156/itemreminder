#include <WiFiEsp.h>

#define LAN_SSID "qqneinei"
#define LAN_PASS "123456789"

WiFiEspServer server(80);
int status = WL_IDLE_STATUS;     // the Wifi radio's status
RingBuffer buf(32);
char cmd[32];


int ledPin = 13;


int led1 = 9;
int led2 = 7;
int led3 = 6;

int key1 = 53;
int key2 = 51;
int key3 = 49;

int index;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  InitWiFi();
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
  pinMode(led1, INPUT_PULLUP);
  pinMode(led2, INPUT_PULLUP);
  pinMode(led3, INPUT_PULLUP);
  
}

void loop() {
  WiFiEspClient client = server.available();  // listen for incoming clients
 
  if(client) {
    Serial.println("New client");
    buf.init();
    index = 0;
    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        buf.push(c);
        cmd[index++] = c;
        if (buf.endsWith(";")) {
          cmd[index]=0;
          execCmd(client);
          index = 0;
        }
      }
      delay(10);
    }
 
    // close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}

void execCmd(WiFiEspClient client)
{
  int keystatus1 = digitalRead(key1);
  int keystatus2 = digitalRead(key2);
  int keystatus3 = digitalRead(key3);
  Serial.println(keystatus1);
  Serial.println(keystatus1);
  Serial.println(keystatus1);
  Serial.println();
  
  while(cmd[index]!='@')
    index++;
  while(cmd[index]!=';')
  {
    if(keystatus1 == 0)
    {
      turnOnLed(9);
      return;
    }
    else if(keystatus1 == 1)
    {
      turnOffLed(9);
      return;
    }
    if(keystatus2 == 0)
    {
      turnOnLed(7);
      return;
    }
    else if(keystatus2 == 1)
    {
      turnOffLed(7);
      return;
    }
    if(keystatus3 == 0)
    {
      turnOnLed(6);
      return;
    }
    else if(keystatus1 == 1)
    {
      turnOffLed(6);
      return;
    }
    else if(cmd[index]=='C')
    {
      int v = getInt(cmd, index+1);
      Serial.println(v);
      
    }
    index++;
    delay(10);
  }
  Serial.println("Job done");
}



void turnOffLed(int ledpin)
{
  digitalWrite(ledpin, LOW);  
}

int getInt(char *cmd, int index)
{
  int v = 0;
  while(cmd[index]<'0'&&cmd[index]>'9')
    index++;
  while(cmd[index]>='0'&&cmd[index]<='9')
  {
    v = v*10+(cmd[index]-'0');
    index++;
  }
  return v;
}



void InitWiFi()
{
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true); // don't continue
  }

  Serial.print("Attempting to start AP ");
  Serial.println(LAN_SSID);

  // start access point
  status = WiFi.beginAP(LAN_SSID, 10, LAN_PASS, ENC_TYPE_WPA2_PSK);

  Serial.println("Access point started");
  printWifiStatus();

  // start the web server on port 80
  server.begin();
  Serial.println("Server started");
}

void printWifiStatus()
{
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address of This AP: ");
  Serial.println(ip);
 }
