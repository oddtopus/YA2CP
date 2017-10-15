// USE IN CONJUNCTION WITH ctrl-pan_SD2.py
 
byte incomingByte = 0;   // for incoming serial data
byte arg=0; // byte for arguments, if any
byte ana[12]; // array of bytes for sending analog port data
int A=8; // pin for OUTPUT
int B=7; // pin for OUTPUT
int C=6; // pin for PWM

void setup() {
        Serial.begin(9600);
        pinMode(A, OUTPUT);
        pinMode(B, OUTPUT);
        pinMode(C, OUTPUT);
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
    pinMode(10, OUTPUT); // for SD library to work properly
    digitalWrite(10,LOW);
        digitalWrite(A,LOW);
        digitalWrite(B,LOW);
        digitalWrite(C,LOW);
}

void setAB(byte inByte) {
  if (isDigit(inByte)){
                inByte&=B001111;
                Serial.println("Setting pins A and B...");
                if (inByte&1==1){
                  digitalWrite(A,HIGH);
                  Serial.println("pin A is HIGH");}
                else{
                  digitalWrite(A,LOW);
                  Serial.println("pin A is LOW");}
                if (((inByte&2)>>1)==1){
                  digitalWrite(B,HIGH);
                  Serial.println("pin B is HIGH");}
                else{
                  digitalWrite(B,LOW);
                  Serial.println("pin B is LOW");}
  } else {
    Serial.print("Invalid argument");
  }
}
void setC(byte pwmval) {
  Serial.println("Setting pin C...");
  analogWrite(C,pwmval);
  Serial.print("Pin C set at ");
  Serial.println(pwmval,DEC);
}
void readA0() {
                 Serial.print(millis());
                 Serial.print("\t");
                 Serial.print("msec -> VALUE (10 bits)=\t");
                 Serial.println(analogRead(0));
}

void leggi_tutti() {
  //inizializzo la variabile per la selezione del
  //primo canale analogico
  int in_ana = 0;
  //eseguo un ciclo for per acquisire ciclicamente tutti i
  //canali analogici da A0 a A5
  for(int Ciclo = 0; Ciclo <12; Ciclo += 2) {
    //leggo il canale analogico
    int valAn = analogRead(in_ana);
    //il valore intero viene diviso in due byte e caricato
    //nell'array dichiarato precedentemente
    ana[Ciclo] = highByte(valAn);
    ana[Ciclo + 1] = lowByte(valAn);
    //piccolo delay tra una acquisizione e l'altra
    delay(1);
    //incremento il canale di acquisizione
    in_ana++;
  }
  //dopo 6 cicli spedisco tutti e 12 i byte al pc
  Serial.write(ana,12);
}

void loop() {
  // process data only when receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, BIN);
    delay(10);
    switch(incomingByte) {
      case 'A': // switch pins A and B:
        arg = Serial.read(); //2-bit pattern to set
        Serial.print("Argument (pattern): ");
        Serial.println(arg, BIN);
        setAB(arg);
        break;
      case 'B':
        readA0();
        break;
      case 'C':
        arg = Serial.read(); //PWM value to set
        Serial.print("Argument (PWM): ");
        Serial.println(arg, DEC);
        setC(arg);
        break;
      case 'D':
        Serial.print("PORTD: ");
        Serial.println(PORTD,BIN);
        Serial.print("DDRD: ");
        Serial.println(DDRD, BIN);
        Serial.print("PIND: ");
        Serial.println(PIND, BIN);
        break;
      case 'F':
        leggi_tutti();
        Serial.println();
        Serial.println("Array (HI-LO-HI-LO..) sent.");
        break;
      default:
        Serial.println("Input is not a valid command");
    }
    // if finished, say goodbye
    if (Serial.available()==0) {
      Serial.println("Input serial buffer clear!");}
  }
  arg=0;
}
