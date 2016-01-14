//WORKING 0.4.5 or develop but panics on 0.4.6
//adapted from @Hootie81 & @jon1977 in community.particle.io
#include "application.h"
SYSTEM_MODE(MANUAL);
int serverPort = 6123;
int counter = 0;
int simulate_temp = 21;
char clientmsg = 'x';
char replymsg = '9';
uint32_t lastTime;

TCPServer server = TCPServer(serverPort);
TCPClient client;

char inmsg[512];
String myInStr;
char myIpString[24];
char outmsg[50];
int LED = D7;

void out(const char *s) {server.write( (const uint8_t*)s, strlen(s) );  client.stop();}

void in(char *ptr, uint8_t timeout) {
        int pos = 0;
        unsigned long lastTime = millis();
        while( client.available()==0 && millis()-lastTime<timeout) { //timeout
        }  //do nothing
        unsigned long lastdata = millis();
        while ( client.available() || (millis()-lastdata < 500)) {  //500 millisecond timeout
            if (client.available()) {
                char c = client.read();
                lastdata = millis();
                ptr[pos] = c;
                pos++;
            }
            if (pos >= 512 - 1)
            break;
        }
        ptr[pos] = '\0'; //end the char array
        while (client.available()) client.read(); // makeshift client.flush()
        client.flush();  //for safety
       lastdata = millis();
        while ( millis()-lastdata < 200) { }
}//void in(char *ptr, uint8_t timeout)

void setup()
{
    WiFi.connect();
    server.begin(); // begin listening for TCP connections
    IPAddress myIP = WiFi.localIP();
    sprintf(myIpString, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
    Spark.variable("devIP", myIpString, STRING);
    pinMode(LED, OUTPUT);
    Serial.begin(9600);
}//setup()

void loop()
{
    if (client.connected())
    {
        while (client.available())
        {
            in(inmsg,1000);
            myInStr =inmsg;
            if (myInStr.indexOf(clientmsg)  >= 0) {
                digitalWrite(D7, 1);
                delay(50);
                digitalWrite(D7, 0);
                sprintf(outmsg,"%c",replymsg);
                out(outmsg);
//THIS WORKS TOO
//                IPAddress myIP = WiFi.localIP();
//                sprintf(outmsg, "%d.%d.%d.%d,%d,2,%d,3,4/e", myIP[0], myIP[1], myIP[2], myIP[3],counter,simulate_temp);
//                out(outmsg); //but don't yet have a tcpclient that can handle such a response
            }//if (myInStr.indexOf("7")  >= 0)
        }//while (client.available())
    }//if (client.connected())
    client.read();
    client.flush();
//    client.stop(); //apparently unnecessary
    client = server.available();
 //keep connection alive long enough for client to receive reply
    lastTime = millis();
    while ( millis()-lastTime < 300) {}
}//loop
