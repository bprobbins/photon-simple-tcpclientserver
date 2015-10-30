
//adapted from @Hootie81 & @jon1977 in community.particle.io
#include "application.h"
SYSTEM_MODE(MANUAL);

int serverPort = 6123;
uint32_t lastTime;
char read_char;
char clientmsg = 'x';
char replymsg = '9';
byte server[] = {192, 168, 1, 10};
bool complete;
TCPClient client;

char outmsg[50];

void out(const char *s) {client.write( (const uint8_t*)s, strlen(s) );}

void setup()
{
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D3,INPUT);
}//setup()

void loop() {
  WiFi.disconnect();
  System.sleep(D3, RISING, 4);
  if (digitalRead(D3)) {digitalWrite(D6,HIGH);delay(200);digitalWrite(D6,LOW);}

    while (!WiFi.ready()) {
      Particle.process();
      WiFi.connect();
      while(WiFi.connecting()){Particle.process();}
    }// while (!WiFi.ready())

    complete = false;
    //don't unsuccessfully persist beyond 10 secs, just go back to sleep
    lastTime = millis();
    while ((!complete) &&  (millis() - lastTime < 10000)){
      if (client.connect( server, serverPort)) {
        if (client.connected()) {
          sprintf(outmsg,"%c",clientmsg);
          out(outmsg);
          lastTime = millis();
          while ((!client.available()) && (millis() - lastTime < 5000)) { } //critical 10000
          lastTime = millis();
          while ((millis() - lastTime < 300)) {}//plays better with nodejs server?
          while (client.available()) { //now get confirmation from server that server received msg
            read_char = client.read();
            if(read_char == replymsg ) { //we got confirmation
              digitalWrite(D7,HIGH);delay(10);digitalWrite(D7,LOW);
              client.read();
              complete = true;
            }//if(read_char == replymsg )
    }//while (client.available())
  }//if (client.connected())
 }//if (client.connect( server, serverPort))
  client.read();
  client.flush();
}//while (!complete) //!!!!!!!!!!!!!!!!
// prevent nodejs ECONNRESET, not necessay with another photon??
//  lastTime = millis();
//  while ((millis() - lastTime < 500)) {}//prevent nodejs ECONNRESET
  delay(1);
}//loop
