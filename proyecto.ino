#include <LiquidCrystal.h>
#include <Servo.h>

#include <DHT.h>
Servo servoMotor;
LiquidCrystal lcd(12, 11, 8, 7, 4, 2);
int buzzerPin = 10;
const int trigPin = 3;
const int echoPin = 5;
const int greenLedPin = 9;
#define DHTTYPE DHT11
#define DHT11_PIN 13
DHT dht (DHT11_PIN,DHTTYPE);
float duracion;
float distancia = 0;

// Define los símbolos personalizados para Celsius y grados
byte celsiusSymbol[8] = {
  B00110,B01001,B01000,B01000,B01001,B00110,B00000,B00000
};
//borrar ambas partes
byte degreesSymbol[8] = {
  B00100,B01010,B00100,B00000,B00000,B00000,B00000,B00000
};

void setup() {
  lcd.begin(16, 2); // Inicializa la pantalla LCD 16x2
  Serial.begin(9600); // Inicializa la comunicación serial
  servoMotor.attach(6); // Inicializa el pin por el cual se comunica el servoMotor
  servoMotor.write(0); // Coloca el servoMotor a 0 grados
  dht.begin(); // Inicializa el sensor DHT11
  pinMode(trigPin, OUTPUT); // Pin de recepción echo de ultrasonico
  pinMode(buzzerPin, OUTPUT); // Pin de buzzer
  pinMode(greenLedPin,OUTPUT);
  pinMode(echoPin, INPUT); // Pin de echo
  // Cargar los símbolos personalizados en la memoria del LCD
  lcd.createChar(0, celsiusSymbol);
  lcd.createChar(1, degreesSymbol);
}

void loop() {
  //sensor ultrasonico
  digitalWrite(echoPin, LOW);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duracion = pulseIn(echoPin, HIGH);
  distancia = (duracion / 2) * 0.0343;
  float distancia2 = constrain(distancia, 5, 20);//modificar dstancias
  // Calcular el porcentaje de acuerdo a la distancia medida
  float porcentajeNivelAgua = ((20 - distancia2) / 15) * 100;//cambiar distancias
  // Muestra el texto en la pantalla LCD
  lcd.setCursor(0,0);
  lcd.print("Lv.Water: ");
  lcd.print(porcentajeNivelAgua);
  lcd.print("%");
  if (porcentajeNivelAgua <= 15) {
    tone(buzzerPin, 5000, 200);
  } else {
    noTone(buzzerPin); // Detener el tono si está dentro del rango
  }
  // Sensor DHT11
  // Obtiene la lectura del DHT11 
  float temp = dht.readTemperature();
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.write(byte(1));
  lcd.write(byte(0));
  if(temp > 32 ){
      servoMotor.write(45);//Desplazamos a la velocidad 45°
      delay(500);//Esperamos 0.5 segundo
      digitalWrite(greenLedPin, HIGH);
      servoMotor.write(90);//Desplazamos a la velocidad 45°
      delay(500);//Esperamos 0.5 segundo
      servoMotor.write(135);//Desplazamos a la velocidad 90°
      delay(500);//Esperamos 0.5 segundo
      servoMotor.write(90);//Desplazamos a la posición 45º
      delay(500);// Esperamos 0.5 segundo
      servoMotor.write(45);//Desplazamos a la posición 90º
      delay(500);// Esperamos 0.5 segundo
  }else{
    servoMotor.write(0);
    digitalWrite(greenLedPin, LOW);
  }
}