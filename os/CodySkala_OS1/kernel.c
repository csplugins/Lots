/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:____Cody_Skala_______________________ Date:__2/9/16_____        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.02, Spring 2016.                 */

void handleInterrupt21(int, int, int, int);

void main(){
   char line[80];
   int x;
   
   /*Prepare the interrupt*/
   makeInterrupt21();
   /*Clear Screen*/
   interrupt(33, 12, 5, 16, 0);

   /*PrintScreen for BlackDOS header*/
   interrupt(33,0,"___.   .__                 __       .___           \r\n\0",0,0);
   interrupt(33,0,"\\_ |__ |  | _____    ____ |  | __ __| _/___  ______\r\n\0",0,0);
   interrupt(33,0," | __ \\|  | \\__  \\ _/ ___\\|  |/ // __ |/ _ \\/  ___/\r\n\0",0,0);
   interrupt(33,0," | \\_\\ \\  |__/ /\\ \\\\  \\___|    </ /_/ ( <_> )___ \\ \r\n\0",0,0);
   interrupt(33,0," |___  /____(____  /\\___  >__|_ \\____ |\\___/____  >\r\n\0",0,0);
   interrupt(33,0,"     \\/          \\/     \\/     \\/    \\/         \\/ \r\n\0",0,0);
   interrupt(33,0," V. 1.02, C. 2016. Based on a project by M. Black. \r\n\0",0,0);
   interrupt(33,0," Author: Cody Skala\r\n\r\n\0",0,0);

   /*A few more interrupts that will trigger printScreen*/
   interrupt(33,0,"Hola mondo.\r\n\0",0,0);
   interrupt(33,0,"Enter a line: \0",0,0);
   /*This interrupt will call readString and save it to line*/
   interrupt(33,1,line,0,0);
   /*This will print to the screen what was typed in line*/
   interrupt(33,0,"\r\nYou typed: \0",0,0);
   interrupt(33,0,line,0,0);
   interrupt(33,0,"\r\n\0",0,0);
   interrupt(33,0,"Enter a number: \0",0,0);
   /*This will interrupt for the readInt function and save the number to x*/
   interrupt(33,14,&x,0,0);
   interrupt(33,0,"Your number is \0",0,0);
   /*This will call the interrupt to writeInt function and print the number saved in x*/
   interrupt(33,13,x,0,0);
   /*A final printScreen to go to the next line*/
   interrupt(33,0,"\r\n\0",0,0);
   /*Pause the OS from restarting*/
   while(1);
}

/*Prints to the screen with Interrup 16(0x10)/Function 14 (0x0E)*/
void printString(char* c){
  int i = 0;
  /*Iterate through all the string characters and put them to the screen*/
  while(1){
    if(c[i] == '\0')
      break;
    else interrupt(16,14*256+c[i],0,0,0);
    i++;
  }
  return;
}

/*Reads keyboard input and saves it to the character array c*/
void readString(char* c){
  char temp;
  int i = 0;
  while(1){
    temp = interrupt(22, 0, 0, 0, 0);
    /*Determine what was pressed and hanlde it appropriately*/
    /*If enter was pressed, append null terminator and exit the loop*/
    if (temp == 13){
      c[i] = '\0';
      break;
    }
    /*If anything but backspace is pressed, append the newly typed char*/
    else if(temp != 8){
      c[i] = temp;
      interrupt(16,14*256+temp,0,0,0);
      i++;
    }
    /*If backspace was pressed, remove the last char and go back an index*/
    /*ONLY WORKS IF YOU ARE ALLOWED TO KEEP DELETING CHARS*/
    else if(i > 0){
      i--;
      c[i] = 0;
      /*This will make the letter disappear from the screen*/
      interrupt(16,14*256+temp,0,0,0);
      interrupt(16,14*256+' ',0,0,0);
      interrupt(16,14*256+temp,0,0,0);
    }
  }
  return;
}

/*This will remove all text currently on the screen then set colors*/
void clearScreen(int bx, int cx){
  int i;
  /*Remove current text on screen*/
  for(i = 0; i < 24; i = i+1){
    interrupt(16,14*256+'\r',0,0,0);
    interrupt(16,14*256+'\n',0,0,0);
  }
  /*Move cursor to top left of screen*/
  interrupt(16,512,0,0,0);
  /*Make sure colors are in range of what they can be set to*/
  if(bx > 8 || cx > 16 || bx < 1 || cx < 1)
    return;
  /*Set the colors to what is passed into the function based off palette*/
  interrupt(16,1536,4096*(bx-1) + 256*(cx-1), 0, 6223);
  return;
}

/*This was provided ahead of time but provides mod operations*/
int mod(int a, int b){
   int x = a;
   while (x >= b) x = x - b;
   return x;
}

/*This too was provided ahead of time and provides the quotient of division*/
int div(int a, int b){
   int q = 0;
   while (q * b <= a) q++;
   return (q - 1);
}

/*This was also provided ahead of time and prints an int to screen*/
void writeInt(int x){
   char number[6], *d;
   int q = x, r;
   if (x < 1){
      d = number; *d = 0x30; d++; *d = 0x0; d--;
   }
   else{
      d = number + 5;
      *d = 0x0; d--;
      while (q > 0){
         r = mod(q,10); q = div(q,10);
         *d = r + 48; d--;
      }
      d++;
   }
   printString(d);
}

/*This will read an integer string and convert it into an int type*/
void readInt(int* number){
  int i = 0;
  char temp[6];
  *number = 0;
  readString(temp);
/*If the string still has numbers to be converted, add to the current int*/
  while(temp[i] != '\0'){
    *number = *number*10 + temp[i]-48;
    i++;
    }
/*End with a new line so when enter is pressed, it automatically goes down*/
  printString("\r\n\0");
  return;
}

/*This determines what function to call based off of the interupt occurence*/
void handleInterrupt21(int ax, int bx, int cx, int dx){
  if(ax == 0)
    printString(bx);
  else if(ax == 1)
    readString(bx);
  else if(ax == 12)
    clearScreen(bx, cx);
  else if(ax == 13)
    writeInt(bx);
  else if(ax == 14)
    readInt(bx);
  else printString("AX was some value that could not be handled!\r\n\0");
  return;
}
