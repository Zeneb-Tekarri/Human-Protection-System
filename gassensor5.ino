  #include <SoftwareSerial.h> //bibliotheque du module GSM
#include <Servo.h> //cette bibliotheque  servo est nécessaire pour contrôler le servomoteur.     
Servo monServomoteur; // Créer un objet servo pour nous permettre de contrôler le servomoteur. 
SoftwareSerial gsm (9,10); // rx ,tx 
const unsigned long eventInterval = 6000; 
unsigned long previousTime = 0;//Type de données: unsigned long . c'est le nombre de millisecondes écoulées depuis le démarrage du programme. 
int  flag = 0 ;
void setup() { 
monServomoteur.attach(7);
pinMode(4,OUTPUT); // buzzer
pinMode(8,OUTPUT);// LED rouge
pinMode(11,OUTPUT);//LED vert
pinMode(12,OUTPUT);//LED jaune
pinMode(5,OUTPUT);//ventilateur
pinMode(A0,INPUT); // capteur de gaz
pinMode(A1,INPUT);// capteur de flame 
Serial.begin (9600); // définit la vitesse des transferts que notre programme va effectuer.
gsm.begin(9600); 
}
void loop() { 
if (analogRead(A0)<400 && analogRead(A1)<400){
 flag =0 ;
 Serial.print("no risk\r"); //affichage des messages
 digitalWrite(11,HIGH); // la led verte est allumée
 digitalWrite(8,LOW);  // la led rouge est éteinte
 digitalWrite(12,LOW);// led jaune est éteinte 
 digitalWrite(4,LOW);// buzzer désactivé
 digitalWrite(5,LOW);//ventilateur désactivé
 for (int position = 90; position >= -90; position--) {
    monServomoteur.write(position); }// Fait bouger le bras de 180° à 0°
delay(1000);}
while (analogRead(A0)>300  && analogRead(A1)<400 && flag == 0){
 flag = 1 ;
 digitalWrite(11,LOW);//la led verte est éteinte
  digitalWrite(8,LOW);//la led rouge est éteinte
 digitalWrite(12,HIGH);// la led jaune est allumée
 digitalWrite(8,LOW);//la led rouge est éteinte
 digitalWrite(4,HIGH); // buzzer actif
 digitalWrite(5,HIGH);//ventilateur actif
 for (int position = -180; position <= 180; position++) {
  monServomoteur.write(position);  }// Fait bouger le bras de 0° à 180°
  delay(100);
  Serial.println("AT");// affichage de commande AT
  gsm.println ("AT");
  delay(100);
  gsm.print("AT+CMGF = 1\r");                        //    Mode Texte
  delay (100);
  gsm.println("AT+CMGS = \"+21623171934\"");       // Numéro du consommateur
  delay (100);
  gsm.print("fuite de gaz detecte");// SMS Texte 
  delay(100);
  Serial.print((char) 26); 
  gsm.write((char) 26);                 // CTRL-Z
  delay (100);
  gsm.println();  
  Serial.println();  
  break ; }
  unsigned long currentTime = millis(); //La fonction millis renvoie le nombre de millisecondes pendant lesquelles notre carte Arduino a été mise sous tension.
  if ( currentTime - previousTime >= eventInterval&& analogRead(A0)>400 && analogRead(A1)<400 ) {
   previousTime = currentTime;   // Mettre à jour le timing pour la prochaine fois
   flag = 0 ;}
  else if ( currentTime - previousTime >= eventInterval && analogRead(A0)>400 && analogRead(A1)>400 )
   {previousTime = currentTime;// Mettre à jour le timing pour la prochaine fois
   flag =0; }
 while (analogRead(A1)>400 && analogRead(A0)>400 && flag == 0 ){
  flag = 1;
 digitalWrite(8,HIGH);//la led rouge est allumée 
 digitalWrite(12,HIGH);//la led jaune est allumée
 digitalWrite(11,LOW);//la led verte est éteinte
 digitalWrite(4,HIGH);// buzzer actif
 digitalWrite(5,LOW);//ventilatteur actif
  for (int position = -180; position <= 180; position++) { 
  monServomoteur.write(position);}   // Fait bouger le bras de 0° à 180° 
  Serial.println("AT");
  gsm.println ("AT");
  delay(100); 
  gsm.println("AT+CMGF = 1");                         //  Mode Texte 
  delay (100);
  gsm.println("AT+CMGS = \"+21650863290\"");     // le numero du protection civile 
  delay (100);
  gsm.println (" il y a un feu danger dans la rue de kalaa soghra"); // text SMS
  Serial.print((char) 26);
  gsm.print((char) 26);                 // CTRL-Z
  delay (100);
  gsm.println();
  Serial.println();
  delay(100);
  break ;}}
