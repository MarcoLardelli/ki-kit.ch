void setup() {

   //###BLINKCODE START###
   pinMode(13, OUTPUT);
   for(int blink=0;blink<1; blink++) {
      digitalWrite(13, HIGH);
      delay(30);
      digitalWrite(13, LOW);
      delay(300);
   }
   delay(200);
   for(int blink=0;blink<1; blink++) {
      digitalWrite(13, HIGH);
      delay(30);
      digitalWrite(13, LOW);
      delay(150);
   }
   digitalWrite(13, LOW);
   //###BLINKCODE ENDE###

}


void loop() {
   analogWrite(9, 255 - ((100 - (analogRead(14) / 10.23)) * 2.55));
   analogWrite(10, 255 - ((100 - (analogRead(15) / 10.23)) * 2.55));
   analogWrite(3, 255 - ((100 - (analogRead(16) / 10.23)) * 2.55));
   analogWrite(11, 255 - ((100 - (analogRead(17) / 10.23)) * 2.55));
}
