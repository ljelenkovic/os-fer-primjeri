#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void obradi(int sig)
{
	printf("Pocetak obrade signala %d\n", sig);
	printf("Kraj obrade signala %d\n", sig);
}

int main()
{
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0; /* naprednije mogucnosti preskocene */
	act.sa_handler = obradi;
	sigaction(SIGINT, &act, NULL);

	//alarm(10);
	printf("Program s PID=%ld krenuo s radom\n", (long) getpid());
	sleep(100);
	printf("nakon prvog sleepa\n");
	sleep(10);
	printf("nakon drugog sleepa\n");

	printf("Program s PID=%ld zavrsio s radom\n", (long) getpid());

	return 0;
}



