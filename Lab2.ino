
  volatile int count=0;   
  volatile int countEnabled=0;
   int code[10]={ 0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11110110 };
  int datapin=4, latchpin=5, clockpin=6;
  int datapin2=7, latchpin2=8, clockpin2=9; 

void setup() { 
  
  Serial.begin(9600);
  attachInterrupt(0, resetCounter, FALLING);
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 62500; // compare match register 16MHz/256/2Hz
  TCCR1B |= (WGM13); // CTC mode
  TCCR1B |= (CS10); // 256 prescaler
  TIMSK1 |= (1<<OCIE1A);
  sei();
  pinMode(latchpin,OUTPUT);
  pinMode(datapin, OUTPUT);
  pinMode(clockpin,OUTPUT);
  pinMode(latchpin2,OUTPUT);
  pinMode(datapin2, OUTPUT);
  pinMode(clockpin2,OUTPUT);
  pinMode(A0, INPUT);
  pinMode(10,OUTPUT);
}

ISR (TIMER1_COMPA_vect)
{
  if(countEnabled==1){
     count=count-1;
    displayNumber(count);
  if(count==0){
    for(int i=0;i<1000;i++){
       digitalWrite(10,HIGH);
       delay(10);
       digitalWrite(10,LOW);
       delay(100);
    }
     countEnabled=0;   
  } 
  }
}

void loop(){
  int reading = analogRead(0);
  Serial.println(reading);
  if(reading>500){
      resetCounter();
  }
  
  while(count>00){
  }
   
}


void displayNumber(int number){
  
  int digitOnes=number%10;
  int digitTens=number/10;
  Serial.println(digitTens);
  digitalWrite(latchpin, LOW);
  digitalWrite(latchpin2,LOW);		//enable 74595 to receive data
	//send 1 bit at a time, starting at the low bit
  shiftOut(datapin, clockpin, LSBFIRST, code[digitOnes]);	
  shiftOut(datapin2,clockpin2,LSBFIRST,code[digitTens]);
  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin2,HIGH);	
  pinMode(datapin,HIGH);
  pinMode(datapin2,HIGH);
}

void resetCounter() {
     count=60;
     countEnabled=1;
     displayNumber(60);
}




