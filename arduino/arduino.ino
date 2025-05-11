#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP_Mail_Client.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN  4     
#define DHTTYPE DHT11   
#define SMTP_server "YOUR_SMTP_SERVER"
#define SMTP_Port YOUR_SMTP_PORT
#define AUTHOR_EMAIL "YOUR_EMAIL@example.com"
#define AUTHOR_PASSWORD "YOUR_EMAIL_PASSWORD"
#define RECIPIENT_EMAIL "RECIPIENT_EMAIL@example.com"

DHT dht(DHTPIN, DHTTYPE);
SMTPSession smtp;

// WiFi bilgileri
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverName = "http://192.168.86.216/display.php";  // Ubuntu üzerindeki PHP betiğinin URL'si

// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(12, 13, 14, 15, 2);
int contrastValue = 60; 

// temperature and humidity değişken tanımlama
float t = 0.0;
float h = 0.0;

// dht'den son gelen verinin zamanını tutar
unsigned long previousDHTMillis = 0;    
const long dhtInterval = 10000;  // her 10 saniyede dht'den veri alacak

// e-posta gönderim zamanını tutar
unsigned long previousEmailMillis = 0;    
const long emailInterval = 300000;  // 5 dakika = 300.000 ms

// web sunucusu oluşturuldu ve port 80'de çalışacak
AsyncWebServer server(80);

// html içeriği
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3rem; }
    p { font-size: 2rem; }
    .units { font-size: 2rem; }
    .dht-labels{
      font-size: 2rem;
      vertical-align:10px;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8226 DHT 11 SERVER</h2>
  <p> 
    <i class="fas fa-sun" style="color:#db3716;font-size: 3.5rem;"></i> 
    <span class="dht-labels">Temperature</span> <br> <br>
    <span id="temperature"> <mark> %TEMPERATURE% </mark> </span>
    <span class="units">°C</span>
  </p> <br><br><br>
  <p>
    <i class="fas fa-cloud" style="color:#4dc0db;font-size: 3.5rem;"></i> 
    <span class="dht-labels">Humidity</span> <br><br>
    <span id="humidity"> <mark> %HUMIDITY% </mark> </span>
    <span class="units">%</span>
  </p>
 
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// HTML içeriğindeki T ve H verilerini güncelle
String processor(const String& var){
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

// SMTP bilgileri
ESP_Mail_Session session;
SMTP_Message message;

void setup(){
  Serial.begin(115200);
  dht.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wi-Fi'ye bağlanılıyor...");
  }
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // web sunucusu yönetim
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  server.begin();

  // LCD ekranı başlat ve ayarla
  display.begin();
  display.setContrast(contrastValue);
  display.clearDisplay();
  display.display();
  delay(100);

  smtp.debug(1);

  // SMTP ayarları
  session.server.host_name = SMTP_server;
  session.server.port = SMTP_Port;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  // Mail başlıkları
  message.sender.name = "ESP8266";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "ESP8266 Sıcaklık ve Nem Güncellemesi";
  message.addRecipient("Feyza", RECIPIENT_EMAIL);
}

// Mail gönderme fonksiyonu
void sendMail() {
  if (!smtp.connect(&session)) {
    Serial.println("SMTP sunucusuna bağlanılamadı: " + smtp.errorReason());
    return;
  }

  // Mail içeriği 
  String htmlMsg = "<div style=\"color:#000000;\"><h1>Merhaba!</h1>";
  htmlMsg += "<p>Sıcaklık: " + String(t) + " &deg;C</p>";
  htmlMsg += "<p>Nem: " + String(h) + " %</p></div>";
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("E-posta gönderme hatası: " + smtp.errorReason());
  } else {
    Serial.println("E-posta başarıyla gönderildi");
  }
}

// Verileri PHP betiğine gönderme fonksiyonu
void sendDataToServer() {
  WiFiClient client; // WiFiClient nesnesini oluşturun
  HTTPClient http;

  String serverName = "http://192.168.86.216//display_data.php";

  http.begin(client, serverName);  // Yeni API kullanımı
  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String httpRequestData = "temperature=" + String(t) + "&humidity=" + String(h);
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error on sending POST: " + String(httpResponseCode));
  }

  http.end();
}


void loop(){  
  unsigned long currentMillis = millis();

  // 10 saniyede bir DHT sensöründen veri okuma
  if (currentMillis - previousDHTMillis >= dhtInterval) {
    previousDHTMillis = currentMillis;

    // Sıcaklık okuma
    float newT = dht.readTemperature();
    if (!isnan(newT)) {
      t = newT;
    } else {
      Serial.println("DHT sensöründen veri okunamadı!");
    }

    // Nem okuma
    float newH = dht.readHumidity();
    if (!isnan(newH)) {
      h = newH;
    } else {
      Serial.println("DHT sensöründen veri okunamadı!");
    }

    // LCD ekranına yazdırma
    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("TEMPERATURE: ");
    display.print(t);
    display.println(" C      ");
    display.println("             ");
    display.println("HUMIDITY:    ");
    display.print(h);
    display.println(" %      ");
    display.display();

    // PHP betiğine veriyi gönder
    sendDataToServer();
  }

    // Eğer sıcaklık >= 25 veya nem >= 55 ise ve 5 dakika geçtiyse e-posta gönder
    if ((t >= 25.0 || h >= 55.0) && (currentMillis - previousEmailMillis >= emailInterval)) {
      previousEmailMillis = currentMillis;
      sendMail();
}

}
