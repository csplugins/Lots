// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:_____Cody__Skala_____________________ Date:__04/14/2016_

// 3460:426 Lab 4A - Basic C shell

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_LINE_LEN 80
#define WHITESPACE " ,\t\n"

struct command_t {
  char *name;
  int argc;
  char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);

int main(int argc, char *argv[]) {
  int pid;
  int status;
  char cmdLine[MAX_LINE_LEN];
  struct command_t command;

  /* Shell initialization */
  while(1){
    /*Show the prompt screen*/
    printPrompt();
    /*Read the command line and parse it*/
    readCommand(cmdLine);
    /*Put the command into separate arguments eith the parse function*/
    parseCommand(cmdLine, &command);
    /*Set the last argument to NULL for termination of the args*/
    command.argv[command.argc] = NULL;
    /*Before forking a child, if the parent sees Q, quit the parent shell*/
    if(command.name[0] == 'Q'){
      break;
    }
    /*Create a child process to execute the command*/
    if ((pid = fork()) == 0){
      /*Child executing command*/
      /*Copy command*/
      if(command.name[0] == 'C'){
        command.name = "cp\0";
      }
      /*Delete command*/
      else if(command.name[0] == 'D'){
        command.name = "rm\0";
      }
      /*Echo command*/
      else if(command.name[0] == 'E'){
        command.name = "echo\0";
      }
      /*If the command is Help, echo the help screen*/
      else if(command.name[0] == 'H'){
        command.name = "echo\0";
        command.argv[1] = "Welcome to my shell!\r\n\0";
        command.argv[2] = "\rHere is a list of compatible commands:\r\n\0";
        command.argv[3] = "C file1 file2\t\tCopies file1 to new file file2\r\n\0";
        command.argv[4] = "D file1\t\tRemoves file called file1\r\n\0";
        command.argv[5] = "E Some message.\tPrints \"Some message.\" to screen\r\n\0";
        command.argv[6] = "H\t\t\tDisplays this help page\r\n\0";
        command.argv[7] = "L\t\t\tList the contents of the current directory\r\n\0";
        command.argv[8] = "M file1\t\tCreate file file1 by launching a text editor\r\n\0";
        command.argv[9] = "P file1\t\tDisplay the contents of file1 on screen\r\n\0";
        command.argv[10] = "Q\t\t\tQuit the shell\r\n\0";
        command.argv[11] = "S\t\t\tSurf the web by launching a browser in the backgrond\r\n\0";
        command.argv[12] = "W\t\t\tClears the screen and wipes it\r\n\0";
        command.argv[13] = "X program1\t\tExecute a program called program1\r\n\0";
        command.argv[14] = NULL;
      }
      /*Print working directory command and use ls -l after*/
      else if(command.name[0] == 'L'){
        int pid2;
        printf("\r\n\0");
        if(pid2=fork() ==0){
          command.name = "pwd\0";
          execvp(command.name, command.argv);
        }
        /*Ensure pwd executes first*/
        wait(&status);
        printf("\r\n\0");
        command.name = "ls\0";
        command.argv[1] = "-l\0";
        command.argv[2] = NULL;
      }
      /*Make a file command*/
      else if(command.name[0] == 'M'){
        command.name = "nano\0";
      }
      /*Print contents of file command*/
      else if(command.name[0] == 'P'){
        command.name = "more\0";
      }
      /*Launch firefox command. This way will supress all warnings and works*/
      else if(command.name[0] == 'S'){
        command.name = "sh";
        command.argv[1] = "-c";
        //command.argv[2] = "firefox > /dev/null 2>&1 &";
        command.argv[2] = "firefox 2> /dev/null &";
        command.argv[3] = NULL;
      }
      /*Clear screen command*/
      else if(command.name[0] == 'W'){
        command.name = "clear\0";
      }
      /*Execute program command*/
      else if(command.name[0] == 'X'){
        int i;
        command.name = command.argv[1];
        /*If the command has arguments, set them*/
        for(i = 0; i < command.argc; ++i){
          command.argv[i] = command.argv[i+1];
        }
        /*Null terminate the last command*/
        command.argv[i] = NULL;
      }
      /*Execute the command name passing in the arguments*/
      execvp(command.name, command.argv);
      /*Have the child return to the parent to prevent multiple spawns*/
      return 0;
    }
    /*Wait for the child to terminate as long as it's not firefox*/
    if(command.name[0] != 'S'){
      wait(&status);
    }
  }

  /* Shell termination */
  printf("\n\n shell: Terminating successfully\n");
  return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
  int argc;
  char **clPtr;
  /* Initialization */
  clPtr = &cLine;/* cLine is the command line */
  argc = 0;
  cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
  /* Fill argv[] */
  while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
    cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
  }

  /* Set the command name and argc */
  cmd->argc = argc-1;
  cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
  strcpy(cmd->name, cmd->argv[0]);
  return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - pp. 79-80 */

void printPrompt(){
  /* Build the prompt string to have the machine name,
   * current directory, or other desired information
   */
  char promptString[16] = "linux (cws26)|>\0";
  printf("%s ", promptString);
}

void readCommand(char *buffer) {
  /* This code uses any set of I/O functions, such as those in
   * the stdio library to read the entire command line into
   * the buffer. This implementation is greatly simplified,
   * but it does the job.
   */
  fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */
