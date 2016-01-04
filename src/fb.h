#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <stdint.h>
/*
 * Explication des adresses, offsets, channels, etc: http://elinux.org/RPi_Framebuffer
 * Intro au framebuffer: http://magicsmoke.co.za/?p=284
 */


/*
 * Mailbox functions
 */

enum {
    MAILBOX_BASE            = 0x2000B880,
    MAILBOX_POLL            = 0x2000B890,    // Receive without retrieving.	 R
    MAILBOX_SENDER          = 0x2000B894,    // Sender information.	 R
    MAILBOX_STATUS          = 0x2000B898,    // Information.	 R
    MAILBOX_CONFIGURATION   = 0x2000B89C,    // Settings.	 RW
    MAILBOX_WRITE           = 0x2000B8A0     // Sending mail.	 W
};

void MailboxWrite(uint32_t message, uint32_t mailbox);
uint32_t MailboxRead(uint32_t mailbox);

#define data_mem_barrier() __asm__ __volatile__ ("mcr p15, 0, %[reg], c7, c10, 5"::[reg] "r" (0))
#define data_sync_barrier() __asm__ __volatile__ ("mcr p15, 0, %[reg], c7, c10, 4"::[reg] "r" (0))


// fonction pour écrire un message data dans une mailbox suivant un des mode de l'enum si dessus 
static inline void mmio_write(uint32_t reg, uint32_t data) {
    uint32_t *ptr = (uint32_t*)reg;
    __asm__ volatile("str %[data], [%[reg]]"
                 : : [reg]"r"(ptr), [data]"r"(data));
}


// fonction pour lire un message data dans une mailbox suivant un des mode de l'enum si dessus
static inline uint32_t mmio_read(uint32_t reg) {
    uint32_t *ptr = (uint32_t*)reg;
    uint32_t data;
    __asm__ volatile("ldr %[data], [%[reg]]"
                 : [data]"=r"(data) : [reg]"r"(ptr));
    return data;
}



/*
 * Framebuffer functions
 */

void setColor(uint8_t redC_t, uint8_t greenC_t, uint8_t blueC_t);

int FramebufferInitialize();

void draw();

void drawRed();

void drawBlue();

void drawString(char * string, int length);

void drawChar(char c);

void drawVerticalLine(int size);

void drawHorizontalLine(int size);

void newLine();

void drawHelloConsole();

void drawError(char * string, int length);

void prompt();

void clear();

void put_pixel_RGB24(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue);

void increaseOffsetY();

#endif
