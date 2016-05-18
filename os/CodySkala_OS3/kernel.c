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
   char buffer[13312];
   int size;
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

   /*Clear screen on reboot and say welcome to BlackDos*/
   interrupt(33, 12, 5, 16, 0);
   interrupt(33, 0, "Welcome to BlackDos!\r\n\0", 0, 0);

   /*Execute the Shell program*/
   interrupt(33,4,"Shell\0",2,0);

   /*Bad news if this ever prints to screen....*/
   interrupt(33,0,"Bad or missing command interpreter.\r\n\0",0,0);
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
  interrupt(16,14*256+'\r',0,0,0);
  interrupt(16,14*256+'\n',0,0,0);
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

/*This function is called for certain cases of different failures or issues*/
void error(int bx){
   char errMsg0[18], errMsg1[17], errMsg2[13], errMsg3[17];
   /*errMsg0 is File not found.*/
   errMsg0[0] = 70; errMsg0[1] = 105; errMsg0[2] = 108; errMsg0[3] = 101;
   errMsg0[4] = 32; errMsg0[5] = 110; errMsg0[6] = 111; errMsg0[7] = 116;
   errMsg0[8] = 32; errMsg0[9] = 102; errMsg0[10] = 111; errMsg0[11] = 117;
   errMsg0[12] = 110; errMsg0[13] = 100; errMsg0[14] = 46; errMsg0[15] = 13;
   errMsg0[16] = 10; errMsg0[17] = 0;
    /*errMsg1 is Bad file name.*/
   errMsg1[0] = 66; errMsg1[1] = 97; errMsg1[2] = 100; errMsg1[3] = 32;
   errMsg1[4] = 102; errMsg1[5] = 105; errMsg1[6] = 108; errMsg1[7] = 101;
   errMsg1[8] = 32; errMsg1[9] = 110; errMsg1[10] = 97; errMsg1[11] = 109;
   errMsg1[12] = 101; errMsg1[13] = 46; errMsg1[14] = 13; errMsg1[15] = 10;
   errMsg1[16] = 0;
   /*errMsg2 is Disk full.*/
   errMsg2[0] = 68; errMsg2[1] = 105; errMsg2[2] = 115; errMsg2[3] = 107;
   errMsg2[4] = 32; errMsg2[5] = 102; errMsg2[6] = 117; errMsg2[7] = 108;
   errMsg2[8] = 108; errMsg2[9] = 46; errMsg2[10] = 13; errMsg2[11] = 10;
   errMsg2[12] = 0;
   /*errMsg3 is General DOS failure.*/
   errMsg3[0] = 71; errMsg3[1] = 101; errMsg3[2] = 110; errMsg3[3] = 101;
   errMsg3[4] = 114; errMsg3[5] = 97; errMsg3[6] = 108; errMsg3[7] = 32;
   errMsg3[8] = 101; errMsg3[9] = 114; errMsg3[10] = 114; errMsg3[11] = 111;
   errMsg3[12] = 114; errMsg3[13] = 46; errMsg3[14] = 13; errMsg3[15] = 10;
   errMsg3[16] = 0;

   switch(bx) {
     case 0: printString(errMsg0); break;
     case 1: printString(errMsg1); break;
     case 2: printString(errMsg2); break;
     default: printString(errMsg3);
   }
}

/*This calculates relative sector and reads it into a buffer with an iterrupt*/
void readSector(char* buffer, int sector){
  int relSecNo = mod(sector,18) + 1;
  int headNo = mod(div(sector, 18), 2);
  int trackNo = div(sector, 36);
  int CX = trackNo * 256 + relSecNo;
  int DX = headNo * 256;
  interrupt(19, 513, buffer, CX, DX);
}

/*This calculates relative sector and writes it into a buffer with an iterrupt*/
void writeSector(char* buffer, int sector){
  int relSecNo = mod(sector,18) + 1;
  int headNo = mod(div(sector, 18), 2);
  int trackNo = div(sector, 36);
  int CX = trackNo * 256 + relSecNo;
  int DX = headNo * 256;
  interrupt(19, 769, buffer, CX, DX);
}

/*Helper function for readFile to look if a file exists*/
int findFile(char* directorySector, char* fileName){
  int i, j;
  int checkFound = 0;
  for (i = 0; i < 16; i++){
    /* If there is a file at the location */
    if (directorySector[32*i] != 0){
      /* Check the first characters */
      for (j=0; j < 6; j++){
        /*Check if end of file name*/
        if (fileName[j] == 0){
          if(directorySector[j+32*i] != 0){
            checkFound = 0;
          }
          break;
        }
        /*Check if characters match and continue checking remaining characters*/
        if (directorySector[j+32*i] == fileName[j]){
          checkFound = 1;
        }
        /*Check if characters are not equal and begin checking the next file*/
        else {
          checkFound = 0;
          break;
        }
      }
      /*If the file is found, return the index of that file*/
      if (checkFound == 1){
        return i;
      }
    }
  }
  /*This will return zero if the file is not found*/
  return -1;
}

/*This function will attempt to find a file that was loaded in the image*/
void readFile(char* fname, char* buffer, int* size){
  char directorySector[512];
  int index;
  int i;
  int buffAddress = 0;
  /*Directory sector loads sector 2 to start loading the known files*/
  readSector(directorySector, 2);
  /*Helper function to look for the filename in the directory sector*/
  index = findFile(directorySector,fname);
  /*Check if the file was found. If it was, load it into the buffer*/
  if (index != -1){
    index = index*32+6;
    /*Load all of the file sectors*/
    for (i=0;i<26;i++){
      if(directorySector[index+i] != 0){
        *size = *size + 1;;
      }
      readSector(buffer+buffAddress,directorySector[index+i]);
      buffAddress += 512;
    }
  }
  /*Let the user know the file was not found*/
  else{
    buffer[0] = 0;
    interrupt(33, 15, 0, 0, 0);
  }
}

/*This function will attempt to find a file and remove it*/
void deleteFile(char* name){
  char map[512];
	char directory[512];
	int i, index;

	readSector(map,1);
	readSector(directory,2);

	index = findFile(directory,name);
  /*If the file does exist*/
	if (index!=-1){
    /*Set the first character of the file to free(null)*/
		directory[index*32] = 0;
		index = index*32+6;
		for (i=0;i<26;i++){
			if(directory[index+i] == 0) break;
      /*Remove file from map sector*/
      map[directory[index+i]] = 0;
		}
	}
	else{
    /*File does not exist*/
		interrupt(33, 15, 0, 0, 0);
	}
  /*Write the changes back to the disk and map sectors*/
	writeSector(map,1);
	writeSector(directory,2);
}

/*Atempts to put a new file into disk with certain contents*/
void writeFile(char* name, char* buffer, int numberOfSectors){
  char map[512];
	char directory[512];
  char subBuff[512];
	int i, j, index, sectorNum;
	int freeSpace = -1;

	readSector(map,1);
	readSector(directory,2);

  /*If the file already exists, inform user*/
  if(findFile(directory,name) != -1){
    interrupt(33, 15, 1, 0, 0);
    return;
  }

  /*Find first free spot to save the file*/
 	for(i = 0; i < 16; i++){
		if (directory[32*i] == 0){
			freeSpace = i;
			break;
		}
	}
  /*If no free spots found inform user*/
	if(freeSpace == -1){
		interrupt(33, 15, 2, 0, 0);
    return;
	}
  /*Put the filename onto the disk*/
	for(i=0; name[i] != '\0';i++){
		directory[32*freeSpace+i] = name[i];
	}
  /*Fill the file with null terminators if necessary*/
  for(i; i < 6; ++i){
    directory[32*freeSpace+i] = 0;
  }
  /*Write the contents to all the needed sectors*/
	for(i = 0; i < numberOfSectors; i++){
		sectorNum = 0;
		while(map[sectorNum] != 0){
			sectorNum++;
		}
		if(sectorNum==26){
			interrupt(33, 15, 2, 0, 0);
		}
    /*Set the map sector to taken*/
		map[sectorNum] = 0xFF;
    /*Let the directroy know the sector number that the file will take up*/
		directory[32*freeSpace+6+i] = sectorNum;
    /*Get all the sectors saved for the file 512 bits at a time*/
		for(j=0;j<512;j++){
			subBuff[j] = buffer[(i*512)+j];
		}
		writeSector(subBuff,sectorNum);
	}
  /*Update both the map and the directory*/
	writeSector(map,1);
	writeSector(directory,2);
}

/*This will try and load a file and then execute it*/
void runProgram(char* name, int segment){
  int i;
  char buffer[13312];
  segment = segment*4096;

  readFile(name, buffer, 0);
  if(buffer[0] == 0){
    return;
  }
  /*Put each piece of the data into memory*/
  for (i=0; i<13312;i++){
    putInMemory(segment,i,buffer[i]);
  }
  /*Execute the program*/
  launchProgram(segment);
}

/*This is a special case handler to simulate an end to a program*/
void stop(){
  launchProgram(8192);
}

/*This determines what function to call based off of the interrupt occurence*/
void handleInterrupt21(int ax, int bx, int cx, int dx){
  if(ax == 0)
    printString(bx);
  else if(ax == 1)
    readString(bx);
  else if(ax == 2)
    readSector(bx, cx);
  else if(ax == 3)
    readFile(bx, cx, dx);
  else if(ax == 4)
    runProgram(bx, cx);
  else if(ax == 5)
    stop();
  else if(ax == 6)
    writeSector(bx, cx);
  else if(ax == 7)
    deleteFile(bx);
  else if(ax == 8)
    writeFile(bx, cx, dx);
  else if(ax == 11)
    interrupt(25, 0, 0, 0, 0);
  else if(ax == 12)
    clearScreen(bx, cx);
  else if(ax == 13)
    writeInt(bx);
  else if(ax == 14)
    readInt(bx);
  else if(ax == 15)
    error(bx);
  else printString("AX was some value that could not be handled!\r\n\0");
  return;
}
