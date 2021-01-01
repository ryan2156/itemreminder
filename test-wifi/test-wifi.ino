#include <WiFiEsp.h>
#include <Servo.h>

#define WLAN_SSID "要連上的AP SSID"
#define WLAN_PASS "要連上的AP 密碼"

WiFiEspServer server(80);
RingBuffer buf(32);
char cmd[32];
Servo myServo;
int bufIndex = 0;

int ledPin = 13;
int servoPin = 8;
int index;

void setup() {
  pinMode(13, OUTPUT);
  myServo.attach(servoPin);
  Serial.begin(9600);
  Serial1.begin(9600);
  InitWiFi();
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
  bufIndex = 0;
  while(cmd[bufIndex]!='@')
    index++;
  while(cmd[bufIndex]!=';')
  {
    if(cmd[bufIndex]=='A')
    {
      turnOnLed(13);
      return;
    }
    else if(cmd[bufIndex]=='B')
    {
      turnOffLed(13);
      return;
    }
    else if(cmd[bufIndex]=='C')
    {
      int v = getInt(cmd);
      Serial.println(v);
      myServo.write(v%181);
    }
    bufIndex++;
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

int getInt(char *cmd)
{
  int v = 0;
  while(cmd[bufIndex]<'0'||cmd[bufIndex]>'9')
    bufIndex++;
  while(cmd[bufIndex]>='0'&&cmd[bufIndex]<='9')
  {
    v = v*10+(cmd[bufIndex]-'0');
    bufIndex++;
  }
  return v;
}


void InitWiFi()
{
  int status = WL_IDLE_STATUS;
  WiFi.init(&Serial1);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
  Serial.println("Connecting to AP ...");
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WLAN_SSID);
    status = WiFi.begin(WLAN_SSID, WLAN_PASS);
    delay(500);
  }

  Serial.println("Connected to AP");
  printWifiStatus();
  server.begin();
  Serial.println("Server Ready");
}

void printWifiStatus()
{
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
 }
