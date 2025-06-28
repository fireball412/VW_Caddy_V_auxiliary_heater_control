// values from experience
//const int servo_on = 125;
//const int servo_neutral  = 90;
//const int servo_off  = 55;


#include <ESP32Servo.h>

Servo myServo;

const int servoPin = 5; // Use any PWM-capable pin, e.g., GPIO 5

void setup() {
  Serial.begin(115200);
  myServo.setPeriodHertz(50); // Standard 50 Hz for servos
  myServo.attach(servoPin, 500, 2400); // Pin, min pulse width, max pulse width (in microseconds)
  myServo.write(90); // Start at neutral

  Serial.println("ESP32 Servo Angle Setter Ready.");
  Serial.println("Type an angle between 0 and 180 and press Enter:");
}

void loop() {
  static String inputString = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n' || inChar == '\r') {
      if (inputString.length() > 0) {
        int angle = inputString.toInt();
        if (angle >= 0 && angle <= 180) {
          myServo.write(angle);
          Serial.print("Servo set to ");
          Serial.print(angle);
          Serial.println(" degrees.");
        } else {
          Serial.println("Invalid angle. Enter value 0-180.");
        }
        inputString = "";
        Serial.println("Type another angle (0-180):");
      }
    } else if (isDigit(inChar)) {
      inputString += inChar;
    }
    // ignore other characters
  }
}
