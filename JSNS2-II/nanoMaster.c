//****************************
//* Master Control for JSNS2 *
//*  Richard White 1/08/18   *
//****************************

#DEVICE PIC18LF4420
#include<18LF4420.h>

#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, ERRORS, stream=com_A)  // Main UART to PC
#use rs232(baud=9600, xmit=PIN_C4, rcv=PIN_C3, stream=com_B)  // Secondary to NANOpulsers (use 'invert' if needed)
#include <stdlib.h>

//#define readbranch1 PIN_D5
//#define readbranch2 PIN_B1
//#define readbranch3 PIN_B2
//#define readbranch4 PIN_B3
//#define readbranch5 PIN_B4
//#define readbranch6 PIN_B5

int command=0b00000011; //write and update DAC
int address1=0b00000000; //select DACA
long height, dtoabit, pheight, timeout, num;
short timeout_error;
int16 i;
int x, y, cmd, addrs1, heighth, heightl, lnum, usdelh, usdell, numhi, numlo; 
int trigdelay1, trigdelay2;
char module;


void triggerdelay5(void);
void triggerdelay025(void);
void run(void);
void continuous(void);
void numberhi(void);
void numberlo(void);
void singleselect(void);
void stop(void);
void loadheight(void);
void heighthi(void);
void heightlo(void);
void usdelay(void);
void flash1(void);
void flash2(void);
void flash3(void);
void reset(void);
void endrun(void);
void rtncode(void);

void main(void)
{
output_high(PIN_D6); // Turn OFF module resets
output_high(PIN_D7); //         "

output_low(PIN_D1); // Green LED ON
delay_ms(800);
output_high(PIN_D1); // Green LED OFF
delay_ms(500);
output_low(PIN_D1); // Green LED ON
delay_ms(400);
output_high(PIN_D1); // Green LED OFF

 
output_high(PIN_B0); // enable RS485 TX to all modules
output_high(PIN_D5); // disable RS485 RX Cable 1  ****for new board****
output_high(PIN_B1); // disable RS485 RX Cable 2
output_high(PIN_B2); // disable RS485 RX Cable 3
output_high(PIN_B3); // disable RS485 RX Cable 4
output_high(PIN_B4); // disable RS485 RX Cable 5
output_high(PIN_B5); // disable RS485 RX Cable 6
delay_ms(5);

loop:
x=fgetc(com_A); 
if (x=='a') continuous();
if (x=='d') triggerdelay5();
if (x=='e') triggerdelay025();
if (x=='g') run();
if (x=='H') numberhi();
if (x=='G') numberlo();
if (x=='I') singleselect();
if (x=='L') heighthi();
if (x=='M') heightlo();
if (x=='P') loadheight();
if (x=='u') usdelay();
if (x=='x') stop();
if (x=='A') flash1();
if (x=='B') flash2();
if (x=='F') flash3();
if (x=='R') reset(); 
goto loop;
}
//Test Master and slave with one LED flash
//**************************************************
void flash1(void)
{
output_low(PIN_D1); // Green LED ON
delay_ms(200);
output_high(PIN_D1); // Green LED OFF
delay_ms(200);
fprintf(com_B,"%c",module);
delay_ms(1);
fprintf(com_B,"@");
delay_ms(1);
fprintf(com_B,"x");
fprintf(com_A,"x");
//rtncode(); // Waits for slave return code(0.5 sec max) 
}

//Test Master and slave with two LED flashes
//**************************************************
void flash2(void)
{
output_low(PIN_D1);
delay_ms(200); 
output_high(PIN_D1);
delay_ms(200);
output_low(PIN_D1);
delay_ms(200);
output_high(PIN_D1);
delay_ms(200);
fprintf(com_B,"%c",module);
delay_ms(1);
fprintf(com_B,"@");
delay_ms(1);
fprintf(com_B,"y");
fprintf(com_A,"y");
}

//Test Master and slave with three LED flashes
//**************************************************
void flash3(void)
{
output_low(PIN_D1);
delay_ms(200);
output_high(PIN_D1);
delay_ms(200);
output_low(PIN_D1);
delay_ms(200);
output_high(PIN_D1);
delay_ms(200);
output_low(PIN_D1);
delay_ms(200);
output_high(PIN_D1);
delay_ms(200);
fprintf(com_B,"%c",module);
delay_ms(1);
fprintf(com_B,"@");
delay_ms(1);
fprintf(com_B,"z");
fprintf(com_A,"z");
}

//Select Slave board (module)
//***************************************************
void singleselect(void)
{
lnum=fgetc(com_A);
if (lnum==1)
{
module=65;
//branch=1;
fprintf(com_A,"B1");
}
if (lnum==2)
{
module=66;
//branch=1;
fprintf(com_A,"B2");
}
if (lnum==3)
{
module=67;
//branch=2;
fprintf(com_A,"B3");
}
if (lnum==4) 
{
module=68;
//branch=2;
fprintf(com_A,"B4");
}
if (lnum==5) 
{
module=69;
//branch=3;
fprintf(com_A,"B5");
}
if (lnum==6) 
{
module=70;
//branch=3;
fprintf(com_A,"B6");
}
if (lnum==7) 
{
module=71;
//branch=4;
fprintf(com_A,"B7");
}
if (lnum==8) 
{
module=72;
//branch=4;
fprintf(com_A,"B8");
}
if (lnum==9) 
{
module=73;
//branch=5;
fprintf(com_A,"B9");
}
if (lnum==10) 
{
module=74;
//branch=5;
fprintf(com_A,"B10");
}
if (lnum==11) 
{
module=75;
//branch=6;
fprintf(com_A,"B11");
}
if (lnum==12) 
{
module=76;
//branch=6;
fprintf(com_A,"B12");
}
if (lnum==13) 
{
module=77;
//branch=6;
fprintf(com_A,"B13");
}
if (lnum==14) 
{
module=78;
//branch=14;
fprintf(com_A,"B14");
}
/*if (lnum==15) 
{
module=79;
fprintf(com_A,"B15");
}
if (lnum==16) 
{
module=80;
fprintf(com_A,"B16");
}
if (lnum==17) 
{
module=81;
fprintf(com_A,"B17");
}
if (lnum==18) 
{
module=82;
fprintf(com_A,"B18");
}
if (lnum==19) 
{
module=83;
fprintf(com_A,"B19");
}
if (lnum==20) 
{
module=84;
fprintf(com_A,"B20");
}
if (lnum==21) 
{
module=85;
fprintf(com_A,"B21");
}
if (lnum==22) 
{
module=86;
fprintf(com_A,"B22");
}
if (lnum==23) 
{
module=87;
fprintf(com_A,"B23");
}
if (lnum==24) 
{
module=88;
fprintf(com_A,"B24");
}
if (lnum==25) 
{
module=89;
fprintf(com_A,"B25");
}
*/
}

//Continuous
//**********************************************
void continuous(void)
{

fprintf(com_B, "%c",module); // module is a character from A to T (65-89)
delay_ms(1);
fprintf(com_B,"@");
delay_ms(1);
fprintf(com_B,"a");
fprintf(com_A,"a");
output_low(PIN_D1); // Green LED ON
}

// Run
//**********************************************
void run(void)
{
num=numhi*numlo;

output_low(PIN_D1); // Pulsing LED ON
fprintf(com_B, "%c",module); // module is a character from A to T (65-89)
delay_ms(1);
fprintf(com_B,"@");
delay_ms(1);
fprintf(com_B,"g");

do
{
output_high(PIN_E0); // output Dummy main pulse
output_low(PIN_E0);

delay_us(usdell);
delay_us(usdell);
delay_us(usdell);
delay_us(usdell);
delay_ms(usdelh);

if(kbhit()) goto jump1;
num=--num;
} while(num>=1);

jump1:
fprintf(com_A,"K");
output_high(PIN_D1); // Pulsing LED OFF
 
}


//Load pulse height high byte
//***********************************************
void heighthi(void)
{
heighth=fgetc(com_A);

fprintf(com_B,"%c",module); // module is a character from A to Z
delay_us(1);
fprintf(com_B,"@");
delay_us(1);
fprintf(com_B,"L");
delay_us(1);
fputc(heighth,com_B);

fprintf(com_A,"L");
}

//Load pulse height low byte
//***********************************************
void heightlo(void)
{
heightl=fgetc(com_A);

fprintf(com_B,"%c",module); // module is a character from A to Z
delay_us(1);
fprintf(com_B,"@");
delay_us(1);
fprintf(com_B,"M");
delay_us(1);
fputc(heightl,com_B); 
fprintf(com_A,"M");
}

// Stop Pusing sequence
//**********************************************
void stop(void)
{
fprintf(com_B,"x");
fprintf(com_A,"x");
output_high(PIN_D1); // Green LED OFF
}

//Trigger Delay 5nS steps
//**************************************************
void triggerdelay5(void)
{
trigdelay1=fgetc(com_A);
fprintf(com_B,"%c",module); // module is a character from A to Z
delay_us(1);
fprintf(com_B,"@");
delay_us(1); 
fprintf(com_B,"d");
delay_us(1); 
fputc(trigdelay1,com_B);
fprintf(com_A,"d");
}

//Trigger Delay 0.25nS steps 
//**************************************************
void triggerdelay025(void)
{
trigdelay2=fgetc(com_A);
fprintf(com_B,"%c",module); // module is a character from A to Z
delay_us(1); 
fprintf(com_B,"@");
delay_us(1); 
fprintf(com_B,"e");
delay_us(1); 
fputc(trigdelay2,com_B); 
fprintf(com_A,"e");
}

//Number of pulses high byte
//***************************************************
void numberhi(void)
{
numhi=fgetc(com_A);
fprintf(com_B, "%c",module); // module is a character from A to Z
delay_us(1);
fprintf(com_B,"@");
delay_us(1);
fprintf(com_B,"H");
delay_us(1);
fputc(numhi,com_B);
fprintf(com_A,"H");
}

//Number of pulses low byte
//***************************************************
void numberlo(void)
{
numlo=fgetc(com_A);
fprintf(com_B, "%c",module); // module is a character from A to Z
delay_us(1); 
fprintf(com_B,"@");
delay_us(1); 
fprintf(com_B,"G");
delay_us(1); 
fputc(numlo,com_B);
fprintf(com_A,"G");
}

//Load Pulse Height
//**********************************************
void loadheight(void)
{
height=(heighth*256)+heightl;
pheight=height;
cmd=command;
addrs1=address1;

//*******set height on control board***********
output_low(PIN_A5);  //set CS/LOAD low on D/A
output_low(PIN_A4);  //Set clock low

for(i=1;i<=4;i++)  //send command to write and update
{
 dtoabit=bit_test(cmd,3); //look at Bit3
 output_bit(PIN_A3, dtoabit);   //present data to LTC2612
 output_high(PIN_A4);   //clock data
 output_low(PIN_A4);
 cmd=cmd << 1; //move next into Bit 3
}
for(i=1;i<=4;i++)  //address DAC1
{
 dtoabit=bit_test(addrs1,3); //look at Bit3
 output_bit(PIN_A3, dtoabit);   //present data to LTC2612
 output_high(PIN_A4);   //clock data
 output_low(PIN_A4);
 addrs1=addrs1 << 1; //move next into Bit3
}
for(i=1;i<=14;i++) //send data
{
 dtoabit=bit_test(height,13); //look at Bit13
 output_bit(PIN_A3, dtoabit);   //present data to LTC2612
 output_high(PIN_A4);   //clock data
 output_low(PIN_A4);
 height=height << 1; //move next into Bit13
}
 output_high(PIN_A4);   //clock don't care data
 output_low(PIN_A4);
 output_high(PIN_A4);   //clock don't care data
 output_low(PIN_A4);    // giving a total of 24 clock cycles

output_high(PIN_A5); //set CS/LOAD high to disable D/A
 

// **********Set height on module*******************
fprintf(com_B,"%c",module); // module is a character from A to Z
delay_us(1);
fprintf(com_B,"@");
delay_us(1);
fprintf(com_B,"P");
delay_us(1);
fputc(pheight,com_B); //send Height to slave
fprintf(com_A,"P");
}


//Set repetition rate
//**********************************************
void usdelay(void)
{
usdelh=getch();
delay_us(1);
usdell=getch();
fprintf(com_B, "%c",module); // module is a character from A to Z
delay_us(1);
fprintf(com_B,"@");
delay_us(1);
fprintf(com_B,"u");
delay_us(1);
fputc(usdelh,com_B);
delay_us(1);
fputc(usdell,com_B);
delay_us(10);
fprintf(com_A,"u");
}

//Master Reset ALL slave modules
//**********************************************
void reset(void)
{  
output_low(PIN_D6); // Enable module resets
output_low(PIN_D7); // RESET pulse
delay_ms(10);
output_high(PIN_D7); //
output_high(PIN_D6); //Disable module resets
}

// Turn off Run indicator LED
void endrun(void)
{
output_high(PIN_D1); // Green LED OFF
output_high(PIN_B1); // disable RS485 RX Cable 1
output_high(PIN_B0); // enable RS485 TX to all modules

}
//Re-transmit return code from slave
//**********************************************
void rtncode(void) 
{
timeout_error=FALSE; 
timeout=0; 
   
   while(!kbhit()&&(++timeout<50000)) // 1/2 second 
      delay_us(10); 

   if(kbhit()) 
      y=fgetc(); 
   else { 
      timeout_error=TRUE; 
      y=0; 
   } 
 fprintf(com_A, "%c", y); // y is a character  
}



/*
//Select modules (and branch)
//**********************************************
void singleselect(void)
{
//lnum=getch();
lnum=fgetc(com_A);
 
//Branch 1
//*********************** 
 if (lnum >=1 && lnum<=6)
   {
     output_high(PIN_B0); //check TX port
     if (lnum==1)
       {
    module=65;
       }
     fprintf(com_A,"B1");
   }

 if (lnum >=1 && lnum<=6)
   {
     output_high(PIN_B0); //check TX port
     if (lnum==2)
       {
    module=66;
       }
     fprintf(com_A,"B2");
   }

 if (lnum >=1 && lnum<=6)
   {
     output_high(PIN_B0); //check TX port
     if (lnum==3)
       {
    module=67;
       }
     fprintf(com_A,"B3");
   }

 if (lnum >=1 && lnum<=6)
   {
     output_high(PIN_B0); //check TX port
     if (lnum==4)
       {
    module=68;
       }
     fprintf(com_A,"B4");
   }

 if (lnum >=1 && lnum<=6)
   {
     output_high(PIN_B0); //check TX port
     if (lnum==5)
       {
    module=69;
       }
     fprintf(com_A,"B5");
   }

 if (lnum >=1 && lnum<=6)
   {
     output_high(PIN_B0); //check TX port
     if (lnum==6)
       {
    module=70;
       }
     fprintf(com_A,"B6");
   }

 if (lnum >=7 && lnum<=12)
   {
     output_high(PIN_B0);
     if (lnum==7)
       {
    module=71;
       }
     fprintf(com_A,"B7");
   }

if (lnum >=7 && lnum<=12)
   {
     output_high(PIN_B0);
     if (lnum==8)
       {
    module=72;
       }
     fprintf(com_A,"B8");
   }

if (lnum >=7 && lnum<=12)
   {
     output_high(PIN_B0);
     if (lnum==9)
       {
    module=73;
       }
     fprintf(com_A,"B9");
   }

if (lnum >=7 && lnum<=12)
   {
     output_high(PIN_B0);
     if (lnum==10)
       {
    module=74;
       }
     fprintf(com_A,"B10");
   }

if (lnum >=7 && lnum<=12)
   {
     output_high(PIN_B0);
     if (lnum==11)
       {
    module=75;
       }
     fprintf(com_A,"B11");
   }

if (lnum >=7 && lnum<=12)
   {
     output_high(PIN_B0);
     if (lnum==12)
       {
    module=76;
       }
     fprintf(com_A,"B12");
   }

if (lnum >=13 && lnum<=18)
   {
     output_high(PIN_B0);
     if (lnum==13)
       {
    module=77;
       }
     fprintf(com_A,"B13");
   }

 if (lnum >=13 && lnum<=18)
   {
     output_high(PIN_B0);
     if (lnum==14)
       {
    module=78;
       }
     fprintf(com_A,"B14");
   }

 if (lnum >=13 && lnum<=18)
   {
     output_high(PIN_B0);
     if (lnum==15)
       {
    module=79;
       }
     fprintf(com_A,"B15");
   }

 if (lnum >=13 && lnum<=18)
   {
     output_high(PIN_B0);
     if (lnum==16)
       {
    module=80;
       }
     fprintf(com_A,"B16");
   }

 if (lnum >=13 && lnum<=18)
   {
     output_high(PIN_B0);
     if (lnum==17)
       {
    module=81;
       }
     fprintf(com_A,"B17");
   }

 if (lnum >=13 && lnum<=18)
   {
     output_high(PIN_B0);
     if (lnum==18)
       {
    module=82;
       }
     fprintf(com_A,"B18");
   }

 if (lnum >=19 && lnum<=24)
   {
     output_high(PIN_B0);
     if (lnum==19)
       {
    module=83;
       }
     fprintf(com_A,"B19");
   }

 if (lnum >=19 && lnum<=24)
   {
     output_high(PIN_B0);
     if (lnum==20)
       {
    module=84;
       }
     fprintf(com_A,"B20");
   }

 if (lnum >=19 && lnum<=24)
   {
     output_high(PIN_B0);
     if (lnum==21)
       {
    module=85;
       }
     fprintf(com_A,"B21");
   }

 if (lnum >=19 && lnum<=24)
   {
     output_high(PIN_B0);
     if (lnum==22)
       {
    module=86;
       }
     fprintf(com_A,"B22");
   }

 if (lnum >=19 && lnum<=24)
   {
     output_high(PIN_B0);
     if (lnum==23)
       {
    module=87;
       }
     fprintf(com_A,"B23");
   }

 if (lnum >=19 && lnum<=24)
   {
     output_high(PIN_B0);
     if (lnum==24)
       {
    module=88;
       }
     fprintf(com_A,"B24");
   }

//******Test only********
output_low(PIN_D1); // Green LED ON
delay_ms(500);
output_high(PIN_D1); // Green LED OFF
delay_ms(500); 
//******Test only******** 
}

*/

//       Richard White - February 2018
//*******************END*************************

