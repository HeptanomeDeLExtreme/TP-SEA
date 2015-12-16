#include <stdint.h>
#include "fb.h"

#include "font8x8_basic.h"

#define jumpLine 10
#define breakspace 8
#define screenWidth 1024
#define screenHeight 768


/*
 * Framebuffer variable
 */
uint8_t redC = 0xFF;
uint8_t greenC = 0xFF;
uint8_t blueC = 0xFF;

int offsetX = 0;
int offsetY = 0;

/*
 * Adresse du framebuffer, taille en byte, résolution de l'écran, pitch et depth (couleurs)
 */
static uint32_t fb_address;
static uint32_t fb_size_bytes;
static uint32_t fb_x,fb_y,pitch,depth;

/*
 * Fonction pour lire et écrire dans les mailboxs
 */

/*
 * Fonction permettant d'écrire un message dans un canal de mailbox
 */
void MailboxWrite(uint32_t message, uint32_t mailbox)
{
  uint32_t status;
  
  if(mailbox > 10)            // Il y a que 10 mailbox (0-9) sur raspberry pi
    return;
  
  // On attend que la mailbox soit disponible i.e. pas pleine
  do{
    /*
     * Permet de flusher
     */
    data_mem_barrier();
    status = mmio_read(MAILBOX_STATUS);
    
    /*
     * Permet de flusher
     */
    data_mem_barrier();
  }while(status & 0x80000000);             // Vérification si la mailbox est pleinne
  
  data_mem_barrier();
  mmio_write(MAILBOX_WRITE, message | mailbox);   // Combine le message à envoyer et le numéro du canal de la mailbox puis écrit en mémoire la combinaison
}


/*
 * Fonction permettant de lire un message et le retourner depuis un canal de mailbox
 */
uint32_t MailboxRead(uint32_t mailbox)
{
  uint32_t status;
  
  if(mailbox > 10)             // Il y a que 10 mailbox (0-9) sur raspberry pi
    return 0;
  
  while(1){
    // On attend que la mailbox soit disponible pour la lecture, i.e. qu'elle n'est pas vide
    do{
      data_mem_barrier();
      status = mmio_read(MAILBOX_STATUS);
      data_mem_barrier();
    }while(status & 0x40000000);             // On vérifie que la mailbox n'est pas vide
    
    data_mem_barrier();
    status = mmio_read(MAILBOX_BASE);
    data_mem_barrier();
    
    // On conserve uniquement les données et on les retourne
    if(mailbox == (status & 0x0000000f))
      return status & 0x0000000f;
  }
}

/*
 * Fonction pour initialiser et écrire dans le framebuffer
 */

int FramebufferInitialize() {
  
  uint32_t retval=0;
  volatile unsigned int mb[100] __attribute__ ((aligned(16)));
  
  depth = 24;
  
  //
  // Tout d'abord, on veut récupérer l'adresse en mémoire du framebuffer
  //
  mb[0] = 8 * 4;		// Taille du buffer i.e. de notre message à envoyer dans la mailbox
  mb[1] = 0;			// On spécifie qu'on demande quelque chose
  mb[2] = 0x00040003;	// La question que l'on pose: https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
  mb[3] = 2*4;		// La taille de la réponse
  mb[4] = 0;			// On indique que c'est une question ou un réponse (0 question)
  mb[5] = 0;			// Largeur
  mb[6] = 0;			// Hauteur
  mb[7] = 0;			// Marqueur de fin
  
  MailboxWrite((uint32_t)(mb+0x40000000), 8); // On écrit le message dans la mailbox
  
  if(((retval = MailboxRead(8)) == 0) || (mb[1] != 0x80000000)){ // On vérifie que le message est passé
    return 0;
  }
  
  fb_x = mb[5]; // On récupére la largeur en pixel de l'écran
  fb_y = mb[6]; // On récupére la hauteur en pixel de l'écran
  
  uint32_t mb_pos=1;
  
  mb[mb_pos++] = 0;			// C'est une requête
  mb[mb_pos++] = 0x00048003;	// On définit la hauteur et la largeur du framebuffer
  mb[mb_pos++] = 2*4;			// On envoi 2 int pour la taille donc on spécifie la taille du buffer
  mb[mb_pos++] = 2*4;			// Taille du message (tag + indicateur de requête)
  mb[mb_pos++] = fb_x;		// On passe la largeur
  mb[mb_pos++] = fb_y;		// On passe la hauteur
  
  mb[mb_pos++] = 0x00048004;	// On définit la hauteur et la largeur virtuel du framebuffer
  mb[mb_pos++] = 2*4;			// On envoi 2 int pour la taille donc on spécifie la taille du buffer
  mb[mb_pos++] = 2*4;			// Taille du message (tag + indicateur de requête)
  mb[mb_pos++] = fb_x;		// On passe la largeur
  mb[mb_pos++] = fb_y;		// On passe la hauteur
  
  mb[mb_pos++] = 0x00048005;	// On définit la profondeur du frame buffer
  mb[mb_pos++] = 1*4;			
  mb[mb_pos++] = 1*4;			
  mb[mb_pos++] = depth;		// Profondeur i.e. nombre de couleur (24bit dans notre cas)
  
  mb[mb_pos++] = 0x00040001;	// On demande l'allocation du buffer
  mb[mb_pos++] = 2*4;			
  mb[mb_pos++] = 2*4;			
  mb[mb_pos++] = 16;			
  mb[mb_pos++] = 0;			

  mb[mb_pos++] = 0;			// Tag de fin de message
  mb[0] = mb_pos*4;			// Taille du message dans son entier
  
  MailboxWrite((uint32_t)(mb+0x40000000), 8); // On écrit dans la mailbox
  
  if(((retval = MailboxRead(8)) == 0) || (mb[1] != 0x80000000)){ // On vérifie que le message a bien été passé
    return 0;
  }
  
  /*
   * On récupére les différente information récupérer de la requête pour pouvoir reconstruire l'adresse du framebuffer et sa taille
   */
  mb_pos = 2;
  unsigned int val_buff_len=0;
  while(mb[mb_pos] != 0){
    switch(mb[mb_pos++])
    {
      case 0x00048003:
	val_buff_len = mb[mb_pos++];
	mb_pos+= (val_buff_len/4)+1;
	break;
      case 0x00048004:
	val_buff_len = mb[mb_pos++];
	mb_pos+= (val_buff_len/4)+1;
	break;
      case 0x00048005:
	val_buff_len = mb[mb_pos++];
	mb_pos+= (val_buff_len/4)+1;
	break;
      case 0x00040001:
	val_buff_len = mb[mb_pos++];
	mb_pos++;
	fb_address = mb[mb_pos++];
	fb_size_bytes = mb[mb_pos++];
	break;
    }
  }
  
  //
  // Récupére le pitch (This indicates the number of bytes between rows. Usually it will be related to the width, but there are exceptions such as when drawing only part of an image.)
  //
  mb[0] = 8 * 4;		// Taille du buffer
  mb[1] = 0;			// C'est une requête
  mb[2] = 0x00040008;	// On veut récupérer le pitch
  mb[3] = 4;			// Taille du buffer
  mb[4] = 0;			// Taille de la demande
  mb[5] = 0;			// Le pitch sera stocké ici
  mb[6] = 0;			// Tag de fin de message
  mb[7] = 0;
  
  MailboxWrite((uint32_t)(mb+0x40000000), 8);
  
  if(((retval = MailboxRead(8)) == 0) || (mb[1] != 0x80000000)){
    return 0;
  }
  
  pitch = mb[5];
  
  fb_x--;
  fb_y--;
  
  return 1;
}

/*
 * Fonction permettant de dessiner un pixel à l'adresse x,y avec la couleur rgb red.green.blue
 */
void put_pixel_RGB24(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	volatile uint32_t *ptr=0;
	uint32_t offset=0;

	offset = (y * pitch) + (x * 3);
	ptr = (uint32_t*)(fb_address + offset);
	*((uint8_t*)ptr) = red;
	*((uint8_t*)(ptr+1)) = green;
	*((uint8_t*)(ptr+2)) = blue;
}

/*
 * Dessine sur tous les pixels des couleurs différentes
 */
void draw() {
  uint8_t red=0,green=0,blue=0;
  uint32_t x=0, y=0;
  for (x = 0; x < fb_x; x++) {
    for (y = 0; y < fb_y; y++) {
      if (blue > 254) {
	if (green > 254) {
	  if (red > 254) {
	    red=0; green=0; blue=0;
	  } else {
	    red++;
	  }
	} else {
	  green++;
	}
      } else blue++;
      put_pixel_RGB24(x,y,red,green,blue);
    }
  }
}

/*
 * Rempli l'écran de rouge
 */
void drawRed() {
  uint32_t x=0, y=0;
  for (x = 0; x < fb_x; x++) {
    for (y = 0; y < fb_y; y++) {
      put_pixel_RGB24(x,y,255,0,0);
    }
  }
}

/*
 * Rempli l'écran de bleu
 */
void drawBlue() {
  uint32_t x=0, y=0;
  for (x = 0; x < fb_x; x++) {
    for (y = 0; y < fb_y; y++) {
      put_pixel_RGB24(x,y,0,0,255);
    }
  }
}

void render(char * bitmap){
	int i,j;
    int set;
    for (i=0; i < 8; i++) {
        for (j=0; j < 8; j++) {
            set = bitmap[i] & 1 << j;
            if(set)
            {
				int x = i+offsetX;
				int y = j+offsetY;
				put_pixel_RGB24(y,x,redC,greenC,blueC);
			}
        }
    }
    offsetY += breakspace;
}

void newLine()
{
	offsetX += jumpLine;
	offsetY = 0;
}

void drawChar(char c){
	char *bitmap = font8x8_basic[(int)c];
    render(bitmap);
}

void drawString(char * string, int length){
	for(int i = 0;i<length;i++)
	{
		drawChar(string[i]);
	}
	newLine();
}

void drawStringWithoutNewLine(char * string, int length){
	for(int i = 0;i<length;i++)
	{
		drawChar(string[i]);
	}
}

void drawVerticalLine(int size)
{
	for(int i = 0;i<size;i++)
	{
		put_pixel_RGB24(10,i,redC,greenC,blueC);
	}
}

void drawHorizontalLine(int size)
{
	for(int i = 0;i<size;i++)
	{
		put_pixel_RGB24(i,offsetX,redC,greenC,blueC);
	}
	newLine();
}

void prompt()
{
	drawStringWithoutNewLine("> ",2);
}

void drawHelloConsole()
{
	drawHorizontalLine(screenWidth);
	drawString("Tac'OS", 6);
	drawString("Badadadadam", 11);
	drawString("Alors mes p'tits cocos !", 24);
	drawHorizontalLine(screenWidth);
	prompt();
}

void clear()
{
	for(int i = 0;i<screenHeight;i++)
	{
		for(int j = 0;j<screenWidth;j++)
		{
			put_pixel_RGB24(j,i,0x00,0x00,0x00);
		}
	}
	offsetX = 0;
	offsetY = 0;
}

void setColor(uint8_t redC_t, uint8_t greenC_t, uint8_t blueC_t)
{
	redC = redC_t;
	greenC = greenC_t;
	blueC = blueC_t;
}

void drawError(char * string, int length)
{
	uint8_t oldRed = redC;
	uint8_t oldGreen = greenC;
	uint8_t oldBlue = blueC;
	setColor(0xFF,0x0,0x0);
	drawString(string,length);
	setColor(oldRed,oldGreen,oldBlue);
	setColor(oldRed,oldGreen,oldBlue);
}
