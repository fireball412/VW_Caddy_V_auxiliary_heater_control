#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h> // Use ESP32Servo instead of Servo

#define ONE_WIRE_BUS 4 // DS18B20 data pin
const float TEMP_THRESHOLD_LOW_C = 18.0;
const float TEMP_THRESHOLD_HIGH_C = 35.0;
const float TEMP_NOTVALID_C = 200.0;


// Servo configuration
#define SERVO_PIN 5 // GPIO for servo
const int servo_on = 125;
const int servo_neutral  = 90;
const int servo_off  = 55;

const unsigned long SERVO_HOLD_DURATION_MS = 3500;      // 3.5 second
const unsigned long SERVO_COOLDOWN_MS = 120UL * 60UL * 1000UL; // 2h

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Servo myServo; // ESP32Servo

unsigned long last_servo_activation = 0;
bool servo_in_cooldown = true;

void moveServoPulse() {
  myServo.write(servo_on);
  delay(SERVO_HOLD_DURATION_MS);
  myServo.write(servo_neutral);
}

void setup() {
  Serial.begin(115200); 
  sensors.begin();
  Serial.println("DS18B20 Temperature Sensor Example");

  myServo.attach(SERVO_PIN, 500, 2400); // Attach with pulse width range!
  myServo.write(servo_neutral);
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  unsigned long now2 = millis();

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
    rgbLedWrite(RGB_BUILTIN, RGB_BRIGHTNESS, RGB_BRIGHTNESS, 0);  // yelloe
  } else {
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");

    if (tempC < TEMP_THRESHOLD_LOW_C) {
      rgbLedWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS);  // Blue
      
      if (!servo_in_cooldown && (now2 - last_servo_activation > SERVO_COOLDOWN_MS)) {
        moveServoPulse();
        last_servo_activation = now2;
        servo_in_cooldown = true;
        Serial.println("Servo activated");        
        }
     } else {
        rgbLedWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red      
     }
   }

  if (servo_in_cooldown && (now2 - last_servo_activation > SERVO_COOLDOWN_MS)) {
      servo_in_cooldown = false;
    }

  delay(2000);
}
