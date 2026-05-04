#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int Id; /* identifikacijski broj segmenta */

//globalne varijable:
struct ZS1 {
	int prvi;
	double drugi;
} *zv1;
int *treci;

void pr1() //prvi proces
{
	zv1->prvi = 1;
	*treci = *treci + 1;
}
void pr2() //drugi proces
{
	zv1->drugi = 1.;
	sleep(1);
	*treci = *treci + 1;
}

int main() {
	Id = shmget(IPC_PRIVATE, sizeof(struct ZS1)+sizeof(int), 0600);
	//               key        size                         flags

	if (Id == -1) {
		fprintf(stderr, "Nema memorije!\n");
		exit(1);
	}

	//povezivanje segmenta na adresni prostor procesa
	zv1 = (struct ZS1 *) shmat(Id, NULL, 0);
	shmctl(Id, IPC_RMID, NULL);

	treci = (int *)(zv1 + 1); //odmah iza zv1

	//inicijalizacija podataka u zajedničkom spremniku
	zv1->prvi = 0;
	zv1->drugi = 0;
	*treci = 0;

	if (!fork()) { pr1(); exit(0); }
	if (!fork()) { pr2(); exit(0); }
	wait(NULL);
	wait(NULL);

	printf("prvi=%d, drugi=%g, treci=%d\n", zv1->prvi, zv1->drugi, *treci);

	shmdt(zv1);

	return 0;
}
