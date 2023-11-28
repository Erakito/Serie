/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Liaison série.
 *
 *      This file contains the full implementation of the motor control,
 *      except the PID-controller.
 *
 * \par Application note:
 *      AVR447: Sinusoidal driving of three-phase permanent motor using
 *      ATmega48/88/168
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Name: RELEASE_1_0 $
 * $Revision: 1.8 $
 * $RCSfile: main.c,v $
 * $Date: 2006/03/27 07:20:51 $  \n
 ******************************************************************************/



//! define des byte de vérification des erreurs
#define FRAMING_ERROR (1<<FE0)
#define PARITY_ERROR (1<<UPE0)
#define DATA_OVERRUN (1<<DOR0)
#define FOSC 16000000 //! Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
int flag = 0;
 char receivByte;
int main(void)
{
  //! Initialisation de la liaison série
  USART_Init(MYUBRR);
  sei();
  while (1){
    if (flag==1){
        USART_Transmit(receivByte);
        flag = 0;
    }
    _delay_ms(1);
  }
}

ISR(USART_RX_vect){
  //! a chaque fois que le lis un mots
  char status=UCSR0A; //! lire le registre
  receivByte = UDR0;//! lire UDR0
  if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0){ //! test si une erreur apparaît
    flag = 1;
  }
  else{
    
    USART_putsln(" erreur "); //! envoyer l'erreur
  }
}

void USART_Transmit(unsigned char data)
{
/*! Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;
/*! Put data into buffer, sends the data */
UDR0 = data;
}

unsigned char USART_Receive(void)
{
  while (!(UCSR0A & (1<<RXC0)));  //! attendre de recevoir une information 

  return UDR0; //! renvoyer le caracter
}


void USART_putsln(unsigned char *str){
  USART_puts(str); //! écris la chaine de caracter à l'aide de la fonction
  USART_Transmit(10); //! fais un line feed
  USART_Transmit(13); //! retourne au début de la ligne
}

void USART_puts(unsigned char *str)
{
do
{
USART_Transmit(*str); //! transmit l'adresse du str
} while (*++str!=0); //! incrément l'adresse jusqu' à la fin de la chaîne
}

void USART_Init(unsigned int ubrr)
{
/*!Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
//!Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
/*! Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}