#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

/* ARGSUSED */
static void myhandler(int s) {
 char aster = '*';
 int errsave;
 errsave = errno;
 write(STDERR_FILENO, &aster, 1);
 errno = errsave;
}
static int setupinterrupt(void) { /* set up myhandler for SIGPROF */
 struct sigaction act;
 act.sa_handler = myhandler;
 act.sa_flags = 0;
 return (sigemptyset(&act.sa_mask) || sigaction(SIGPROF, &act, NULL));
}
static int setupitimer(void) { /* set ITIMER_PROF for 2-second intervals */
 struct itimerval value;
 value.it_interval.tv_sec = 2;
 value.it_interval.tv_usec = 0;
 value.it_value = value.it_interval;
 return (setitimer(ITIMER_PROF, &value, NULL));
}


int main(int argc, char** argv)
{
        pid_t childPid = fork(); // This is where the child process splits from the parent
        if (childPid == 0)
        {
                printf("I am a child but a copy of parent! My parent's PID is %d, and my PID is %d\n", getppid(), getpid());
                char* args[] = {"./worker", "Hello", "there", "exec", "is", "neat", 0};
                execvp(args[0], args);
                execlp(args[0],args[0],args[1],args[2],args[3],args[4],args[5],args[6]);
                fprintf(stderr,"Exec failed, terminating\n");
                exit(1);
        }
        else
        {
                printf("I'm a parent! My pid is %d, and my child's pid is %d \n",
                getpid(), childPid);
                sleep(1);
                wait(0);
        }
        printf("Parent is now ending.\n");
        return EXIT_SUCCESS;
        if (setupinterrupt() == -1) {
                perror("Failed to set up handler for SIGPROF");
                return 1;
        }
        if (setupitimer() == -1) {
                perror("Failed to set up the ITIMER_PROF interval timer");
                return 1;
        }
        for ( ; ; );
}
