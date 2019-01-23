//Arduino based GPIO expander for xCraft
//
//Written by: Marshall Wingerson
//
//

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

#define LED_DELAY 250	//millisseconds
#define GPIO_DELAY 100
#define SERIAL_DELAY 100
#define HUMID_DELAY 2000

#define GPIO_POLARITY HIGH

#define GPIO_PIN_1 3
#define GPIO_PIN_2 4
#define GPIO_PIN_3 5
#define GPIO_PIN_4 6
#define GPIO_PIN_5 7

String gpioStr1  = String("set 3");
String gpioStr2  = String("set 4");
String gpioStr3  = String("set 5");
String gpioStr4  = String("set 6");
String gpioStr5  = String("set 7");
String gpioStr6  = String("clr 3");
String gpioStr7  = String("clr 4");
String gpioStr8  = String("clr 5");
String gpioStr9  = String("clr 6");
String gpioStr10 = String("clr 7");
String gpioStr11 = String("stat 3");
String gpioStr12 = String("stat 4");
String gpioStr13 = String("stat 5");
String gpioStr14 = String("stat 6");
String gpioStr15 = String("stat 7");
String gpioStr16 = String("humid");

unsigned int currTime;
unsigned int lastLED;
unsigned int lastGPIO;
unsigned int lastSerial;
unsigned int lastHumidity;
bool blinkOn;

#define buffSize 100
char buffer[buffSize];
int buffIt;
bool buffRdy;

float humidity;
float temperature;

void initTimers(){
  //Serial.println("Initializing timers");
  lastLED = millis();
  lastLED = lastGPIO;
  lastLED = lastGPIO;
}

void initSerial(){	
  Serial.begin(115200);
  Serial.println("Starting xCraft GPIO Expander\n\rBy: Marshall Wingerson");
}

void initGPIO(){

	//Serial.println("Initializing GPIO");

	blinkOn = false;

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(GPIO_PIN_1, OUTPUT);
	pinMode(GPIO_PIN_2, OUTPUT);
	pinMode(GPIO_PIN_3, OUTPUT);
	pinMode(GPIO_PIN_4, OUTPUT);
	pinMode(GPIO_PIN_5, OUTPUT);

	digitalWrite(GPIO_PIN_1, GPIO_POLARITY);
	digitalWrite(GPIO_PIN_2, GPIO_POLARITY);
	digitalWrite(GPIO_PIN_3, GPIO_POLARITY);
	digitalWrite(GPIO_PIN_4, GPIO_POLARITY);
	digitalWrite(GPIO_PIN_5, GPIO_POLARITY);
}

void clearbuff(){
	for(int i=0; i<buffSize; i++)
		buffer[i] = 0;
}

void SendDatHumidity(){
	Serial.println(humidity);
	Serial.println(temperature);
}

char gpioStatus(int pin){

	uint8_t bitmask = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);

	return (*portOutputRegister(port) & bitmask) ? '1' : '0';
}

void LEDSrv(){
	blinkOn = !blinkOn;

	digitalWrite(LED_BUILTIN, blinkOn);
}

void GPIOSrv(){
	//Serial.println("In GPIOSrv()");
	if(buffRdy){
		buffRdy = false;

		if((String)buffer == gpioStr1)
			digitalWrite(GPIO_PIN_1, HIGH);
		else if((String)buffer == gpioStr2)
			digitalWrite(GPIO_PIN_2, HIGH);
		else if((String)buffer == gpioStr3)
			digitalWrite(GPIO_PIN_3, HIGH);
		else if((String)buffer == gpioStr4)
			digitalWrite(GPIO_PIN_4, HIGH);
		else if((String)buffer == gpioStr5)
			digitalWrite(GPIO_PIN_5, HIGH);

		else if((String)buffer == gpioStr6)
			digitalWrite(GPIO_PIN_1, LOW);
		else if((String)buffer == gpioStr7)
			digitalWrite(GPIO_PIN_2, LOW);
		else if((String)buffer == gpioStr8)
			digitalWrite(GPIO_PIN_3, LOW);
		else if((String)buffer == gpioStr9)
			digitalWrite(GPIO_PIN_4, LOW);
		else if((String)buffer == gpioStr10)
			digitalWrite(GPIO_PIN_5, LOW);

		else if((String)buffer == gpioStr11)
			Serial.println(gpioStatus(GPIO_PIN_1));
		else if((String)buffer == gpioStr12)
			Serial.println(gpioStatus(GPIO_PIN_2));
		else if((String)buffer == gpioStr13)
			Serial.println(gpioStatus(GPIO_PIN_3));
		else if((String)buffer == gpioStr14)
			Serial.println(gpioStatus(GPIO_PIN_4));
		else if((String)buffer == gpioStr15)
			Serial.println(gpioStatus(GPIO_PIN_5));
		else if((String)buffer == gpioStr16){
			SendDatHumidity();
		}
		else
			Serial.println("Unrecognized command!");

		clearbuff();
	}
}

void serialSrv(){
	char tempByte;

	//Serial.println("in serialSrv()");
	//Serial.print("buff: ");
	//Serial.println(buffer);

	lastSerial = millis();
	if(Serial.available() == 0){
		//Serial.println("Nothing in buffer");
		return;
	}

	tempByte = Serial.read();

	Serial.print(tempByte);

	//end of string
	if(tempByte == '\r'){
		//Serial.println("Terminating string");
		buffer[buffIt] = 0;
		Serial.read();
		buffRdy = true;
		buffIt = 0;
		return;
	}

	// if(tempByte == '\n')
	// 	Serial.println("Ignoring NL");
	// else
		buffer[buffIt++] = tempByte;
}

void humiditySrv(){
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(true);

  //Serial.println("Got new sesor values!!!");
  //Serial.print("Humidity: ");
  //Serial.println(humidity);
  //Serial.print("Temp: ");
  //Serial.println(temperature);
}

void setup() {

  initSerial();
  initGPIO();

  //Serial.println("Initializing humidity Sensor AM2302 - DHT22");
  dht.begin();

  initTimers();

  //Serial.println("Starting loop()");
 }

void loop() {

  currTime = millis();

  if((currTime - lastLED) > LED_DELAY){
  	lastLED = currTime;
  	LEDSrv();
  }

  if((currTime - lastSerial) > SERIAL_DELAY){
  	lastSerial = currTime;
  	serialSrv();
  }

  if((currTime - lastGPIO) > GPIO_DELAY){
  	lastGPIO = currTime;
  	GPIOSrv();
  }
  if((currTime - lastHumidity) > HUMID_DELAY){
  	lastHumidity = currTime;
  	humiditySrv();
  }
}


