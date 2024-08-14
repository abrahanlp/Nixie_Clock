#include <Wire.h>
uint8_t second, minute, hour, wday, day, month, year;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Hola Nixie");
  
  second = 30;
  minute = 30;
  hour = 20;
  wday = 4;
  day = 1;
  month = 1;
  year = 15;

  // Iniciar el intercambio de información con el DS1307 (0x68)
  Wire.beginTransmission(0x68);
 
  // Escribir la dirección del registro segundero
  Wire.write(0x00);
 
  // Escribir valores en los registros, nos aseguramos que el bit clock halt
  // en el registro del segundero este desactivado (esto hace que el reloj funcione)
  Wire.write(bin2bcd(second & 0x7F)); // <--- Esto hace que el reloj comience a trabajar
  Wire.write(bin2bcd(minute));
  Wire.write(bin2bcd(hour));
  Wire.write(bin2bcd(wday));
  Wire.write(bin2bcd(day));
  Wire.write(bin2bcd(month));
  Wire.write(bin2bcd(year));
 
  // Terminamos la escritura y verificamos si el DS1307 respondio
  Wire.endTransmission();
}

void loop() {
  delay(1000);
 
  // Leer los registros del RTC
  if (read_ds1307()) {
    // Mostrar la fecha y hora
    print_time();
  } else {
    // No se puede leer desde le DS1307 (NACK en I2C)
    Serial.println("No se detecta el DS1307, revisar conexiones");
  }
}

bool read_ds1307()
{
  // Iniciar el intercambio de información con el DS1307 (0xD0)
  Wire.beginTransmission(0x68);
 
  // Escribir la dirección del segundero
  Wire.write(0x00);
 
  // Terminamos la escritura y verificamos si el DS1307 respondio
  // Si la escritura se llevo a cabo el metodo endTransmission retorna 0
  if (Wire.endTransmission() != 0)
    return false;
 
  // Si el DS1307 esta presente, comenzar la lectura de 8 bytes
  Wire.requestFrom(0x68, 8);
 
  // Recibimos el byte del registro 0x00 y lo convertimos a binario
  second = bcd2bin(Wire.read());
  minute = bcd2bin(Wire.read()); // Continuamos recibiendo cada uno de los registros
  hour = bcd2bin(Wire.read());
  wday = bcd2bin(Wire.read());
  day = bcd2bin(Wire.read());
  month = bcd2bin(Wire.read());
  year = bcd2bin(Wire.read());
 
  // Recibir los datos del registro de control en la dirección 0x07
  //ctrl = Wire.read();
 
  // Operacion satisfactoria, retornamos verdadero
  return true;
}

/**
   Imprime la fecha y hora al monitor serial de arduino
*/
void print_time()
{
  Serial.print("Fecha: ");
  Serial.print(day);
  Serial.print('/');
  Serial.print(month);
  Serial.print('/');
  Serial.print(year);
 
  Serial.print("  Hora: ");
  Serial.print(hour);
  Serial.print(':');
  Serial.print(minute);
  Serial.print(':');
  Serial.print(second);
 
  Serial.println();
}

/**
   Convierte un numero binario a BCD
*/
uint8_t bin2bcd(uint8_t bin)
{
  return (bin / 10 * 16) + (bin % 10);
}
uint8_t bcd2bin(uint8_t bcd)
{
  // Convertir decenas y luego unidades a un numero binario
  return (bcd / 16 * 10) + (bcd % 16);
}
