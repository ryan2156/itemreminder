int led1 = 13;

int key1 = 2;
void setup() {
  pinMode(key1,INPUT);
  pinMode(led1, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int keystatus = digitalRead(key1);
  if(keystatus == 0){
    digitalWrite(led1, LOW);
    }
  else{
    digitalWrite(led1, HIGH);
    }
  Serial.println(keystatus);
  delay(100);
}
