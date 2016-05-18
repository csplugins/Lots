/*Header to make typing a little easier*/
#include "blackdos.h"

/*Prototype to run a given command*/
void checkCmd(char*);

/*Constantly get input from the user and try to execute their command*/
void main(){
  while(1){
    char cmd[13312];
    interrupt(33, 0, "cxxxx][===blackdos===>\0", 0, 0);
    SCANS(cmd);
    checkCmd(cmd);
  }
}

void checkCmd(char* cmd){
  /*If command is boot*/
  if(cmd[0] == 'b' && cmd[1] == 'o' && cmd[2] == 'o' && cmd[3] == 't')
    interrupt(33, 11, 0, 0, 0);
  /*If command is cls*/
  else if(cmd[0] == 'c' && cmd[1] == 'l' && cmd[2] == 's')
    interrupt(33, 12, 5, 16, 0);
  /*If command is copy*/
  else if(cmd[0] == 'c' && cmd[1] == 'o' && cmd[2] == 'p' && cmd[3] == 'y'){
    int i = 5;
    int j = 0;
    int size = 0;
    char file1[50];
    char file2[50];
    char buffer[13312];
    /*Some arguments are missing*/
    if (cmd[4] == '\0'){
      interrupt(33,0,"Usage:\r\ncopy file1 file2\r\n\0",0,0);
      return;
    }
    /*Copy should be followed by a space*/
    else if(cmd[4] != ' '){
      interrupt(33,0,"Bad command or file name.\r\n\0",0,0);
      return;
    }
    /*Get the first file name*/
    while(cmd[i] != ' '){
      /*Noticed null terminator before getting second file name*/
      if(cmd[i] == '\0'){
        interrupt(33,0,"Usage:\r\ncopy file1 file2\r\n\0",0,0);
        return;
      }
      file1[j] = cmd[i];
      i++;
      j++;
    }
    /*Filenames can only be 6 characters, ignore anything else*/
    file1[j] = '\0';
    file1[6] = '\0';
    i++;
    j = 0;
    /*Get the second file name*/
    while(cmd[i] != '\0' && cmd[i] !=' '){
      file2[j] = cmd[i];
      i++;
      j++;
    }
    /*Make sure the new file name will not start uppercase*/
    if(file2[0] >= 'A' && file2[0] <= 'Z'){
      interrupt(33,0,"Error. Cannot create files starting with capital letters!\r\n\0",0,0);
      return;
    }
    /*Filenames can only be 6 characters, ignore anything else*/
    file2[j] = '\0';
    file2[6] = '\0';
    /*Call readFile interrupt to load the contents into buffer*/
    interrupt(33,3,file1,buffer,&size);
    /*Only make the file if the original file to copy was found*/
    /*This uses wrtieFile interrupt to save te contents of buffer*/
    if(buffer[0] != 0)
		  interrupt(33,8,file2,buffer,size);
  }
  /*If command is del*/
  else if(cmd[0] == 'd' && cmd[1] == 'e' && cmd[2] == 'l'){
    char filename[7];
    char buffer[13312];
    int size;
    int i;
    /*Some arguments are missing*/
    if (cmd[3] == '\0'){
      interrupt(33,0,"Usage:\r\ndel filename\r\n\0",0,0);
      return;
    }
    /*del should be followed by a space*/
    else if(cmd[3] != ' '){
      interrupt(33,0,"Bad command or file name.\r\n\0",0,0);
      return;
    }
    /*Set the filename to whatever follows del*/
    for(i = 4; i < 10; ++i){
      filename[i-4] = cmd[i];
    }
    /*Make sure the user is not trying to delete an uppercase file*/
    if(filename[0] >= 'A' && filename[0] <= 'Z'){
      interrupt(33,0,"Error. Cannot delete files starting with capital letters!\r\n\0",0,0);
      return;
    }
    /*Force the null terminator at the end*/
    filename[6] = '\0';
    /*Call the delete interrupt*/
    interrupt(33, 7, filename, buffer, &size);
  }
  /*If command is dir*/
  else if(cmd[0] == 'd' && cmd[1] == 'i' && cmd[2] == 'r'){
    char directorySector[512];
    int i, j, count;
    int totalCount = 0;
    /*Load the directory sector*/
    interrupt(33, 2, directorySector, 2, 0);
    /*Display all the files and their size (in sectors)*/
    for(i = 0; i < 16; ++i){
      if(directorySector[i*32] != 0){
        j = 0;
        count = 0;
        /*Only display the files not starting uppercase*/
        if(directorySector[i*32] < 'A' || directorySector[i*32] > 'Z'){
          while(directorySector[i*32+j] != '\0' && j < 6){
            interrupt(16,14*256+directorySector[i*32+j],0,0,0);
            j++;
          }
        }
        /*Get the size of the file*/
        for(j = 6; j < 32; ++j){
          if(directorySector[i*32+j] != 0){
            count++;
          }
          else break;
        }
        totalCount += count;
        /*Continue showing the file size only if it starts not uppercase*/
        if(directorySector[i*32] < 'A' || directorySector[i*32] > 'Z'){
          interrupt(33, 0, " -> \0", 0, 0);
          interrupt(33,13,count,0,0);
          interrupt(33, 0, " sectors(\0", 0, 0);
          interrupt(33,13,count*512,0,0);
          interrupt(33, 0, " bytes)\r\n\0", 0, 0);
        }
      }
    }
    /*Display total free and total used sectors*/
    interrupt(33, 0, "\r\nTotal sectors used -> \0", 0, 0);
    interrupt(33,13,totalCount + 3,0,0);
    interrupt(33, 0, "\r\nTotal sectors free -> \0", 0, 0);
    interrupt(33,13,256 - totalCount - 3,0,0);
    interrupt(33, 0, "\r\n\0", 0, 0);
    interrupt(33, 0, "\r\n\0", 0, 0);
  }
  /*If command is echo*/
  else if(cmd[0] == 'e' && cmd[1] == 'c' && cmd[2] == 'h' && cmd[3] == 'o'){
    char filename[13312];
    int i;
    int j = 5;
    /*Some arguments are missing*/
    if (cmd[4] == '\0'){
      interrupt(33,0,"Usage:\r\necho comment\r\n\0",0,0);
      return;
    }
    /*echo should be followed by a space*/
    else if(cmd[4] != ' '){
      interrupt(33,0,"Bad command or file name.\r\n\0",0,0);
      return;
    }
    /*Put the comment from echo into a file buffer*/
    for(i = 5; i < 13311; ++i){
      if((cmd[i] == ' ' && (cmd[i+1] == ' ' || cmd[i+1] == '\t')) ||
        (cmd[i] == '\t' && (cmd[i+1] == ' ' || cmd[i+1] == '\t'))){
        ++j;
        continue;
      }
      else filename[i-j] = cmd[i];
    }
    /*Null terminate the buffer*/
    filename[13311] = '\0';
    /*Put the contents to screen using printString interrupt*/
    interrupt(33,0,filename,0,0);
    interrupt(33,0,"\r\n\0",0,0);
  }
  /*If command is help*/
  else if(cmd[0] == 'h' && cmd[1] == 'e' && cmd[2] == 'l' && cmd[3] == 'p'){
    /*Call this function to jump to showing the contents of Help file*/
    checkCmd("type Help");
  }
  /*If command is run*/
  else if(cmd[0] == 'r' && cmd[1] == 'u' && cmd[2] == 'n'){
    char filename[50];
    int i;
    /*Some arguments are missing*/
    if(cmd[3] == '\0'){
      interrupt(33,0,"Usage:\r\nrun filename\r\n\0",0,0);
      return;
    }
    /*run should be follwoed by a space*/
    else if(cmd[3] != ' '){
      interrupt(33,0,"Bad command or file name.\r\n\0",0,0);
      return;
    }
    /*Put the filename into its own buffer*/
    for(i = 4; i < 10; ++i){
      filename[i-4] = cmd[i];
    }
    /*Make sure the filename does not start with capital letter*/
    if(filename[0] >= 'A' && filename[0] <= 'Z'){
      interrupt(33,0,"Error. Cannot run files starting with capital letters!\r\n\0",0,0);
      return;
    }
    /*Null terminate the end of the file*/
    filename[6] = '\0';
    /*Call run program interrupt to start the program*/
    interrupt(33,4,filename,4,0);
  }
  /*If command is tweet*/
  else if(cmd[0] == 't' && cmd[1] == 'w' && cmd[2] == 'e' && cmd[3] == 'e' && cmd[4] == 't'){
    int i;
    char filename[50];
    char msg[1024];
    /*Some arguments missing*/
    if (cmd[5] == '\0'){
      interrupt(33,0,"Usage:\r\ntweet filename\r\n\0",0,0);
      return;
    }
    /*tweet should be followed by a space*/
    else if(cmd[5] != ' '){
      interrupt(33,0,"Bad command or file name.\r\n\0",0,0);
      return;
    }
    /*Put the filename into a buffer*/
    for(i = 6; i < 12; ++i)
      filename[i - 6] = cmd[i];
    /*Make sure the filename does nto start with an uppercase letter*/
    if(filename[0] >= 'A' && filename[0] <= 'Z'){
      interrupt(33,0,"Error. Cannot create files starting with capital letters!\r\n\0",0,0);
      return;
    }
    /*Force the last character to be null terminated on the filename*/
    filename[6] = '\0';
    interrupt(33,0,"Enter a message less than 140 characters:\r\n>\0",0,0);
    /*Read in the message*/
    SCANS(msg);
    /*Force the last character to be null terminated on the message(less than 140 chars)*/
    msg[139] = '\0';
    /*Call writeFile interrupt to save the contents to a new file*/
    interrupt(33,8,filename,msg,1);
  }
  /*If command is type*/
  else if(cmd[0] == 't' && cmd[1] == 'y' && cmd[2] == 'p' && cmd[3] == 'e'){
    int i, size;
    char buffer[13312];
    char filename[7];
    /*Missign some paameters*/
    if (cmd[4] == '\0'){
      interrupt(33,0,"Usage:\r\ntype filename\r\n\0",0,0);
      return;
    }
    /*tpye should be followed by a space*/
    else if(cmd[4] != ' '){
      interrupt(33,0,"Bad command or file name.\r\n\0",0,0);
      return;
    }
    /*Save the filename to a buffer*/
    for(i = 5; i < 11; ++i){
      filename[i-5] = cmd[i];
    }
    /*Null terminate the last char of the filename*/
    filename[6] = '\0';
    /*Call the readFile interrupt to get the contents of the file stored to buffer*/
    interrupt(33,3,filename,buffer,&size);
    /*Print the contents of the buffer*/
    interrupt(33,0,buffer,0,0);
    interrupt(33,0,"\r\n\0",0,0);
  }
  /*Command was not found*/
  else{
    interrupt(33,0,"Bad command or file name.\r\n\0",0,0);
  }
}
