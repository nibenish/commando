#define _GNU_SOURCE 
#include "commando.h"

/*
typedef struct {
  cmd_t *cmd[MAX_CHILD];        // array of pointers to cmd_t
  int size;                     // number of cmds in the array
} cmdctl_t;
*/



/* 
 * Adds the given command to the control structure and updates the size
 */
void cmdctl_add(cmdctl_t *ctl, cmd_t *cmd) {
	          
  if (ctl->size < MAX_CHILD) {               // See if there is room for the command 	
    ctl->cmd[ctl->size] = cmd;				// Put command into array
    ctl->size += 1;   						// Increment size
  }
  else {
    printf("Maximum number of children reached");
  }
  return;
}



/* 
 * Prints out a header and each job with the following information
 * 
 * JOB  #PID      STAT   STR_STAT OUTB COMMAND
 * 0    #17434       0    EXIT(0) 2239 ls -l -a -F
 * 1    #17435       0    EXIT(0) 3936 gcc --help
 * 2    #17436      -1        RUN   -1 sleep 2
 * 3    #17437       0    EXIT(0)  921 cat Makefile
 * 
 * Widths of the fields and justification are as follows:
 * 
 * JOB  #PID      STAT   STR_STAT OUTB COMMAND
 * 1234  12345678 1234 1234567890 1234 Remaining
 * left  left    right      right rigt left
 * int   int       int     string  int string
 * 
 * The final field should be the contents of cmd->argv[] with a space
 * between each element of the array.
 */
void cmdctl_print(cmdctl_t *ctl) {
  printf("JOB  #PID      STAT   STR_STAT OUTB COMMAND\n");
  for(int i = 0; i < ctl->size; i ++) {
    cmd_t *cmd = ctl->cmd[i];
    int j = 0;
    char args[(ARG_MAX*2) * NAME_MAX];		// Create a buffer larger enough from ARG_MAX with NAME_MAX lengths, spaces and null terminator
    args[0] = '\0';							// Start with empty string
    while (cmd->argv[j] != NULL) {
		strcat(args, cmd->argv[j]);
		strcat(args, " ");
		j = j+1;
	}
    printf("%-4d #%-8d %4d %10s %4d %s\n",i, cmd->pid, cmd->status, cmd->str_status, cmd->output_size, args);
  }
  return;
}



/*
 * Updates each cmd in ctl by calling cmd_update_state() which is also
 * passed the block argument (either NOBLOCK or DOBLOCK) 
 */
void cmdctl_update_state(cmdctl_t *ctl, int block) {
	
	for(int i=0; i<ctl->size; i++) {
		cmd_update_state(ctl->cmd[i], block);
	}
	return;
}



/*
 * Frees all the commands located in the cmd array by calling 
 * cmd_free() on all the cmd_t's  
 */
void cmdctl_freeall(cmdctl_t *ctl) {
	
	for(int i=0; i<ctl->size; i++) {
		cmd_free(ctl->cmd[i]);
	}
	free(ctl);
	return;
}
