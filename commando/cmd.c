#include "commando.h"

/*
typedef struct {
  char   name[NAME_MAX+1];         // name of command like "ls" or "gcc"
  char  *argv[ARG_MAX+1];          // argv for running child, NULL terminated
  pid_t  pid;                      // PID of child
  int    out_pipe[2];              // pipe for child output
  int    finished;                 // 1 if child process finished, 0 otherwise
  int    status;                   // return value of child, -1 if not finished
  char   str_status[STATUS_LEN+1]; // describes child status such as RUN or EXIT(..)
  void  *output;                   // saved output from child, NULL initially
  int    output_size;              // number of bytes in output
} cmd_t;
*/


/*
 * Initalizes all the fields to defaults values, returns a pointer to a new cmd_t
 */
cmd_t *cmd_new(char *argv[]) {
	
	// Allocate memory for the new command
	cmd_t *new = malloc(sizeof(cmd_t));
	
	// Get the name of the command
	strncpy(new->name, argv[0], NAME_MAX);
	
	// Copy arguments into new->argv[]
	for (int j=0; j<ARG_MAX+1; j++ ) {
		new->argv[j] = '\0';
	}
	int i = 0;
	while (argv[i] != NULL) {
		new->argv[i] = strdup(argv[i]);
		i = i+1;
	}
	
	// Set default values
	new->pid = -1;
	new->out_pipe[PREAD] = 0;
	new->out_pipe[PWRITE] = 0;
	new->finished = 0;
	new->status = -1;
	snprintf(new->str_status,STATUS_LEN+1,"INIT");
	new->output = NULL;
	new->output_size = -1;
	

	return new;
	
}



/*
 * Deallocates the cmd_t structure
 * 	1. Deallocates the strings in the argv[]
 * 	2. Deallocate the buffer if it is not null
 * 	3. Deallocate itself
 */
void cmd_free(cmd_t *cmd) {
	
	for (int i=0; i < ARG_MAX; i++) {
		if (cmd->argv[i] != NULL) {
			free(cmd->argv[i]);
		}
	}
	
	
    if (cmd->output){ 
		free(cmd->output); 
	}
    free(cmd);
    
    return;
}



/*
 * Forks a process and starts the command in it
 */
void cmd_start(cmd_t *cmd) {
	
    pipe(cmd->out_pipe);                            // Create a pipe for out_pipe to capture standard input
    
    snprintf(cmd->str_status, STATUS_LEN+1, "RUN");	// Change the str_status to "RUN" using snprintf()

    cmd->pid = fork();                              // Ensure that the pid field is set to child PID
    if(cmd->pid == 0) { // Child process
		dup2(cmd->out_pipe[PWRITE], STDOUT_FILENO); // Redirect standard input to our pipe
		close(cmd->out_pipe[PREAD]);
		execvp(cmd->name, cmd->argv);               // Start the new process with the output directed to pipe
    }
    else { // Parent process
		close(cmd->out_pipe[PWRITE]);
    }
    
    return;
}


/*
 * Reads all input from an open file descriptor
 */
char *read_all(int fd, int *nread){

    int i = 1;
    int bytes_read = 0;
	char *buffer = malloc(sizeof(char)*BUFSIZE);
	
    bytes_read = read(fd, buffer, BUFSIZE);			// Initial read into buffer
    
    while (bytes_read == (BUFSIZE *i)) {
		i = i*2;
		char *temp = realloc(buffer,(sizeof(char)*(BUFSIZE*i)));
		bytes_read += read(fd, &temp[bytes_read], (BUFSIZE*i)-bytes_read);
		buffer = temp;
    }

	*nread = bytes_read;
	buffer[*nread] = '\0';
	return buffer;
}



/*
 * If a cmd is not finished yet print a message, otherwise read all output from pipe
 */
void cmd_fetch_output(cmd_t *cmd) {
	
	if(cmd->finished == 0){
		printf("%s[#%d] not finished yet", cmd->name, cmd->pid);
	} 
	else {
		cmd->output = read_all(cmd->out_pipe[PREAD], &cmd->output_size);
	}
  
  return;
}



/*
 * Print the output of the cmd if it is not null, if it is null print a message
 */
void cmd_print_output(cmd_t *cmd) {
	
	printf("@<<< Output for %s[#%d] (%d bytes):\n", cmd->name, cmd->pid, cmd->output_size);
	printf("----------------------------------------\n");
	
	if (cmd->output == NULL) {
		printf("%s[#%d] has no output yet\n", cmd->name, cmd->pid);
	} 
	else {
		printf("%s", (char *)cmd->output);
	}
	
	printf("----------------------------------------\n");
	
	return;
}



/*
 * Update the state of cmd
 */
void cmd_update_state(cmd_t *cmd, int nohang) {
		
	if (cmd->finished == 1) {
		//do nothing
	} 
	else {
		int temp = waitpid(cmd->pid, &cmd->status, nohang);
		if (temp >= 0) {
			if (WIFEXITED(cmd->status) != 0) {
				cmd->status = WEXITSTATUS(cmd->status);
				cmd->finished = 1;
				snprintf(cmd->str_status, STATUS_LEN+1, "EXIT(%d)", cmd->status);
				cmd_fetch_output(cmd);
				printf("@!!! %s[#%d]: %s\n", cmd->name, cmd->pid, cmd->str_status);
			}
		}
	}
	
	return;
}


  

