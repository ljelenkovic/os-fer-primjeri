#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void obradi(int sig)
{
	int i;
	printf("Pocetak obrade signala %d\n", sig);
	for (i = 1; i <= 5; i++) {
		printf("Obrada signala %d: %d/5\n", sig, i);
		sleep(1);
	}
	printf("Kraj obrade signala %d\n", sig);
}

int main()
{
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0; /* naprednije mogucnosti preskocene */
	act.sa_handler = obradi;
	sigaction(SIGINT, &act, NULL);

	printf("Program s PID=%ld krenuo s radom\n", (long) getpid());

	/* neki posao koji program radi; ovdje samo simulacija */
	int i = 1;
	while (1) {
		printf("Program: iteracija %d\n", i++);
		sleep(1);
	}

	printf("Program s PID=%ld zavrsio s radom\n", (long) getpid());

	return 0;
}
