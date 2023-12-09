/**@file liaisonSerieAppareil.ino */
/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 * \mainpage
 * \brief
 *      Liaison série.
 *
 *      Ce code permet d'émettre et recevoir des information par les liaison
 *      série 0 et 3 de l'arduino mega afin de contrôler un appareil de mesure
 *
 * \par Application note:
 *      
 *      Utilisation de ce code sur un générateur basse fréquence GF266
 *
 * \par Documentation
 *      
 *      Documentation en Doxygene
 *
 * \author
 *      Thomas Meunier \n
 *      IUT de Troyes
 *
 * $Name: liaisonSerieAvecAppareil Version PrintSr $
 * $Revision: 1.O $
 * $RCSfile: main.c,v $
 * $Date: 09/12/23 $  \n
 ******************************************************************************/
 /*!\brief
Horloge du processeur de la carte 
*/
#define FOSC 16000000 // Clock Speed
/*!\brief
Vitesse de la transmission
*/
#define BAUD 9600
/*!\brief
Prescaler 
*/
#define MYUBRR FOSC/16/BAUD-1
/*!\brief
Drapeau de l'interruption 3
*/
int flag3 = 0;
/*!\brief
Drapeau de l'interruption 0 
*/
int flag0 = 0;


/*!\brief
La fonction main initialise les deux communication série, autorise les interruption
et va boucler lorsque que l'arduino va émettre ou recevoir des information sur la liaison série.
*/
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
        USART_putsSr(UDR0); //transimission de la valeur Rx3 sur Tx0 
        flag0 = 0;
    }
    _delay_ms(1);
  }
}


/*!\brief
Interruption lié au receive des deux liaison série 3
*/
ISR(USART3_RX_vect){
  flag3 = 1;
}
/*!\brief
Interruption lié au receive des deux liaison série 0
*/
ISR(USART0_RX_vect){
  flag0 = 1;
}

/*!\brief
La fonction permet de transmettre sur la liaisons série 0
*/
void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;
/* Put data into buffer, sends the data */
UDR0 = data;
}

/*!\brief
La fonction USART_Transmit permet de transmettre sur la liaisons série 3
*/
void USART_Transmit3(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR3A & (1<<UDRE3)))
;
/* Put data into buffer, sends the data */
UDR3 = data;
}


/*!\brief
La fonction permet de recevoir sur la liaisons série 0
*/
unsigned char USART_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)))
;
/* Get and return received data from buffer */
return UDR0;
}

/*!\brief
La fonction permet de recevoir sur la liaisons série 3
*/
unsigned char USART_Receive3(void)
{
/* Wait for data to be received */
while (!(UCSR3A & (1<<RXC3)))
;
/* Get and return received data from buffer */
return UDR3;
}

/*!\brief
La fonction permet de print des caractère avec "entré" en fin de chaîne sur la liaisonSerie 3
afin de programmer l'appareil en LiaisonSerie
*/
void USART_putsSr(unsigned char *str)
{
  USART_puts(str);
  USART_Transmit3(10);
}

/*!\brief
La fonction permet de print des caractère sur la liaisonSerie 3 
*/
void USART_puts(unsigned char *str) {
  do
  {
    USART_Transmit3(*str);
  } while (*++str!=0);
}


/*!\brief
La Fonction va initialiser les différentes liaison Serie dont nous avons besoin 
pour la communication 
*/
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