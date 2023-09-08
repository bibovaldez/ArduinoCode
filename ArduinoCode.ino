// include the library use
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
//  initialize the arduino data holder
int temperature = 0;
int humidity = 0;
bool fanState = false;
bool airConState = false;
bool AutoManState = false;
//  initialize the arduino board pin
#define AutoManButton 6
#define Relay_FanButton 7
#define Relay_AirCButton 8
#define red1 2
#define red2 3
#define red3 4
#define red4 5
#define Relay_Fan 9
#define Relay_AirC 10
#define DHTPIN 11      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 22  (AM2302), AM2321
DHT_Unified dht(DHTPIN, DHTTYPE);
// button state
int autoManButtonState = LOW;         // Current state of the button
int lastAutoManButtonState = LOW;     // Previous state of the button
int fanButtonState = LOW;             // Current state of the button
int lastFanButtonState = LOW;         // Previous state of the button
int airConButtonState = LOW;          // Current state of the button
int lastAirConButtonState = LOW;      // Previous state of the button
unsigned long lastDebounceTime1 = 0;  // Last time the button state changed
unsigned long lastDebounceTime2 = 0;  // Last time the button state changed
unsigned long lastDebounceTime3 = 0;  // Last time the button state changed
unsigned long debounceDelay = 10;     // Debounce time
unsigned long Time = 0;
unsigned long Time1 = 0;
byte block[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
void loading() {
  for (int u = 0; u < 2; u++) {
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(x, 0);
      lcd.write(byte(0));
      lcd.setCursor(x, 1);
      lcd.write(byte(0));
      // light up the LEDs
      if (x < 4) {
        digitalWrite(red1, HIGH);
      } else if (x < 8) {
        digitalWrite(red2, HIGH);
      } else if (x < 12) {
        digitalWrite(red3, HIGH);
      } else if (x < 16) {
        digitalWrite(red4, HIGH);
      }
      delay(5);
    }
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(x, 0);
      lcd.print(" ");
      lcd.setCursor(x, 1);
      lcd.print(" ");
      // light up the LEDs
      if (x < 4) {
        digitalWrite(red1, LOW);
      } else if (x < 8) {
        digitalWrite(red2, LOW);
      } else if (x < 12) {
        digitalWrite(red3, LOW);
      } else if (x < 16) {
        digitalWrite(red4, LOW);
      }
      delay(10);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  // initialize the LCD
  // lcd.begin();
  lcd.init();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  pinMode(red1, OUTPUT);                    // led red1
  pinMode(red2, OUTPUT);                    // led red2
  pinMode(red3, OUTPUT);                    // led red3
  pinMode(red4, OUTPUT);                    // led red4
  pinMode(Relay_Fan, OUTPUT);               // FanOutput
  pinMode(Relay_AirC, OUTPUT);              // AirconOutput
  pinMode(AutoManButton, INPUT_PULLUP);     // autoMan Button
  pinMode(Relay_FanButton, INPUT_PULLUP);   // Fan Button
  pinMode(Relay_AirCButton, INPUT_PULLUP);  // Aircon Button
  lcd.createChar(0, block);
  loading();
}
void loop() {
  // For Auto and Manual
  int reading1 = digitalRead(AutoManButton);
  if (reading1 != lastAutoManButtonState) {
    lastDebounceTime1 = millis();
  }
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != autoManButtonState) {
      autoManButtonState = reading1;
      if (autoManButtonState == LOW) {
        AutoManState = !AutoManState;
      }
    }
  }
  lastAutoManButtonState = reading1;
  // For FanButton
  int reading2 = digitalRead(Relay_FanButton);
  if (reading2 != lastFanButtonState) {
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (reading2 != fanButtonState) {
      fanButtonState = reading2;
      if (fanButtonState == LOW) {
        if (AutoManState) {
          AutoManState = !AutoManState;
        }
        airConState = !airConState;
      }
    }
  }
  lastFanButtonState = reading2;
  // For airConButton
  int reading3 = digitalRead(Relay_AirCButton);
  if (reading3 != lastAirConButtonState) {
    lastDebounceTime3 = millis();
  }
  if ((millis() - lastDebounceTime3) > debounceDelay) {
    if (reading3 != airConButtonState) {
      airConButtonState = reading3;
      if (airConButtonState == LOW) {
        if (AutoManState) {
          AutoManState = !AutoManState;
        }
        fanState = !fanState;
      }
    }
  }
  lastAirConButtonState = reading3;
  // Temperature
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  temperature = event.temperature;
  // Humidity
  dht.humidity().getEvent(&event);
  humidity = event.relative_humidity;

  // LED indicator
  if (humidity != 0 && temperature != 0) {
    if (temperature < 30) {
      digitalWrite(red1, HIGH);
      digitalWrite(red2, LOW);
      digitalWrite(red3, LOW);
      digitalWrite(red4, LOW);
    } else if (temperature >= 30 && temperature < 33) {
      digitalWrite(red1, HIGH);
      digitalWrite(red2, HIGH);
      digitalWrite(red3, LOW);
      digitalWrite(red4, LOW);
    } else if (temperature >= 33 && temperature < 35) {
      digitalWrite(red1, HIGH);
      digitalWrite(red2, HIGH);
      digitalWrite(red3, HIGH);
      digitalWrite(red4, LOW);
    } else if (temperature >= 35 && temperature < 36) {
      digitalWrite(red1, HIGH);
      digitalWrite(red2, HIGH);
      digitalWrite(red3, HIGH);
      digitalWrite(red4, HIGH);
    } else if (temperature >= 36) {
      // warning signal
      digitalWrite(red1, LOW);
      digitalWrite(red2, LOW);
      digitalWrite(red3, LOW);
      digitalWrite(red4, LOW);
      delay(300);
      digitalWrite(red1, HIGH);
      digitalWrite(red2, HIGH);
      digitalWrite(red3, HIGH);
      digitalWrite(red4, HIGH);
      delay(300);
    }
  }
  // Changing State in Auto
  if (AutoManState) {
    // normal condition is 28-30 C and 75-85%
    // balance the temperature and humidity
    if (temperature >= 33 && humidity > 85) {  // high temperature and high humidity
      digitalWrite(Relay_AirC, HIGH);          // para mabilis ang pagbaba ng temperature at humidity
      airConState = true;
      digitalWrite(Relay_Fan, LOW);
      fanState = false;
    } else if (temperature >= 33 && humidity <= 75) {  // high temperature and low humidity
      digitalWrite(Relay_AirC, HIGH);                  // on dalawa para mabilis ang pagbaba ng temperature at pagtaas ng humidity
      airConState = true;
      digitalWrite(Relay_Fan, HIGH);
      fanState = true;
    } else if (temperature < 28 && humidity > 85) {  // low temperature and high humidity
      digitalWrite(Relay_AirC, LOW);                 // if may heater pwede ni e on pero walay heater
      airConState = false;
      digitalWrite(Relay_Fan, LOW);
      fanState = false;
    } else if (temperature <= 28 && humidity <= 75) {  // low temperature and low humidity
      digitalWrite(Relay_AirC, LOW);                   // pinapababa ng aircon ang humid at temperature kaya fan lang
      airConState = false;
      digitalWrite(Relay_Fan, HIGH);
      fanState = true;
    } else if (temperature >= 33 && humidity <= 85) {  // high temperature and low humidity
      digitalWrite(Relay_AirC, HIGH);                  // on dalawa para mabilis ang pagbaba ng temperature at pagtaas ng humidity
      airConState = true;
      digitalWrite(Relay_Fan, HIGH);
      fanState = true;
    } else {  // if any condition is not met then turn off the aircon and fan
      digitalWrite(Relay_AirC, LOW);
      airConState = false;
      digitalWrite(Relay_Fan, LOW);
      fanState = false;
    }

  } else {  // manual
    if (airConState) {
      digitalWrite(Relay_AirC, HIGH);
    } else {
      digitalWrite(Relay_AirC, LOW);
    }
    if (fanState) {
      digitalWrite(Relay_Fan, HIGH);
    } else {
      digitalWrite(Relay_Fan, LOW);
    }
  }

  // reading button from client
  //  Reading And control condition
  if (Serial.available() > 0) {
    char controlCode = Serial.read();
    if (controlCode == '\n') {
      return;
    } else if (controlCode == 'A') {
      AutoManState = !AutoManState;
    }
    if (!AutoManState) {
      if (controlCode == 'F') {
        fanState = !fanState;
      } else if (controlCode == 'W') {
        airConState = !airConState;
      }
    }
  }
  Time = millis();
  //  print the data every 250ms
  if (Time % 250 == 0) {
    Serial.print(String(temperature) + "," + String(humidity) + "," + String(AutoManState) + "," + String(airConState) + "," + String(fanState) + "\n");
    Time = 0;
  }
  //  display the data every 3s
  Time1 = millis();
  if (Time1 % 3000 == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
  }
  if (Time1 % 6000 == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ac:");
    if (airConState) {
      lcd.print("ON   ");
    } else {
      lcd.print("OFF  ");
    }
    lcd.print("Fan:");
    if (fanState) {
      lcd.print("ON");
    } else {
      lcd.print("OFF");
    }
    lcd.setCursor(0, 1);
    if (AutoManState) {
      lcd.print("Auto");
    } else {
      lcd.print("Manual");
    }
    Time1 = 0;
  }
}
