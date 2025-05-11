
# **Arduino DHT11 Sıcaklık ve Nem Takip Sistemi**

Bu proje, **ESP8266** tabanlı bir mikrodenetleyici kullanarak **DHT11** sensörü ile sıcaklık ve nem verilerini okur. Bu verileri bir web sunucusuna gönderir ve belirli değerleri aştığında e-posta gönderir. Aynı zamanda bir **LCD ekran** üzerinde sıcaklık ve nem bilgilerini görüntüler.

### **Proje İçeriği**

* **Arduino Kodu:** ESP8266 ile sıcaklık ve nem verilerini okur, gönderir ve görselleştirir.
* **Web Arayüzü:** PHP dosyaları (index.php, graph.php, display\_data.php) ile sıcaklık ve nem verilerinin izlenmesini sağlar.
* **E-posta Gönderimi:** Belirli değerler aşıldığında sıcaklık ve nem verilerini e-posta olarak gönderir.

### **Kullanılan Teknolojiler**

* **ESP8266 Wi-Fi Modülü**
* **DHT11 Sıcaklık ve Nem Sensörü**
* **Adafruit PCD8544 LCD Ekran**
* **Web Arayüzü: PHP, HTML, JavaScript**
* **E-posta Gönderimi: SMTP**

### **Gereksinimler**

#### **Donanım:**

* ESP8266 modülü (örneğin: NodeMCU)
* DHT11 sıcaklık ve nem sensörü
* Adafruit PCD8544 LCD ekran
* Wi-Fi bağlantısı

#### **Yazılım:**

* Arduino IDE
* ESP8266 kütüphaneleri
* Adafruit GFX, PCD8544 ve DHT kütüphaneleri
* ESPAsyncWebServer, ESP8266WiFi, ESP\_Mail\_Client kütüphaneleri

### **Kurulum**

#### 1. **Donanım Bağlantıları**

* **DHT11** sensörünün **4. pin**'i ESP8266'nın **GPIO4** pinine bağlanmalıdır.
* **LCD ekran** bağlantıları:

  * CLK: GPIO12
  * DIN: GPIO13
  * D/C: GPIO14
  * CE: GPIO15
  * RST: GPIO2

#### 2. **Yazılım Kurulumu**

* **Arduino IDE**'yi açın ve gerekli kütüphaneleri yükleyin.

  * **Adafruit\_Sensor**
  * **Adafruit\_GFX**
  * **Adafruit\_PCD8544**
  * **DHT**
  * **ESP8266WiFi**
  * **ESPAsyncWebServer**
  * **ESP\_Mail\_Client**
  * **ESP8266HTTPClient**
* **SMTP** ayarlarını ve Wi-Fi bilgilerini Arduino kodunda yapılandırın.

#### 3. **Wi-Fi ve E-posta Ayarları**

* **Wi-Fi:** `ssid` ve `password` değişkenlerine kendi Wi-Fi ağınıza ait bilgileri girin.
* **SMTP:** E-posta gönderebilmek için SMTP sunucusu, port, e-posta ve şifre bilgilerini doğru şekilde ayarlayın.

```cpp
#define SMTP_server "YOUR_SMTP_SERVER"
#define SMTP_Port YOUR_SMTP_PORT
#define AUTHOR_EMAIL "YOUR_EMAIL@example.com"
#define AUTHOR_PASSWORD "YOUR_EMAIL_PASSWORD"
#define RECIPIENT_EMAIL "RECIPIENT_EMAIL@example.com"
```

#### 4. **Web Sunucusu ve PHP Betikleri**

* PHP betiklerini uygun bir web sunucusuna (örneğin Apache veya Nginx) yükleyin.
* **index.php:** Kullanıcıların sıcaklık ve nem verilerini anlık olarak görmesini sağlar.
* **graph.php:** Verilerin grafiksel gösterimini sağlar.
* **display\_data.php:** ESP8266'dan gelen sıcaklık ve nem verilerini alır ve saklar.

### **Arduino Kodu**

Arduino kodu, DHT11 sensöründen sıcaklık ve nem verilerini okur, bu verileri bir web sunucusuna gönderir ve e-posta gönderir. Ayrıca, veriler LCD ekranda görüntülenir.

#### **Ana İşlevler:**

* **DHT11 Sensörü:** Her 10 saniyede bir sıcaklık ve nem verisi okur.
* **Web Sunucu:** Anlık sıcaklık ve nem verilerini web sayfasında görüntüler.
* **E-posta Gönderimi:** Belirli değerler aşıldığında sıcaklık ve nem verilerini e-posta olarak gönderir.
* **LCD Ekran:** Anlık verileri LCD ekranda gösterir.

#### **Kod Açıklamaları:**

* `setup()` fonksiyonu, Wi-Fi bağlantısını başlatır, web sunucusunu ve SMTP ayarlarını yapılandırır.
* `sendDataToServer()` fonksiyonu, sıcaklık ve nem verilerini PHP betiğine gönderir.
* `sendMail()` fonksiyonu, e-posta gönderimini gerçekleştirir.
* `loop()` fonksiyonu, sürekli olarak verileri okur ve uygun aralıklarla PHP betiğine gönderir ve e-posta gönderir.

### **Web Arayüzü**

1. **index.php:** Web sayfasının ana sayfasıdır ve sıcaklık ve nem verilerini anlık olarak gösterir.
2. **graph.php:** Zamanla değişen sıcaklık ve nem verilerinin grafiksel bir gösterimini sağlar.
3. **display\_data.php:** Arduino'dan gelen sıcaklık ve nem verilerini veritabanına kaydeder.

### **PHP Betikleri Kullanımı:**

1. **index.php:** Kullanıcıların sıcaklık ve nem verilerini görmesini sağlar.

   * Anlık sıcaklık ve nem verileri, JavaScript kullanılarak 10 saniyede bir güncellenir.

2. **graph.php:** Verileri grafiksel olarak gösterir. PHP ve JavaScript ile sıcaklık ve nem verilerini zamanla görselleştirir.

3. **display\_data.php:** Arduino'dan gelen verileri işler ve sunucuda saklar.

### **E-posta Ayarları**

* Bu projede e-posta gönderimi, SMTP protokolü ile yapılır. E-posta bilgilerini doğru bir şekilde ayarladığınızda, ESP8266 belirli değerler aşıldığında her 5 dakikada bir sıcaklık ve nem verilerini e-posta olarak gönderir.

### **Sonuç**

Bu proje, **IoT** tabanlı bir sıcaklık ve nem izleme sistemi sağlar. ESP8266, DHT11 sensörü ve LCD ekran kullanarak sıcaklık ve nem verilerini toplar, web üzerinden takip eder ve belirli aralıklarla e-posta yoluyla kullanıcıya gönderir.


