#define BLYNK_TEMPLATE_ID "TMPL6JJxQs8Us"
#define BLYNK_TEMPLATE_NAME "ESP"
#define BLYNK_AUTH_TOKEN "Py1vQU4GE6mSEastzVH4OZPEXdP4DNul"

#define LEDCONNECT(V0);
#define NHIETDO V1
#define DOAM V2
#define GIATRI V3

#include <BlynkSimpleEsp8266.h>  // Thư viện Blynk và ESP8266
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <RH_NRF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

char ssid[] = "SkyNet";   // Tên Wifi
char password[] = "12345678123";  // Mật khẩu Wifi
float temperature = 0.0, humidity = 0.0, value = 0.0; // Khai báo biến toàn cục

LiquidCrystal_I2C lcd(0x27, 16, 2); 

RH_NRF24 nrf24(2, 15);

void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);  // Hàm kích hoạt Blynk
  //Wire.begin(D2, D1); //Thiết lập chân kết nối I2C (SDA, SCL);
  lcd.init();        //Khởi tạo LCD
  lcd.backlight();   //Bật đèn nền
  Serial.begin(9600);
  delay(100);

  pinMode(0, OUTPUT);

  if (!nrf24.init())
    Serial.println("init failed");
  
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
  
  delay(2000); 
}

void loop() {
  Blynk.run();

  nrf24.waitPacketSent();
  
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (nrf24.waitAvailableTimeout(1000)) { 
    if (nrf24.recv(buf, &len)) {
      value = (buf[5] << 8) | buf[4];
      humidity = (buf[3] << 8) | buf[2];
      temperature = (buf[1] << 8) | buf[0];
      Serial.println(value);
      Serial.println(temperature);
      Serial.println(humidity);
      Serial.println();
      if(value >= 400)
      {
        tone(16,659);
        delay(300);
      }
      tone(16,0);
    } else {
      Serial.println("recv failed");
    }
  }
  showlcd(temperature,value);
  updateBlynk();
}

void updateBlynk() {
  Blynk.virtualWrite(NHIETDO, temperature);
  Blynk.virtualWrite(DOAM, humidity);
  Blynk.virtualWrite(GIATRI, value);
}

void showlcd(float temp, float value) {
  lcd.clear();
  lcd.setCursor(0, 0);                    
  lcd.print("T: ");
  lcd.setCursor(3, 0);
  lcd.print(temp);
  lcd.setCursor(0, 1);               
  lcd.print("Nong do: ");
  lcd.setCursor(9, 1);
  lcd.print(value);
}
