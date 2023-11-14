#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

const int pines = 23;
const int neopines = 16;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(neopines, pines, NEO_GRB + NEO_KHZ800);

float temperatura = 0.0;
bool BotonPrecionado[2] = {false};  // Array para manejar múltiples botones

void BotonRGB(int Boton1, int r, int g, int b);
void EncenderTodos(int r, int g, int b);

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  strip.begin();
  strip.show(); // Inicializa los pixeles

  Wire.begin(); // Inicializa la comunicación I2C

  // Establecer el LED Neopixel en color verde al inicio
  strip.fill(strip.Color(255, 1, 230)); // Color verde
  strip.show();
  delay(2000); // Mantener el LED encendido durante 3 segundos al inicio
  strip.clear();
  strip.show();
  strip.clear();
}

void loop() {
  while (Serial2.available() > 0) {
    char comando = Serial2.read();

    if (comando == 'p') {
      // Leer temperatura desde el sensor LM75 a través de I2C
      Wire.beginTransmission(0x48); // Dirección del sensor LM75 (0x48 en este caso)
      Wire.write(0x00); // Registro de lectura de temperatura
      Wire.endTransmission();
      Wire.requestFrom(0x48, 2); // Lea 2 bytes de datos

      if (Wire.available() >= 2) {
        int Byte1 = Wire.read();
        int Byte2 = Wire.read();
        int Tempera = (Byte1 << 8) | Byte2;
        temperatura = Tempera / 256.0;

        Serial.print("La medición del sensor es: ");
        Serial2.println(temperatura);
        Serial.println(temperatura);
      }
    }

    // Detectar pulsaciones de botones
    for (int i = 0; i < 2; ++i) {
      if (comando == '1' + i && !BotonPrecionado[i]) {
        BotonPrecionado[i] = true;
        BotonRGB(i, 150 * (i == 0), 0, 300 * (i == 1)); // Rojo para '1', Azul para '2'
      }
    }
  }
}

void BotonRGB(int Boton1, int r, int g, int b) {
  // Efecto de pulso
  for (int i = 0; i < 256; ++i) {
    int intensity = abs(128 - i); // Cambia la intensidad del color

    strip.setPixelColor(Boton1, strip.Color((r * intensity) / 128, (g * intensity) / 128, (b * intensity) / 128));
    strip.show();
    delay(10);
  }

  for (int i = 255; i >= 0; --i) {
    int intensity = abs(128 - i); // Cambia la intensidad del color

    strip.setPixelColor(Boton1, strip.Color((r * intensity) / 128, (g * intensity) / 128, (b * intensity) / 128));
    strip.show();
    delay(10);
  }

  // Apagar el LED después de la rutina
  strip.setPixelColor(Boton1, strip.Color(0, 0, 0));
  strip.show();

  // Resetear el estado del botón después de la rutina
  BotonPrecionado[Boton1] = false;
}

void EncenderTodos(int r, int g, int b) {
  // Enciende todas las LEDs con el color especificado
  for (int i = 0; i < neopines; ++i) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}
