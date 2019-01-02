#include "commando.h"


int main(int argc, char *argv[]){

  // Variables
  setvbuf(stdout, NULL, _IONBF, 0); 	// Turn off output buffering
  int echo = 0;							// Echo condition
  int run = 1; 							// While loop condition
  char **tokens;						// Arguments to pass to parse_into_tokens
  int *ntok; 							// Contains the total number of tokens
  cmdctl_t *mission_control; 			// Pointer to command control structure


  // Set echo variable
  if (argc > 1){
    if (strcmp(argv[1],"--echo")==0){		// "--echo" flag is passed
      echo = 1;
    }
  }
  else if (getenv("COMMANDO_ECHO")) {		// "COMMANDO_ECHO" environment variable is set
	echo = 1;  
  }
  
  // Initialize cmdctrl structure
  mission_control = (cmdctl_t *) malloc(sizeof(cmdctl_t));
  mission_control->size = 0;
    
  
  // Initialize tokens and ntok
  tokens = (char **) malloc(sizeof(char) * ARG_MAX);
  ntok = (int *) malloc(sizeof(int));
  

  // Main Loop
  while(run){

    char buffer[MAX_LINE];
    printf("@> ");
    if (fgets(buffer, MAX_LINE, stdin) != NULL) {

    if(echo == 1) {
      printf("%s",buffer);
    }

    parse_into_tokens(buffer, tokens, ntok);
    

    // User input entered
	if (tokens[0] != NULL) {
				
		// "help" command	
		if (strcmp(tokens[0],"help") == 0){
			printf("COMMANDO COMMANDS\n");
			printf("help               : show this message\n");
			printf("exit               : exit the program\n");
			printf("list               : list all jobs that have been started giving information on each\n");
			printf("pause nanos secs   : pause for the given number of nanseconds and seconds\n");
			printf("output-for int     : print the output for given job number\n");
			printf("output-all         : print output for all jobs\n");
			printf("wait-for int       : wait until the given job number finishes\n");
			printf("wait-all           : wait for all jobs to finish\n");
			printf("command arg1 ...   : non-built-in is run as a job\n");
		}

		// "exit" command
		else if (strcmp(tokens[0],"exit") == 0) {
			run = 0;
			break;
		}

		// "list" command
		else if (strcmp(tokens[0],"list") == 0) {
			cmdctl_print(mission_control);
		}

		// "pause" command
		else if (strcmp(tokens[0],"pause") == 0) {
			int nanos = atoi(tokens[1]);
			int seconds = atoi(tokens[2]);
			pause_for(nanos, seconds);
		}

		// "output-for" command
		else if (strcmp(tokens[0],"output-for") == 0) {
			int index = atoi(tokens[1]);
			cmd_print_output(mission_control->cmd[index]);
		}

		// "output-all" command
		else if (strcmp(tokens[0],"output-all") == 0) {
			for (int i=0; i < mission_control->size; i++) {
				cmd_print_output(mission_control->cmd[i]);
			}
		}

		// "wait-for" command
		else if (strcmp(tokens[0],"wait-for") == 0) {
			int index = atoi(tokens[1]);
			cmd_update_state(mission_control->cmd[index], DOBLOCK);
		}

		// "wait-all" command
		else if (strcmp(tokens[0],"wait-all") == 0){
			cmdctl_update_state(mission_control, DOBLOCK);
		}

		// Create a new cmd
		else {
			cmd_t *temp = cmd_new(tokens);  
			cmdctl_add(mission_control, temp);
			cmd_start(mission_control->cmd[mission_control->size-1]);
		}
      
		cmdctl_update_state(mission_control, NOBLOCK);		// Updates the status of all commands with NOBLOCK
      
		}
	}
	else {
		printf("\nEnd of input\n");
		run = 0;
		break;
	}	

  } // End while loop
  
  free(ntok);
  free(tokens);
  
  cmdctl_freeall(mission_control);
  return 0;
}
