#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
int flag3 = 0;
int flag0 = 0;
int main(void)
{
  USART_Init(MYUBRR);
  sei();
  while (1){
    if (flag3==1){
        USART_Transmit(UDR3); //transimission de la valeur Rx3 sur Tx0 
        flag3 = 0;
    }
    if (flag0==1){
        USART_Transmit3(UDR0); //transimission de la valeur Rx3 sur Tx0 
        flag0 = 0;
    }
    _delay_ms(1);
  }
}



ISR(USART3_RX_vect){
  flag3 = 1;
}
ISR(USART0_RX_vect){
  flag0 = 1;
}

void USART_puts(unsigned char *str)
{
do
{
USART_Transmit(*str);
} while (*++str!=0);
}

void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;
/* Put data into buffer, sends the data */
UDR0 = data;
}

void USART_Transmit3(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR3A & (1<<UDRE3)))
;
/* Put data into buffer, sends the data */
UDR3 = data;
}



unsigned char USART_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)))
;
/* Get and return received data from buffer */
return UDR0;
}


unsigned char USART_Receive3(void)
{
/* Wait for data to be received */
while (!(UCSR3A & (1<<RXC3)))
;
/* Get and return received data from buffer */
return UDR3;
}

void USART_Init(unsigned int ubrr)
{
/*Init du Registre USART0*/
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
//Init du registre 3
UBRR3H = (unsigned char)(ubrr>>8);
UBRR3L = (unsigned char)ubrr;
// activer transmettre et recevoir et l'interruption */
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
UCSR3B = (1<<RXEN3)|(1<<TXEN3)|(1<<RXCIE3);
/* 1 bit de stop  */
UCSR3C = (3<<UCSZ30);
UCSR0C = (3<<UCSZ00);
}