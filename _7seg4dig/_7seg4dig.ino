#define DIG0 0
#define DIG1 1 //tx
#define DIG2 2
#define DIG3 3 //rx

#define SEGA 4
#define SEGB 5
#define SEGC 12
#define SEGD 13
#define SEGE 14
#define SEGF 15
#define SEGG 16
#define PONTO 10

#define FPS 50
//A0 porta analógica

void teste_seg(int digito){//acende os segmentos do digito.
    //apaga tudo
    digitalWrite(DIG0,HIGH);
    digitalWrite(DIG1,HIGH);
    digitalWrite(DIG2,HIGH);
    digitalWrite(DIG3,HIGH);
    digitalWrite(SEGA, LOW);
    digitalWrite(SEGB, LOW);
    digitalWrite(SEGC, LOW);
    digitalWrite(SEGD, LOW);
    digitalWrite(SEGE, LOW);
    digitalWrite(SEGF, LOW);
    digitalWrite(SEGG, LOW);
    digitalWrite(PONTO, LOW);
    //Inicio do teste
    digitalWrite(digito, LOW); //ativa o digito solicitado
    digitalWrite(SEGA, HIGH);
    delay(FPS);
    digitalWrite(SEGA, LOW);
    digitalWrite(SEGB, HIGH);
    delay(FPS);
    digitalWrite(SEGB, LOW);
    digitalWrite(SEGC, HIGH);
    delay(FPS);
    digitalWrite(SEGC, LOW);
    digitalWrite(SEGD, HIGH);
    delay(FPS);
    digitalWrite(SEGD, LOW);
    digitalWrite(SEGE, HIGH);
    delay(FPS);
    digitalWrite(SEGE, LOW);
    digitalWrite(SEGF, HIGH);
    delay(FPS);
    digitalWrite(SEGF, LOW);
    digitalWrite(SEGG, HIGH);
    delay(FPS);
    digitalWrite(SEGG, LOW);
    digitalWrite(PONTO, HIGH);
    delay(FPS);
    digitalWrite(PONTO, LOW);
}

void teste_dig(){ //testa os digitos
    teste_seg(DIG0);
    teste_seg(DIG1);
    teste_seg(DIG2);
    teste_seg(DIG3);
}

void exibir_numero(int num, int dig) {
    delay(FPS/10);
    digitalWrite(DIG0,HIGH);
    digitalWrite(DIG1,HIGH);
    digitalWrite(DIG2,HIGH);
    digitalWrite(DIG3,HIGH);
    digitalWrite(SEGA, LOW);
    digitalWrite(SEGB, LOW);
    digitalWrite(SEGC, LOW);
    digitalWrite(SEGD, LOW);
    digitalWrite(SEGE, LOW);
    digitalWrite(SEGF, LOW);
    digitalWrite(SEGG, LOW);
    digitalWrite(PONTO, LOW);
    
    digitalWrite(dig, LOW);
    
    if (num == 0) {
        digitalWrite(SEGA, HIGH);
        digitalWrite(SEGB, HIGH);
        digitalWrite(SEGC, HIGH);
        digitalWrite(SEGD, HIGH);
        digitalWrite(SEGE, HIGH);
        digitalWrite(SEGF, HIGH);
    }
    else if (num == 1) {
        digitalWrite(SEGB, HIGH);
        digitalWrite(SEGC, HIGH);
    }
    else if (num == 2) {
        digitalWrite(SEGA, HIGH);
        digitalWrite(SEGB, HIGH);
        digitalWrite(SEGD, HIGH);
        digitalWrite(SEGE, HIGH);
        digitalWrite(SEGG, HIGH);
    }
    else if (num == 3) {
        digitalWrite(SEGA, HIGH);
        digitalWrite(SEGB, HIGH);
        digitalWrite(SEGC, HIGH);
        digitalWrite(SEGD, HIGH);
        digitalWrite(SEGG, HIGH);
    }
    else if (num == 4) {
        digitalWrite(SEGB, HIGH);
        digitalWrite(SEGC, HIGH);
        digitalWrite(SEGF, HIGH);
        digitalWrite(SEGG, HIGH);
    }
    else if (num == 5) {
        digitalWrite(SEGA, HIGH);
        digitalWrite(SEGC, HIGH);
        digitalWrite(SEGD, HIGH);
        digitalWrite(SEGF, HIGH);
        digitalWrite(SEGG, HIGH);
    }
    else if (num == 6) {
        digitalWrite(SEGA, HIGH);
        digitalWrite(SEGC, HIGH);
        digitalWrite(SEGD, HIGH);
        digitalWrite(SEGE, HIGH);
        digitalWrite(SEGF, HIGH);
        digitalWrite(SEGG, HIGH);
    }
    else if (num == 7) {
        digitalWrite(SEGA, HIGH);
        digitalWrite(SEGB, HIGH);
        digitalWrite(SEGC, HIGH);
    }
    else if (num == 8) {
        digitalWrite(SEGA, HIGH);
        digitalWrite(SEGB, HIGH);
        digitalWrite(SEGC, HIGH);
        digitalWrite(SEGD, HIGH);
        digitalWrite(SEGE, HIGH);
        digitalWrite(SEGF, HIGH);
        digitalWrite(SEGG, HIGH);
    }
    else if (num == 9) {
        digitalWrite(SEGA, HIGH);
        digitalWrite(SEGB, HIGH);
        digitalWrite(SEGC, HIGH);
        digitalWrite(SEGD, HIGH);
        digitalWrite(SEGF, HIGH);
        digitalWrite(SEGG, HIGH);
    }
}

void teste_numero() {
    for(int num=0; num<11; num++){
        for(int dig=0; dig<4; dig++){
            exibir_numero(num, dig);
            delay(50);
        }
    }
}

void setup() {
    //Dígitos são ativados em LOW
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    //Segmentos são ativados em HIGH
    pinMode(4, OUTPUT); //A
    pinMode(5, OUTPUT); //B
    pinMode(12, OUTPUT); //C
    pinMode(13, OUTPUT); //D
    pinMode(14, OUTPUT); //E
    pinMode(15, OUTPUT); //F
    pinMode(16, OUTPUT); //G
    pinMode(PONTO, OUTPUT); //PONTO
}

void loop() {
    teste_dig();
    delay(500);
    teste_numero();
    delay(500);
}