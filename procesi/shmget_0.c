#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int Id; /* identifikacijski broj segmenta */

//globalna varijabla - kazaljka na zajedničku memoriju (treba postaviti)
int *a;

#define PROCESA		10
#define N		10000000

void proces()
{
	for (int i = 0; i < N; i++)
		*a = *a + 1;
	exit(0);
}

int main() {
	//zauzimanje segmenta zajedničke memorije
	Id = shmget(IPC_PRIVATE, sizeof(int), 0600);
	//               key      size        flags

	if (Id == -1) {
		fprintf(stderr, "Nema memorije!\n");
		exit(1);
	}

	//povezivanje segmenta na adresni prostor procesa
	a = shmat(Id, NULL, 0);
	shmctl(Id, IPC_RMID, NULL);

	*a = 0;

	for (int i = 0; i < PROCESA; i++)
		if (fork() == 0)
			proces();

	for (int i = 0; i < PROCESA; i++)
		wait(NULL);

	printf("a=%d\n", *a);

	shmdt(a);

	return 0;
}
