#include <WiFiEsp.h>
#include <Servo.h>

#define LAN_SSID "qqneinei"
#define LAN_PASS "123456789"

WiFiEspServer server(80);
int status = WL_IDLE_STATUS;     // the Wifi radio's status
RingBuffer buf(32);
char cmd[32];
Servo myServo;

int ledPin = 13;
int servoPin = 8;

int led1 = 9;
int led2 = 7;
int led3 = 6;

int key1 = 53;
int key2 = 51;
int key3 = 49;

int index;

void setup() {
  pinMode(ledPin, OUTPUT);
  myServo.attach(servoPin);
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
  Serial.println(cmd);
  
  while(cmd[index]!='@')
    index++;
  while(cmd[index]!=';')
  {
    if(cmd[index]=='A')
    {
      turnOnLed(13);
      return;
    }
    else if(cmd[index]=='B')
    {
      turnOffLed(13);
      return;
    }
    else if(cmd[index]=='C')
    {
      int v = getInt(cmd, index+1);
      Serial.println(v);
      myServo.write(v%181);
    }
    index++;
    delay(10);
  }
  Serial.println("Job done");
}

void turnOnLed(int ledpin)
{
  digitalWrite(ledpin, HIGH);  
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
/*
int key1 = 2;
int led1 = 13;

void light1() {
  pinMode(key1, INPUT);
  pinMode(led1, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int keystate = digitalRead(key1);
  if(keystate == 0)
  {
    digitalWrite(led1, LOW);
  }
  else
  {
    digitalWrite(led1, HIGH);
  }
  Serial.print(keystate);
  delay(500);
*/
