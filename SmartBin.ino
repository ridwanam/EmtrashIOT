#include <FirebaseArduino.h>
#include <NewPing.h>
#include <Servo.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "smarbin-ridwan.firebaseio.com"
#define FIREBASE_AUTH "EF8AWA2dVMO2cy85KSJZs7zIWTvdVwuSfZSrcy7o"
#define WIFI_SSID "Sholat Dulu Internetan Kemudian"
#define WIFI_PASSWORD "12345678"

const int BUTTON = 12; //D6
#define TRIGGER_PIN   16 //D0
#define ECHO_PIN      5 //D1
#define TRIGGER_PIN2  4 //D2
#define ECHO_PIN2     2 //D4
#define MAX_DISTANCE 200

Servo servoAne;
NewPing ultrasonic1(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing ultrasonic2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);

int  trashTime, lastTime, idleTime;
int binSize, binStatus;
double binPercent;
int state;
void setup() {
  Serial.begin(115200);
  servoAne.attach(14); //D5
  servoAne.write(0);
  pinMode ( BUTTON , INPUT ); // TombolPin sebagai INPUT
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("connected: ");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("bins/0/sensor1", 0);
  Firebase.set("bins/0/sensor2", 0);


}

void loop() {
  int  US1 = ultrasonic1.ping_cm();
  int  US2 = ultrasonic2.ping_cm();
  //triger jarak sensor
  if (US1 > 22)
  {
    US1 = 22;
  }//end

  //membalikan nilai sensor
  int reUS1;
  for (int a = 0; a <= 22; a++) {
    if (US1 == a) {
      reUS1 = 22 - US1;
    }
  }//end

  Firebase.set("bins/0/sensor2", US2);

  Serial.print("Hasil Sensor 1 :");
  Serial.print(US1);
  Serial.print("cm     |     ");
  Serial.print("Hasil Sensor 2 :");
  Serial.print(US2);
  Serial.println("cm");
//trigger tutup tempat sampah
  if (US2 <= 10 && binPercent <=93 ) {
    servoAne.write(180);
    delay(1000);
  }
  if else(US2 <= 10 && binPercent >=93){
    servoAne.write(0);
    delay(1000);
  }
  else {
    servoAne.write(0);
    delay(1000);
  }
  // Cek Kondisi NodeMCU
  if (US2 == 0)
  {
    idleTime++;
    Firebase.set("bins/0/idleTimeidleTime", idleTime);
    if (idleTime >= 1800) {
      binStatus = 404;
      Firebase.set("bins/0/bin_status", binStatus);
    }
  } else {
    idleTime = 0;
    Firebase.set("bins/0/idleTime", idleTime);
  }
  //Waktu Pembersihan Terakhir
  Firebase.set("bins/0/second", trashTime);
  trashTime = Firebase.getInt("bins/0/second");
  trashTime++;
  if (trashTime < 3600) {
    lastTime = 0;
    Serial.print("trashTime: ");
    Serial.println(trashTime);
    Firebase.set("bins/0/terakhir_pembersihan", lastTime);
  } else {
    lastTime = trashTime / 3600;
    Serial.print("trashTime: ");
    Serial.println(trashTime);
    Firebase.set("bins/0/terakhir_pembersihan", lastTime);
  }
  // reset waitu pembersihan terakhir
  state = digitalRead ( BUTTON );
  Serial.print("state: ");
  Serial.println(state);
  if ( state == HIGH ) {
    trashTime = 0;
    servoAne.write(180);
    delay(10000);
  } else {
    trashTime++;
  }
  Serial.print("lastTime: ");
  Serial.println(lastTime);

  //Notifikasi Setup & display widget setup
  binSize = 18;
  binPercent = double(reUS1) / binSize;
  Serial.print("binPercent int: ");
  Serial.println(binPercent);
  binPercent = binPercent * 100;
  Firebase.set("bins/0/sensor1", int(binPercent));
  Serial.print("binPercent %: ");
  Serial.println(binPercent);
  //Fuzzy Logic
  /*R1*/
  if (lastTime >= 0 && lastTime <= 12 && binPercent >= 0 && binPercent <= 25) {
    binStatus = 1;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Belum ada tindakan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  } /*R2*/else if (lastTime >= 0 && lastTime <= 12 && binPercent >= 26 && binPercent <= 60) {
    binStatus = 11;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Hampir setengah muatan, dibersihkan lebih baik");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  } /*R3*/else if (lastTime >= 0 && lastTime <= 12 && binPercent >= 61 && binPercent <= 80) {
    binStatus = 2;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Segera dibersihkan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }/*R4*/else if (lastTime >= 0 && lastTime <= 12 && binPercent >= 81 && binPercent <= 100) {
    binStatus = 3;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Sangat segera dibersihkan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }

  /*R5*/else if (lastTime >= 12 && lastTime <= 18 && binPercent >= 0 && binPercent <= 25) {
    binStatus = 11;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Hampir setengah muatan, dibersihkan lebih baik");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }/*R6*/else if (lastTime >= 12 && lastTime <= 18 && binPercent >= 26 && binPercent <= 60) {
    binStatus = 2;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Segera dibersihkan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }/*R7*/else if (lastTime >= 12 && lastTime <= 18 && binPercent >= 61 && binPercent <= 80) {
    binStatus = 3;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Sangat segera dibersihkan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }/*R8*/else if (lastTime >= 12 && lastTime <= 18 && binPercent >= 81 && binPercent <= 100) {
    binStatus = 3;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Sangat segera dibersihkan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }

  /*R9*/else if (lastTime > 18 && binPercent >= 0 && binPercent <= 25) {
    binStatus = 4;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Sangat segera dibersihkan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }/*R10*/else if (lastTime > 18 && binPercent >= 26 && binPercent <= 60) {
    binStatus = 4;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Sangat segera dibersihkan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }/*R11*/else if (lastTime > 18 && binPercent >= 61 && binPercent <= 80) {
    binStatus = 4;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Sangat segera dibersihkan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }/*R12*/else if (lastTime > 18 && binPercent >= 81 && binPercent <= 100) {
    binStatus = 4;
    Firebase.set("bins/0/bin_status", binStatus);
    Firebase.set("bins/0/tindakan", "Sangat segera dibersihkan");
    Serial.print("binStatus: ");
    Serial.println(binStatus);
  }

}
