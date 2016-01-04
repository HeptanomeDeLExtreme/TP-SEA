#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "pwm.h"
#include "asm_tools.h"
#include "fb.h"

char * snoopIMG = " \
+======~~~~~::,,,.........MMD?,~D,.,NMMMMMMMMMMMMMMMMM..........,,,,,:::::~~~~~= \
=====~~~~~:::,,,........:M.M=.MMM.M.MMZMMMMMMMMMMMM?NOZ. .........,,,,,:::::~~~~ \
===~~~~~::::,,,........MMM.N=MM+..M.MM:MMM+,.I.,MM:M+N+M............,,,,:::::~~~\
=~~~~~::::,,,,...... .8MM:M~.MMMI..,=M.NMM.8.NM,M:7M.M~7?.............,,,,::::~~\
~~~:::::,,,,.........NMM7:.M7M.=MMMMMM8MN+.~.7~?N.IM,MMM$,........ ....,,,,:::::\
~:::::,,,,.. ......?MMMMMMMMMMMMN~,.MMMMMMMM,=$.8MM:~O=MM?...............,,,::::\
::::,,,,..........MMMMMMMMMMMMMMMMMD?NMMMMMMMMM~MMMMM...M=M.........  ....,,,,::\
::,,,,.... ......MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMDND,7Z ..............,,,,:\
,,,,...........,MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM...N.8=+O . .............,,,,\
,,,...........MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM?.?8MMM........... ....,,,\
,,......... .MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM~. .............,,\
,,....... .DMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM,.......... ....,\
,,....... MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8......... .....\
,,......?MMMMMMMMMMMMMMMMMMMND8OOZOZODMMMMMMMMMMMMMMMMMMMMMMMMMMMN..............\
,,.....MMMMMMMMMMMMMMMMMMMMMMMDOZ7I???I?I7$OMMMMMMMMMMMMMMMMMMMMMMD.............\
,,... 8MMMMMMNNMMMMMMMMMMNDDMMMNO7?I????I7?8O8MMMMMMMMMMMMMMMMMMMMMO............\
,,...=NMMM8888DMDDDDMMMMMMND8N8MN$7I?I?I7$8DMMDO8MMMMMMMMMMMMMMMMMMMD...........\
,,...MMMD888O8OODDDDMMMNMM8M+NNZ8NO7?I7ODNO$?I$ZODNMMMMMMMMMMMMMMMMMM7..........\
,,,.MMMMZOOZ8OD8D88DMMMDZ8Z7=??+OZO+=+7Z887777$DMNNMMMMMMMMMMMMMMMMMMM~.........\
,,,$MMMMZOZO8ZO8D88DMMDO7IZ$77I?III+=?77INI:MD$DMMNMMMMMMMMMMMMMMMMMMMM,........\
,,,MMMMMOOZ8O8OO888DNMOZ7?+=+=+II7?++I7$IIOII$7OMNNMMMNNNDMMMMMMMMMMMMMM~.......\
,,,MMMMMZZO8$OOOO88DNM8$7I?==+IIII?=+I77II?I??I7$ONMMM8O888DDNNMMMMMMMMMM ......\
,,,MMMMM$ZOOOZOOOOO88NN8Z7I???777?!=?I7$77?+???I$ODMM8O8O88OO888DDMMMMMMMM .....\
,,,MMMMMZZ8$ZZZZZZOOODMD8$7II?7I7?+==?IO$7I??II7Z8DM$OOO8888OOOZZZO8DNMMMMN.....\
,,:MMMMMZZZ7Z$$$$ZZZO88NDO$$7IO?7?::~?II7$7III7$ONNNOOOZOO8OOOZ$7$I7$ZODMMM, ...\
,:,MMMMMZO$$7$$$7$Z$O8DDDDO$I?78INI?7DMND$$7$Z88NMMOZOOZZZZ$$$$$$I?+II7ZNMMM  ..\
,,,MMMMMOZIZ$777$$$$$ZDDNDMDMNMMM8DMDMMN8$$O8DDNNNO8OOZZZZZZ$7$77I??+=+?8MMM  ..\
,,,8MMMMMZ7$I7I777$7$$NZDMMMMMNMNNMNMMMMMNM8DDNMNMOO8OZZ$$$7777III+=+===MMMM. ..\
,,,:MMMMM$777II77II77778ODMDNDDMNNNMMMMMMMMMNNNNNOD8O$Z$$7$7IIII??+=~~=~MMMM. ..\
,,,.MMMMMI7III?III77I7$DOD8ZOIII7N8DO8NMNMMMMMDNM88OZZ$$$IIII???++==~~~MMMMM. ..\
,,,,MMMMMNI???I?II77777$MND8OO8I++=+?7788DMMNNNMM8OOZ$$777I??+??+++==~IMMMMM  ..\
,,,,+MMMMM???I?I?I7I7IIIMM88D8DMMMMMNNND8MMNMNMNM8$$$$7III?????I?!=+=~MMMMM7  ..\
,,,,,MMMMMII???IIIII77I?MMM8$7IIDMMNNDD8DMMMNDDNMD$7$$7III????+?+++=~8MMMMM ....\
,,,,,,MMMMMI+???IIII77I?MMMMM88$ND$II77OMMMD8DDDM8777I77I?II?I?+++==NMMMMM:.....\
,,,...MMMMM7????I77IIIIIM8MMMMMMMMNNNMMMMNDD8DDDMMI7I77I+?I??III?++8MMMMMD. ....\
,,....+MMMMMI???7II7III?MN8NMMMMMMMMMMMMNNNDDDDNMM777I7III?I??I???8MMMMMM. .....\
,.,.,7MMMMMM7+??II77I7I?MMOO8MMMMMMMMMMMNNNNNNNMMMI777IIII?II++++8MMMMMMMMMM  ..\
,,$MMMMMMMMMM??II7I7I7?IMNO88DNMMMMMNMMMMMMMNMMMMMNDM$I$I?7I7??+MMMMMMMMMMMMMMM.\
MMMMMMMMMMMMMO=?I7II7II?D:$Z8DDNMMMMMMMMMMMMZ$$I$$?IIZ$$$$7$D++MMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMM?++??II77?DM77Z8DNMMMMMM77II??+++++?I7Z$I?+++?II~=?MMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMM==+?IIII8I.?I$8DDMMMD88Z$8DOZ7$I$$7NNZZ777ZZZ$I=$MMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMM=++?II?DDZ,+I$O8DNM8$++????III????$8ONM$$8ZDONMMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMMM+=++?DI~$I=??7$O8D8ZDDO7?++7I7I?I$Z$ZO++=DO$MMMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMMMM+++7+=~7+II=?I$$8M87I$$$77$7$NMMMDO8M7??ZOODMMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMMMMMOO?=+~O??+++??78NOZ777?I?????I7$8ON87IDMMM=NMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMMMMMMM~===?I?===??DNND8Z$OO$$7????IIIZO8MMMMMMMMMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMMMMMMD====~$??I:7I8DDD8I?7$$$77$NN$8D8NDDMMMMMMMMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMMMMMMM8+===D???+=DMDNN8Z???I7$778MDZO8DNNMMMMMMMMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMMMMMMMM+===~7IIIINNDDD8O$$7$ZZ7$ZNZONM8MMMMMMMMMMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMMMMMMMMM+==~:,=77MNND8OOOZZZNMDNMMM8NDNMOMMMMMMMMMMMMMMMMMMMMMMM\
MMMMMMMMMMMMMMMMMMMMMMMMM+=MMMMMMNND88OZ7$+ZNDDNNMMD8DDMMMMMMMMMMMMMMMMMMMMMMMMM\
";

int snoopLine = 51;
int snoopSize = 4183-51;
int snoopWidth = 80;


//~ syntax :
//~ {
	//~ {
		//~ {r,g,b},{r,g,b}
	//~ },
	//~ {
		//~ {r,g,b},{r,g,b}
	//~ },
	//~ {
		//~ {r,g,b},{r,g,b}
	//~ }	
//~ };

uint8_t snoopArray[3][10][3] = {
	{{0x00,0x00,0xFF}, {0x00,0x00,0xFF},{0x00,0x00,0xFF}, {0x00,0x00,0xFF},{0x00,0x00,0xFF}, {0x00,0x00,0xFF}, {0x00,0x00,0xFF},{0x00,0x00,0xFF}, {0x00,0x00,0xFF},{0x00,0x00,0xFF}},
    {{0xFF,0x00,0x00}, {0xFF,0x00,0x00},{0xFF,0x00,0x00}, {0xFF,0x00,0x00},{0xFF,0x00,0x00} , {0xFF,0x00,0x00}, {0xFF,0x00,0x00},{0xFF,0x00,0x00}, {0xFF,0x00,0x00},{0xFF,0x00,0x00}},
    {{0x00,0xFF,0x00}, {0x00,0xFF,0x00},{0x00,0xFF,0x00}, {0x00,0xFF,0x00},{0x00,0xFF,0x00} , {0x00,0xFF,0x00}, {0x00,0xFF,0x00},{0x00,0xFF,0x00}, {0x00,0xFF,0x00},{0x00,0xFF,0x00}}
    };
                    
                    
void drawSnoop()
{
	int largeur = 10;
	int hauteur = 3;
	for(int i = 0;i<largeur;i++)
	{
		for(int j = 0;j<hauteur;j++)
		{
			uint8_t red = snoopArray[j][i][0];
			uint8_t blue = snoopArray[j][i][1];
			uint8_t green = snoopArray[j][i][2];
			put_pixel_RGB24(i+5,j+10,red,blue,green);
		}
	}
	for(int k = 0;k<10;k++)
	{
		newLine();
	}
}

void printSnoop()
{
	int cpt = 0;
	for(int i = 0;i<snoopSize;i++)
	{
		cpt++;
		if(cpt == 80){
			drawChar(snoopIMG[i]);
			newLine();
			cpt= 0;
		}
		else
		{
			drawChar(snoopIMG[i]);
		}
	}
	newLine();
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

void UsbInitialise();
void KeyboardUpdate();
char KeyboardGetChar();

char commandLine[256];
int commandLineSize = 0;

char command[256];
int commandSize = 0;

char parameters[256];
int parametersSize = 0;

char history[256];
int historySize = 0;

void testAffichage()
{
	for(int i = 0; i < commandLineSize; i++)
	{
		drawChar(commandLine[i]);
	}
	drawChar(commandLine[0]);
}

void commandProcess()
{
	// Récupération de la commande et du reste dans deux variables
	// différentes
	int index = 0;
	while(commandLine[index] != ' ')
	{
		if(index != commandLineSize)
		{
			command[commandSize] = commandLine[index];
			commandSize++;
			index++;
		}
		else
		{
			break;
		}
	}
	command[commandSize] = '\0';
	commandSize++;
	
	index++;
	while(commandLine[index] == ' ')
	{
		index++;
	}

	for(int i = index; i < commandLineSize; i++)
	{
		parameters[parametersSize] = commandLine[index];
		parametersSize++;
		index++;
	}
	parameters[parametersSize] = '\0';
	parametersSize++;
	
	// Traitement de chaque commande individuellement
	// OK echo : Affiche à l'écran ce qui est tapé.
	// OK music : Lance audio_test qui joue de la musique.
	// xx currentTime
	// xx reboot
	int resultEcho = strcmp("echo", command);
	int resultMusic = strcmp("music", command);
	
	if(resultEcho == 0)
	{
		drawString(parameters, parametersSize);
	}
	else if(resultMusic == 0) 
	{
		drawString("Quelle douce musique...", 23);
		audio_test();
	}
	else
	{
		drawError("La commande n'existe pas.", 25);
	}
	
	commandSize = 0;
	parametersSize = 0;
}

void applyMethod()
{
	commandProcess();
	commandLineSize = 0;
	newLine();
	drawChar('>');
	drawChar(' ');
}

void keyboardLoop()
{
	while(1){
		KeyboardUpdate();
		
		char c = KeyboardGetChar();
		
		if (c != 0)
		{
			if(c == '\n')
			{
				// sauvegarde de l'historique
				//~ drawChar(commandLine[0]);
				//~ drawChar(command[0]);
				for(int i = 0; i<commandLineSize;i++)
				{
					history[i] = commandLine[i];
					historySize++;
				}
				
				newLine();
				applyMethod();
			}
			else if(c == 'h')
			{
				// restauration de l'historique
				for(int i = 0; i<historySize;i++)
				{
					commandLine[i] = history[i];
				}
				commandLineSize = historySize;
				newLine();
				drawChar('$');
				drawChar(' ');
				for(int i = 0; i<commandLineSize;i++)
				{
					drawChar(commandLine[i]);
				}
				
			}
			else
			{
				commandLine[commandLineSize] = c;
				commandLineSize++;
				drawChar(c);
			}
		}
	}
}

void printHeader()
{
	drawHelloConsole();
	while(1)
	{
		volatile int i = 0;
		i++;
	}
}

void dummy()
{
	while(1)
	{
		volatile int i = 0;
		i++;
	}
}


int kmain (void)
{
	sched_init(); 
	
	create_process((func_t*)&printHeader);
	create_process((func_t*)&dummy);
	create_process((func_t*)&keyboardLoop);
	

	// init screen
	FramebufferInitialize();
	
	// init keyboard
	UsbInitialise();
	
	// clean screen
	clear();
	
	hw_init();
	
	
	timer_init();
	ENABLE_IRQ();
	
	__asm("cps 0x10");
	
	drawSnoop();
	while(1)
	{
		sys_yield();
	}

	return 0;
}
