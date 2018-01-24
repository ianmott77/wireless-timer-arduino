#include <Connection.h>
#include <Packet.h>

uint8_t t_add = 2;
uint8_t n_add = 1;

static bool sent = false;
static bool rece = false;

int g = 0;
int q = 0;
unsigned long now = 0;

void mc(){
  //receive();
  Serial.print("Now: ");
  Serial.println(now);
  Serial.flush();
  sent = true;
}

void sen_c(){
  //receive();
  sent = true;
}

void rec_c(){
  if(q == 2){
    setSendHandler(sendMils);
    setSendCallback(mc);
  }
  rece = true;
}

//receiver
void rec(Packet * pack) {
 q  = *(int*) pack->data;
}

//sender
Packet * sendMils() {
  now = millis();
  return new Packet(&now, ULONG, sizeof(now), 0);
}

//sender
Packet * sen() {
  g++;
  return new Packet(&g, INT, sizeof(g), 0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("ready!");
  setBlocking(false);
  switchTo(I2);
  setThisAddress(t_add);
  if(!switchTo(LORA))
    Serial.println("init failed!");
  Serial.flush();
  setNextAddress(n_add);
  setReceiveCallback(rec_c);
  setSendCallback(sen_c);
  setHandlers(rec, sen);
}

void loop() {
  if(!sent && !rece){
    receive();
  }else if(rece && !sent){
    send();
  }
  if(sent && rece){
    sent = false;
    rece = false;
  }
}
