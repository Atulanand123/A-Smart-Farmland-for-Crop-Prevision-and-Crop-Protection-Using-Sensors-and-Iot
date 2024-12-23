Share


You said:
#include <LiquidCrystal.h> #include <SimpleDHT.h> #include <stdio.h>
#include <SoftwareSerial.h> int pinDHT11 = 5; SimpleDHT11 dht11; SoftwareSerial mySerial(2,3);
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13; LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int IR = 4;

const int MOISTURE = 6; const int PUMP= 7;
const int BUZZER= A2;



#define SensorPin A0	//pH meter Analog output to Arduino Analog Input 0
 
#define Offset 0.00	//deviation compensate #define samplingInterval 20
#define LED 13

#define printInterval 800

#define ArrayLenth 40	//times of collection

int pHArray[ArrayLenth]; //Store the average value of the sensor feedback int pHArrayIndex=0;


int sampletime = 280; int deltatime = 40;
int sleeptime = 9680;



String inputString=""; void setup() {
// put your setup code here, to run once: Serial.begin(9600); mySerial.begin(9600);
lcd.begin(16,2); pinMode(IR , INPUT);
pinMode(BUZZER , OUTPUT);
 
pinMode(MOISTURE , INPUT); pinMode(PUMP , OUTPUT);
digitalWrite(PUMP, LOW); digitalWrite(BUZZER, LOW);
lcd.clear(); lcd.setCursor(0,0);
lcd.print("IOT BASE AGRI"); lcd.setCursor(0,1); lcd.print("SYSTEM "); delay(3000);




}

void read_object(void); void read_soil(void);
void temperaturecheck(void); void read_ph(void);


void loop() {

// put your main code here, to run repeatedly:
 
static unsigned long samplingTime = millis(); static unsigned long printTime = millis(); static float pHValue,voltage;
read_object(); read_soil(); temperaturecheck();
if(millis()-samplingTime > samplingInterval)

{

pHArray[pHArrayIndex++]=analogRead(SensorPin); if(pHArrayIndex==ArrayLenth)pHArrayIndex=0; voltage = avergearray(pHArray, ArrayLenth)*5.0/1024; pHValue = 3.5*voltage+Offset; samplingTime=millis();
}

if(millis() - printTime > printInterval)	//Every 800 milliseconds, print a numerical, convert the state of the LED indicator

{

// Serial.print("Voltage:");

// Serial.print(voltage,2); mySerial.print("	pH value: "); mySerial.println(pHValue,2);
 
lcd.clear(); lcd.setCursor(0, 0); lcd.print("pH:"); lcd.setCursor(3, 0); lcd.print(pHValue);
if(pHValue>=1 && pHValue <=4)

{

lcd.clear(); lcd.setCursor(0, 0);
lcd.print("pH: ACIDIC");delay(2000); mySerial.println("pH:ACIDIC");


}

else if(pHValue>=5 && pHValue <=8)

{

lcd.clear(); lcd.setCursor(0, 0);
lcd.print("pH: NETURAL");delay(2000); mySerial.println("pH:NETURAL");
}
 
else if(pHValue>=8 && pHValue <=14)

{

lcd.clear(); lcd.setCursor(0, 0); lcd.print("pH: BASIC");
mySerial.println("pH:BASIC"); delay(2000);
}

// digitalWrite(LED,digitalRead(LED)^1); printTime=millis();


}

while (Serial.available())

{

if(Serial.available())

{

inputString=Serial.readString();

}

delay(500); if(inputString.substring(0,1)=="1")
 
{

inputString="";



mySerial.println("ANIMAL FOUND");

}

if(inputString.substring(0,1)=="2")

{

inputString="";



mySerial.println("ANIMAL FOUND");

}

}

}

double avergearray(int* arr, int number){ int i;
int max,min; double avg; long amount=0; if(number<=0){
Serial.println("Error number for the array to avraging!/n");
 
return 0;

}

if(number<5){ //less than 5, calculated directly statistics for(i=0;i<number;i++){
amount+=arr[i];

}

avg = amount/number; return avg;
}else{ if(arr[0]<arr[1]){
min = arr[0];max=arr[1];

}

else{ min=arr[1];max=arr[0];
}

for(i=2;i<number;i++){ if(arr[i]<min){
amount+=min;	//arr<min min=arr[i];
}else {
 
if(arr[i]>max){ amount+=max;	//arr>max max=arr[i];
}else{

amount+=arr[i]; //min<=arr<=max

}

}//if

}//for

avg = (double)amount/(number-2);

}//if

return avg; delay(100);
}

void temperaturecheck()

{

int tempcnt = 0; int humycnt = 0;
byte temperature = 0; byte humidity = 0;
int err = SimpleDHTErrSuccess;
 
if	((err	=	dht11.read(pinDHT11,	&temperature,	&humidity,	NULL))	!= SimpleDHTErrSuccess)

{

// mySerial.print("Read DHT11 failed, err="); mySerial.println(err);delay(1000);

// mySerial.println("TEMP:"); mySerial.println("29");

// mySerial.println("HUMDITY:");mySerial.println("60"); return;
}

//It will give temperature value mySerial.println("TEMP:"); mySerial.println((int)temperature); lcd.clear();
lcd.setCursor(0,0); lcd.print("TEMP:"); lcd.setCursor(0,1); lcd.print((int)temperature);
//It will give Humidity value delay(3000); mySerial.println("HUMDITY:"); mySerial.println((int)humidity); lcd.clear();
 
lcd.setCursor(0,0); lcd.print("HUMDITY:"); lcd.setCursor(0,1); lcd.print((int)humidity);
delay(3000); if((int)temperature >35)
{

mySerial.println("HIGH TEMPERATURE	"); lcd.clear();
lcd.setCursor(0,0); lcd.print("HIGH "); lcd.setCursor(0,1); lcd.print("TEMPERATURE ");
}

else

{

mySerial.println("NORMAL TEMPERATURE	"); lcd.clear();
lcd.setCursor(0,0); lcd.print("NORMAL ");
 
lcd.setCursor(0,1); lcd.print("TEMPERATURE ")
}

delay(3000);



if((int)humidity >100)

{



lcd.clear(); lcd.setCursor(0,0); lcd.print("HIGH "); lcd.setCursor(0,1); lcd.print("HUMIDITY ");
mySerial.println("HIGH HUMIDITY	");

}

else

{

lcd.clear(); lcd.setCursor(0,0); lcd.print("NORMAL ");
 
lcd.setCursor(0,1); lcd.print("HUMIDITY ");
mySerial.println("NORMAL HUMIDITY	");

}

delay(3000);



}

void read_object(void)

{



if(digitalRead(IR) == 0)

{



mySerial.println("OBJECT DETECTED"); lcd.clear();
lcd.setCursor(0,0); lcd.print("OBJECT "); lcd.setCursor(0,1); lcd.print("DETECTED"); digitalWrite(BUZZER, HIGH)
 
}

else

{

mySerial.println("NO OBJECT DETECTED"); lcd.clear();
lcd.setCursor(0,0); lcd.print("NO OBJECT"); lcd.setCursor(0,1); lcd.print("DETECTED");
digitalWrite(BUZZER, LOW);

}

delay(3000);

}

void read_soil(void)

{

if(digitalRead(MOISTURE) == 1)

{

mySerial.println("LAND IS DRY PUMP ON"); lcd.clear();
lcd.setCursor(0,0);
 
lcd.print("LAND IS DRY"); lcd.setCursor(0,1); lcd.print("PUMP ON"); digitalWrite(PUMP, HIGH); delay(3000); digitalWrite(PUMP, LOW); delay(1000)
}

else if(digitalRead(MOISTURE) == 0)

{

mySerial.println("LAND IS WET PUMP OFF"); lcd.clear();
lcd.setCursor(0,0); lcd.print("LAND IS WET"); lcd.setCursor(0,1); lcd.print("PUMP OFF");
}

delay(3000);

}