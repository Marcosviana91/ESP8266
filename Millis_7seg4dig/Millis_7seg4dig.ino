#include <neotimer.h>

#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3 //RX
#define D10 1 //TX
//#define LED_BUILTIN = 2;
//A0 porta analógica
//LED_BUILTIN;

class Digito {
    public: //declara as variaveis e métodos publicos
        int Porta;
        int duracao;
        void ExibirNum(int num);
    private:
        int NumAtual;
        unsigned long int tempoInicial;
};

void Digito::ExibirNum(int num) {
    this->NumAtual = num;
}

Digito *Dig1 = new Digito();
Digito *Dig2 = new Digito();
//Digito *Dig3 = new Digito();
//Digito *Dig4 = new Digito();

void setup() {
    Dig1->Porta = D0;
    Dig2->Porta = D1;
    //Dig3->Porta = D2;
    //Dig4->Porta = D3;
    pinMode(Dig1->Porta, OUTPUT);
    pinMode(Dig2->Porta, OUTPUT);
    digitalWrite(Dig1->Porta,HIGH);
    digitalWrite(Dig2->Porta,HIGH);
}

void loop() {
    
}