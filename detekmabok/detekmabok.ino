#include <ESP8266WiFi.h>                
#include <Wire.h>     //modifikasi atau copas kontak ZUNUZZZ                    
#include <LiquidCrystal_I2C.h>          
#include <ThingSpeak.h>                  
#include <WiFiClient.h>                  
#include <ESP8266HTTPClient.h>           

LiquidCrystal_I2C lcd(0x27, 16, 2); //modifikasi atau copas kontak ZUNUZZZ  

char ssid[] = "K7";                     
char pass[] = "12345678";               
int sensor = A0;                         
int rentangSensor = 100;                 

bool pesanAwal = false;                 

byte panahKiri[8] = {                    
    0b01110,
    0b01110,
    0b01110,
    0b01110,//modifikasi atau copas kontak ZUNUZZZ  
    0b01110,
    0b11111,
    0b01110,
    0b00100
};

byte panahKanan[8] = {                   
    0b01110,
    0b01110,
    0b01110,//modifikasi atau copas kontak ZUNUZZZ  
    0b01110,
    0b01110,
    0b11111,
    0b01110,
    0b00100
};

const char *apiKeyTulis = "CNW5IHL36BCR0KJ2";     
const char *apiKeyBaca = "V9ZKWWOECTY1OV38";      
const char *server = "api.thingspeak.com";        
const long channelID = 2383286;                   
WiFiClient klien;                               
HTTPClient http;                //modifikasi atau copas kontak ZUNUZZZ                   
String url;                                     
String API = "CNW5IHL36BCR0KJ2";                  
String noField = "1";                            

void sambungKeWiFi(void) {
  
}

void setup() {
  lcd.init();                       
  lcd.backlight();                  
  pinMode(sensor, OUTPUT);          
  Serial.begin(115200); //modifikasi atau copas kontak ZUNUZZZ              
  sambungKeWiFi();                  
  delay(10);
  Serial.println("Menghubungkan ke "); 
  Serial.println(ssid);
  WiFi.begin(ssid, pass);           

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Tidak dapat terhubung...Silakan coba lagi"); 
    delay(10000);
  }

  Serial.println("Menghubungkan ke WiFi..."); //modifikasi atau copas kontak ZUNUZZZ  
  Serial.print("WiFi terhubung, alamat IP:"); 
  Serial.println(WiFi.localIP()); 
  Serial.println("Selamat Datang"); 
}

int httpcode; 

void loop() {
  int data = analogRead(sensor); 
  Serial.print("DATA SENSOR: "); 
  Serial.println(data); //modifikasi atau copas kontak ZUNUZZZ  
  KirimPermintaanGET(data);            
  delay(10000);

  if (!pesanAwal) {
    lcd.setCursor(1, 0); 
    lcd.print("DEKATKAN MULUT"); 
    lcd.setCursor(3, 1); 
    lcd.print("KE SENSOR!"); 
    lcd.createChar(0, panahKiri); 
    lcd.setCursor(14, 1); 
    lcd.write((byte)0); //modifikasi atau copas kontak ZUNUZZZ  
    lcd.createChar(0, panahKanan); 
    lcd.setCursor(1, 1); 
    lcd.write((byte)0); 
    pesanAwal = true; 
  } else {
    int nilaiPemetaan = map(data, 512, 1024, 0, 100); 
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("DRUNK DETECTOR K7"); 
    lcd.setCursor(1, 1);//modifikasi atau copas kontak ZUNUZZZ  
    if (nilaiPemetaan < 50) { 
      lcd.print("STATUS: NORMAL");
    } else { 
      lcd.setCursor(1, 1);
      lcd.print("STATUS: MABUK");
    }
  }

  if (data < 1025) { 
    ThingSpeak.begin(klien);//modifikasi atau copas kontak ZUNUZZZ  
    ThingSpeak.writeField(channelID, 1, data, apiKeyTulis);
    Serial.println("DATA ALKOHOL TERKIRIM KE THINGSPEAK!");
    delay(10000);
  }
}

void KirimPermintaanGET(int nilai) {  
  url = "http://api.thingspeak.com/update?api_keys=";
  url = url + API;
  url = url + "&field";
  url = url + noField;//modifikasi atau copas kontak ZUNUZZZ  
  url = url + "=";
  url = url + nilai;
  http.begin(klien, url);
  httpcode = http.GET();
  http.end();
}