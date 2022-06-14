#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int cnt;

void newalarm(int pid, unsigned int sec){
	sleep(sec);
	int raise(int pid);
}

void myalarm(int signo){
	for (;;){
		printf("ding dong dang : %d\n", cnt ++);
		fflush(stdout);
		sleep(1);
	}
}

int main(void){
	int pid, status, signo;
	time_t t;

	sigset_t set;
	sigfillset(&set);
	sigdelset(&set, SIGALRM);
	sigdelset(&set, SIGINT);

	if ( sigprocmask(SIG_SETMASK, &set, NULL) == -1){
		printf("mask err\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGALRM, (void *)myalarm);

	if ((pid=fork()) < 0){
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid == 0){
		myalarm(1);
		pause();
	} else {
		newalarm(pid, 3);
		kill(pid, SIGINT);
		if ( waitpid(pid, &status, 0) < 0){
			perror("waitpid");
			exit(EXIT_FAILURE);
		} else {
			if ( WIFSIGNALED(status)){
				signo = WTERMSIG(status);

				if ( signo != SIGINT){
					perror("termsig");
					printf("child process killed by %s\n", strsignal(signo));
					exit(EXIT_FAILURE);
				}else {
					printf("Success\n");
				}
			}else{
				perror("ifsignaled");
				exit(EXIT_FAILURE);
			}
		}
	}
	return 0;
}
