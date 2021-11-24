// Load libraries
#include "BluetoothSerial.h"

// Estou Fazendo um codigo para utilizar um esp32 para ser meu conversor bluetooth
// Caso aconteça algum erro utilizando o HC-05 temos uma outra opção

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;

// GPIO where LED is connected to
const int ledPin = 25;

// Handle received and sent messages
String message = "";
char incomingChar;
String temperatureString = "";

// Timer: auxiliar variables
unsigned long previousMillis = 0; // Stores last time temperature was published
const long interval = 10000;      // interval at which to publish sensor readings

void setup()
{
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);
    // Bluetooth device name
    SerialBT.begin("CAIXA-DAGUA");
    Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop()
{
    // Read received messages (LED control command)
    if (Serial.available())
    {
        SerialBT.write(Serial.read());
    }
    bool envio = false;

    if (SerialBT.available())
    {
        char incomingChar = SerialBT.read();
        if (incomingChar != '\n')
        {
            message += String(incomingChar);
        }
        else
        {
            message = "";
        }

        Serial.write(incomingChar);
        // Check received message and control output accordingly
        if (message.startsWith("ev") && !envio)
        {
            SerialBT.println("evok\n");
            envio = true;
            message = "";
        }
        else if (message == "lr" && !envio)
        {
            SerialBT.println("ev,18,12,15,17,\n");
            envio = true;
            message = "";
        }
        else if (message == "bb" && !envio)
        {
            SerialBT.println("bbok\n");
            envio = true;
            message = "";
        }
    }
    delay(500);
}
