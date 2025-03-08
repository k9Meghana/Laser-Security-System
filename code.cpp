// C++ code
//
#include <Servo.h>
#include <Adafruit_LiquidCrystal.h>

#define pirSensor 2      // PIR motion sensor
#define photodiode1 A0   // Photodiode for Laser 1
#define photodiode2 A1   // Photodiode for Laser 2
#define photodiode3 A2   // Photodiode for Laser 3
#define buzzer 6         // Buzzer for alarm
#define servoPin 9       // Servo for automatic lockdown
#define redLED 3         // Red LED for alert
#define greenLED 4       // Green LED for normal state
#define button1 5        // Push button 1 (e.g., system activation)
#define button2 8        // Push button 2 (e.g., reset alarm)
#define potentiometer A3 // Potentiometer for sensitivity control

Servo lockServo;

Adafruit_LiquidCrystal lcd(0);

void setup() {
    pinMode(pirSensor, INPUT);
    pinMode(photodiode1, INPUT);
    pinMode(photodiode2, INPUT);
    pinMode(photodiode3, INPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    lockServo.attach(servoPin);
    
    lcd.begin(16, 2);
    lcd.print("Security System");
    delay(2000);
    lcd.clear();
    lockServo.write(0); // Initial position (unlocked)
    Serial.begin(9600);
}

void loop() {
    int motionDetected = digitalRead(pirSensor);
    int lightValue1 = analogRead(photodiode1);
    int lightValue2 = analogRead(photodiode2);
    int lightValue3 = analogRead(photodiode3);
    int potValue = analogRead(potentiometer);
    int button1State = digitalRead(button1);
    int button2State = digitalRead(button2);

    Serial.print("PIR: "); Serial.print(motionDetected);
    Serial.print(" | Light1: "); Serial.print(lightValue1);
    Serial.print(" | Light2: "); Serial.print(lightValue2);
    Serial.print(" | Light3: "); Serial.println(lightValue3);
    
    if (motionDetected == HIGH || lightValue1 < 10 || lightValue2 < 10 || lightValue3 < 10) {  // If motion or any laser interruption detected
        digitalWrite(buzzer, HIGH);
        digitalWrite(redLED, HIGH);
        digitalWrite(greenLED, LOW);
        lockServo.write(90);  // Lock the door
        lcd.clear();
        lcd.print("Intruder Alert!");
        Serial.println("Intruder Detected! Locking System...");
        delay(5000);
        digitalWrite(buzzer, LOW);
    } else {
      digitalWrite(redLED, LOW);
        digitalWrite(greenLED, HIGH);
        lockServo.write(0); // Keep the door unlocked if no intrusion
        lcd.clear();
        lcd.print("System Secure");
    }
    
    // Reset alarm with button2
    if (button2State == LOW) {
        lcd.clear();
        lcd.print("Alarm Reset");
        digitalWrite(buzzer, LOW);
        digitalWrite(redLED, LOW);
        digitalWrite(greenLED, HIGH);
        lockServo.write(0);
        delay(1000);
    }
    
    delay(500);
}