#include <LiquidCrystal.h>
#include <DHT.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastUpdate = 0;
const unsigned long UPDATE_MS = 1000;

//print padded so old chars get overwritten
void printLine(uint8_t row, const String &text) {
  lcd.setCursor(0, row);
  String t = text;
  if (t.length() > 16) t = t.substring(0, 16);
  lcd.print(t);
  for (int i = t.length(); i < 16; i++) lcd.print(' ');
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  printLine(0, "Env dashboard");
  printLine(1, "starting...");
  delay(800);

  dht.begin();
  lcd.clear();
}

void loop() {
    unsigned long now = millis();
  if (now - lastUpdate < UPDATE_MS) return;
  lastUpdate = now;

  float h = dht.readHumidity();
  float tC = dht.readTemperature(); // celsius
  float tF = dht.readTemperature(true); //fahrenheit

  // if sensor read failed, show an error (if wiring is off etc)
  if (isnan(h) || isnan(tC) || isnan(tF)) {
    printLine(0, "DHT read failed");
    printLine(1, "check wiring");
    return;
  }

  // comfort label based on humidity
  String comfort;
  if (h < 30) comfort = "DRY";
  else if (h <= 60) comfort = "OK";
  else comfort = "HUMID";

  // line 1: temp + humidity
  // ex: "T:72.3F H:45%"
  String line1 = "T:" + String(tF, 1) + "F H:" + String((int)h) + "%";
  printLine(0, line1);

  // line 2: comfort + celsius (or heat index later)
  // ex: "OK 22.4C"
  String line2 = comfort + "  " + String(tC, 1) + "C";
  printLine(1, line2);
}
