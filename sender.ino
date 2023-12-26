#include <SPI.h>
#include <RH_NRF24.h>
#include <Wire.h> 
#include <DHT.h>

#define DHTPIN 2  // Chân kết nối DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
 
RH_NRF24 nrf24(9, 10);
uint8_t data[6] ;
const int sensor = A0;
int value = 0, temperature = 0, humidity = 0;
unsigned long time;

void setup(){ 
  Serial.begin(9600);
  dht.begin();
  time = millis();
  //============================================================Module NRF24
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
  pinMode(sensor, INPUT);
}
 
void loop(){
 if ( (unsigned long) (millis() - time) > 5000)
 {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    value = analogRead(sensor);
    data[0] = temperature;
    data[1] = temperature >> 8;
    data[2] = humidity;
    data[3] = humidity >> 8;
    data[4] = value;
    data[5] = value >> 8;
    nrf24.send(data, sizeof(data));
    nrf24.waitPacketSent();
 }
}
