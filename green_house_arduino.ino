#include <LiquidCrystal.h>
#include <stdio.h>
#include <SoftwareSerial.h>


LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
// defines pins numbers
//New GPS GY-GPS6MV2

SoftwareSerial mySerial(8, 9);

#include <Wire.h>
#include "dht.h"
#define dht_apin A5
dht DHT;

int sms = 2;
int relay1 = 3;
int relay2 = 4;
int relay3 = 5;

int buzzer  = 7;

int units=0,amount=0;


void beep()
{
  digitalWrite(buzzer, LOW);delay(1500);digitalWrite(buzzer, HIGH);delay(200);
}


char pwds='f';
int dist1,dist2,dist3,sts1=0;

long duration;
int distanceCm, distanceInch;

int memsx=0,memsy=0;
 
 unsigned char rcv,count,gchr='x',gchr1='x',robos='s';
 
 char pastnumber[31];
 char pastnumber1[11],pastnumber2[11],pastnumber3[11];
 
 char gpsval[50];
// char dataread[100] = "";
// char lt[15],ln[15];


int i=0,k=0,lop=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";


int hbtc=0,hbtc1=0,rtrl=0;

unsigned char gv=0,msg1[10],msg2[11];
 float lati=0,longi=0;
unsigned int lati1=0,longi1=0;
unsigned char flat[5],flong[5];
unsigned char finallat[8],finallong[9];

char keypads='x';
 int ii=0;
 int tempc=0,humc=0;


float weight=0;
float vout=0;

int sti=0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


char cntt=0;
char pd=0,pwd[5];



void okcheck()
{
  unsigned char rcr;
  do{
      rcr = Serial.read();
    }while(rcr != 'K');
}


void send_link()
 {
    Serial.write("AT+CMGS=\"");
    Serial.write(pastnumber);
    Serial.write("\"\r\n");  delay(2500);  
     Serial.write(0x1A);delay(4000);delay(4000);
 }


 
void setup() 
{
 // unsigned char rcr;
 Serial.begin(115200);//serialEvent();
 mySerial.begin(9600);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(sms, INPUT); 
 
  pinMode(buzzer, OUTPUT);

  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(buzzer, HIGH);


  lcd.begin(16, 2);lcd.cursor();  
  lcd.print("  WELCOME TO  ");
  lcd.setCursor(0,1);
  lcd.print("   THE PROJECT");
  delay(2000);
  
  //get_gps();
  //gps_convert();

    lcd.clear();
    lcd.setCursor(0,0);
     for(ii=0;ii<=6;ii++) lcd.write(finallat[ii]);
     
     lcd.setCursor(0,1);
     for(ii=0;ii<=7;ii++) lcd.write(finallong[ii]);    
   
  delay(1500); 
      lcd.clear();

  lcd.print("Wifi init");

  Serial.write("AT\r\n");         delay(500); okcheck();
  Serial.write("ATE0\r\n");         okcheck();
  Serial.write("AT+CWMODE=3\r\n"); delay(500);
  Serial.write("AT+CIPMUX=1\r\n"); delay(500);         okcheck();
  Serial.write("AT+CIPSERVER=1,23\r\n");      okcheck();


  lcd.clear();
  lcd.print("Waiting For");
  lcd.setCursor(0, 1);
  lcd.print("Connection");
  do {
    rcv = Serial.read();
  } while (rcv != 'C');


  lcd.clear();
  lcd.print("Connected");
  delay(2000);
  lcd.clear();
      lcd.setCursor(0,0);
    lcd.print("T:");  //2-3-4,0
    lcd.setCursor(5,0);
    lcd.print("H:");  //7-8-9,0
    lcd.setCursor(0,1);
    lcd.print("Soil:");  //7-8-9,    
}


int otp_n=0;
void loop() 
{
DHT.read11(dht_apin);

      tempc = DHT.temperature;
      humc  = DHT.humidity;

    lcd.setCursor(2,0);convertl(tempc);
    lcd.setCursor(7,0);convertl(humc);
    if(tempc > 40)
      {beep();
                   digitalWrite(relay1, LOW);
    Serial.write("AT+CIPSEND=0,23\r\n");delay(2000);
    Serial.write("High Temp:\r\n");
    converts(tempc);//3
    Serial.write(0x1A);    
    delay(4000);
 
      }
         if(tempc < 40)
         {
             digitalWrite(relay1, HIGH);
         }
    if(humc > 90)
      {beep();
  Serial.write("AT+CIPSEND=0,23\r\n");delay(2000);
    Serial.write("High Hum:\r\n");
              converts(humc);//3
    Serial.write(0x1A);    
    delay(4000); 
      }
    if(digitalRead(sms) == LOW)
    {
      lcd.setCursor(5,1);
      lcd.print("wet");
      digitalWrite(relay2, HIGH);   
    }
    
  if(digitalRead(sms) == HIGH)
    {
      lcd.setCursor(5,1);
      lcd.print("Dry");
        digitalWrite(relay2, LOW);    
    }  


if(stringComplete)
     {
              if(inputString[2] == '1')
                {
                  digitalWrite(relay3,LOW);
    Serial.write("AT+CIPSEND=0,9\r\n");delay(2000);  
    Serial.write(" Load ON ");
   delay(3000);   
                }              
              if(inputString[2] == '2')
                {

                  digitalWrite(relay3,HIGH);
    Serial.write("AT+CIPSEND=0,10\r\n");delay(2000);  
    Serial.write(" Load Off ");
   delay(3000);
                } 
            
                 
        inputString = "";
        stringComplete = false;          
             delay(2500);
     }

    
 delay(100);
}





void serialEvent() 
{
  while (Serial.available()) 
        {
         
         char inChar = (char)Serial.read();
           //sti++;
           //inputString += inChar;
          if(inChar == '*')
            {sti=1;
              inputString += inChar;
             //  stringComplete = true;
             // gchr = inputString[sti-1] 
            }
          if(sti == 1)
            {
                inputString += inChar;
            }
          if(inChar == '#')
            {sti=0;
              stringComplete = true;      
            }
        }
}

int readSerial(char result[]) 
{
  int i = 0;
  while (1) 
  {
    while (Serial.available() > 0) 
    {
      char inChar = Serial.read();
      if (inChar == '\n') 
         {
          return 0;
         }
      if (inChar != '\r') 
         {
          result[i] = inChar;
          i++;
         }
    }
  }
}


int readSerial1(char result[]) 
{
  int i = 0;
  while (1) 
  {
    while (Serial.available() > 0) 
    {
      char inChar = Serial.read();
      if (inChar == '*') 
         {
          return 0;
         }
      if (inChar != '*') 
         {
          result[i] = inChar;
          i++;
         }
    }
  }
}




void gpsEvent()
{
  gpsString="";
  while(1)
  {
   //while (gps.available()>0)            //Serial incoming data from GPS

   // while (mySerial.available() > 0)
   {
    //char inChar = (char)gps.read();
//    char inChar = (char)mySerial.read();
//     gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
     i++;
    // Serial.print(inChar);
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //check for right string
      {
        i=0;
        gpsString="";
      }
     }
//    if(inChar=='\r')
    {
     if(i>60)
     {
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}

void get_gps()
{
  
  lcd.clear();
  lcd.print("Getting GPS Data");
  lcd.setCursor(0,1);
  lcd.print("Please Wait.....");
 
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    int str_lenth=i;
    coordinate2dec();
    i=0;x=0;
    str_lenth=0;
   }
}

void coordinate2dec()
{
  String lat_degree="";
    for(i=17;i<=18;i++)         
      lat_degree+=gpsString[i];
      
  String lat_minut="";
     for(i=18;i<=19;i++)         
      lat_minut+=gpsString[i];
     for(i=21;i<=22;i++)         
      lat_minut+=gpsString[i];


       
  String log_degree="";
    for(i=29;i<=31;i++)
      log_degree+=gpsString[i];
  String log_minut="";
    for(i=32;i<=33;i++)
      log_minut+=gpsString[i];
    for(i=35;i<=36;i++)
      log_minut+=gpsString[i];

      
    Speed="";
    for(i=42;i<45;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
}

/*
void coordinate2dec()
{
  String lat_degree="";
    for(i=19;i<=20;i++)         
      lat_degree+=gpsString[i];
      
  String lat_minut="";
     for(i=21;i<=22;i++)         
      lat_minut+=gpsString[i];
     for(i=24;i<=25;i++)         
      lat_minut+=gpsString[i];


       
  String log_degree="";
    for(i=32;i<=34;i++)
      log_degree+=gpsString[i];
  String log_minut="";
    for(i=35;i<=36;i++)
      log_minut+=gpsString[i];
    for(i=38;i<=39;i++)
      log_minut+=gpsString[i];

      
    Speed="";
    for(i=45;i<48;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
}
*/
void gps_convert()
{
  if(gps_status)
  {
  Serial.println(gpsString);

  if(gpsString[0] == '$' && gpsString[1] == 'G' && gpsString[2] == 'P' && gpsString[3] == 'R' && gpsString[4] == 'M' && gpsString[5] == 'C')
    {
     // Serial.println("Don11111111111111111111111111111111111111111111111111111\r\n");      
    //  Serial.write(gpsString[18]);Serial.write(gpsString[19]);Serial.write(gpsString[20]);Serial.write(gpsString[21]);Serial.write(gpsString[22]);
     //lcd.setCursor(0,0);
     for(ii=0;ii<9;ii++)
       {
        //lcd.write(gpsString[19+ii]);    
        msg1[ii] = gpsString[19+ii];
        //Serial.write(msg1[ii]);
       }
       //Serial.println("\r\n");
     //lcd.setCursor(0,1);
      for(ii=0;ii<10;ii++)
       {
        //lcd.write(gpsString[32+ii]);
        msg2[ii] = gpsString[32+ii];    
       // Serial.write(msg2[ii]);
       }

// Serial.println(msg1);
// Serial.println(msg2);

              convlat(lati); convlong(longi);
        finallat[0] = msg1[0];
        finallat[1] = msg1[1];
        finallat[2] = '.';
        finallat[3] = flat[0]; finallat[4] = flat[1];finallat[5] = flat[2];finallat[6] = flat[3];finallat[7] = '\0';


        finallong[0] = msg2[0];
        finallong[1] = msg2[1];
        finallong[2] = msg2[2];
        finallong[3] = '.';
        finallong[4] = flong[0];finallong[5] = flong[1];finallong[6] = flong[2];finallong[7] = flong[3];finallong[8] = '\0'; 

   
    }
  }  
}

 void convlat(unsigned int value)  
      {
             unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;              
      c=c|0x30;
      e=e|0x30; 
    g=g|0x30;              
      h=h|0x30;
   
  // dlcd(a);
//   dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    
         
               flat[0] = c;
               flat[1] = e;
               flat[2] = g;
             flat[3] = h;


          }

 void convlong(unsigned int value)  
      {
            unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;              
      c=c|0x30;
      e=e|0x30; 
    g=g|0x30;              
      h=h|0x30;
   
  // dlcd(a);
//   dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    
         
               flong[0] = c;
               flong[1] = e;
               flong[2] = g;
             flong[3] = h;


          }


/*
void coordinate2dec()
{
  String lat_degree="";
    for(i=20;i<=21;i++)         
      lat_degree+=gpsString[i];
      
  String lat_minut="";
     for(i=22;i<=28;i++)         
      lat_minut+=gpsString[i];
  String log_degree="";
    for(i=32;i<=34;i++)
      log_degree+=gpsString[i];
  String log_minut="";
    for(i=35;i<=41;i++)
      log_minut+=gpsString[i];
    
    Speed="";
    for(i=45;i<48;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
}*/

void gsminit()
{
  Serial.write("AT\r\n");                   okcheck();
  Serial.write("ATE0\r\n");                 okcheck();
  Serial.write("AT+CMGF=1\r\n");            okcheck();
  Serial.write("AT+CNMI=1,2,0,0\r\n");      okcheck();
  Serial.write("AT+CSMP=17,167,0,0\r\n");   okcheck();
    
  lcd.clear();
  lcd.print("SEND MSG STORE");
  lcd.setCursor(0,1);
  lcd.print("MOBILE NUMBER");  
  do{
     rcv = Serial.read();
    }while(rcv != '*');
     readSerial(pastnumber1);
        pastnumber1[10]='\0';
        
     /*
  pastnumber1[0] = pastnumber[0];pastnumber1[1] = pastnumber[1];pastnumber1[2] = pastnumber[2];pastnumber1[3] = pastnumber[3];pastnumber1[4] = pastnumber[4];pastnumber1[5] = pastnumber[5];
  pastnumber1[6] = pastnumber[6];pastnumber1[7] = pastnumber[7];pastnumber1[8] = pastnumber[8];pastnumber1[9] = pastnumber[9];pastnumber1[10] = '\0';

  pastnumber2[0] = pastnumber[10];pastnumber2[1] = pastnumber[11];pastnumber2[2] = pastnumber[12];pastnumber2[3] = pastnumber[13];pastnumber2[4] = pastnumber[14];pastnumber2[5] = pastnumber[15];
  pastnumber2[6] = pastnumber[16];pastnumber2[7] = pastnumber[17];pastnumber2[8] = pastnumber[18];pastnumber2[9] = pastnumber[19];pastnumber2[10] = '\0';

  pastnumber3[0] = pastnumber[20];pastnumber3[1] = pastnumber[21];pastnumber3[2] = pastnumber[22];pastnumber3[3] = pastnumber[23];pastnumber3[4] = pastnumber[24];pastnumber3[5] = pastnumber[25];
  pastnumber3[6] = pastnumber[26];pastnumber3[7] = pastnumber[27];pastnumber3[8] = pastnumber[28];pastnumber3[9] = pastnumber[29];pastnumber3[10] = '\0';
*/
  delay(3000);
  lcd.clear();
  lcd.print(pastnumber1);
  delay(500);
    lcd.clear();
  /*
  lcd.setCursor(0,1);
  lcd.print(pastnumber2);
  delay(2000);
  lcd.clear();
  lcd.print(pastnumber3);
  */
  
    Serial.write("AT+CMGS=\"");
    Serial.write(pastnumber1);
    Serial.write("\"\r\n"); delay(2500);
    Serial.write("Reg-1\r\n");
    Serial.write(0x1A);    
    delay(4000);  delay(4000);  
}

void converts(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   Serial.write(a);
   Serial.write(c);
   Serial.write(e); 
   Serial.write(g);
   Serial.write(h);
}


void convertl(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   //lcd.write(a);
   //lcd.write(c);
   lcd.write(e); 
   lcd.write(g);
   lcd.write(h);
}

void convertk(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
  // lcd.write(a);
  // lcd.write(c);
  // lcd.write(e); 
  // lcd.write(g);
   lcd.write(h);
}
/*
   sensorValue = analogRead(analogInPin);
   sensorValue = (sensorValue/9.31);
   lcd.setCursor(1,1); //rc
   lcd.print(sensorValue);
   Serial.print(sensorValue);
 */
