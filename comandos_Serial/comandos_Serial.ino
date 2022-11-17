int PORTAS[] = {16, 5, 4, 0, 2, 14, 12, 13, 15, 3, 1};// 11 portas
String palavra;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    palavra = Serial.readStringUntil(' ');
    Serial.println(palavra);
    int num = palavra.toInt();
    Serial.println(num);
  }
}
