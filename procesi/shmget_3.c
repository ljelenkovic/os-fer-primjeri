#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define MS_VEL	10

//zajednička memorija za Petersonov algoritam i problem proizvođača i potrošača
struct peter {
	int zastavica[2];
	int pravo;
	char buffer[MS_VEL+1];
	int ulaz, izlaz, punih;
} *p;

void udji_u_KO(int i)
{
	int j = 1 - i;
	p->zastavica[i] = 1;
	p->pravo = j;
	
	while (p->zastavica[j] == 1 && p->pravo == j)
		;
}
void izadji_iz_KO(int i)
{
	p->zastavica[i] = 0;
}

char ulaz[] = "0123456789abcdefghijklm";
size_t s = sizeof(ulaz) - 1;

void proizvodjac()
{
	for (size_t i = 0; i < s;) {
		udji_u_KO(0);
		if (p->punih != MS_VEL) {
			p->buffer[p->ulaz] = ulaz[i];
			printf("proizvodjac: stavio %c\t(ms=%s)\n", ulaz[i], p->buffer);
			p->ulaz = (p->ulaz + 1) % MS_VEL;
			i++;
			p->punih++;
		}
		izadji_iz_KO(0);
		sleep(1);
	}
}
void potrosac()
{
	for (size_t i = 0; i < s;) {
		udji_u_KO(1);
		if (p->punih > 0) {
			char c = p->buffer[p->izlaz];
			printf("potrosac: uzeo %c\t(ms=%s)\n", c, p->buffer);
			p->buffer[p->izlaz] = '-';
			p->izlaz = (p->izlaz + 1) % MS_VEL;
			i++;
			p->punih--;
		}
		izadji_iz_KO(1);
		sleep(2);
	}
}

int main ()
{
	int Id = shmget(IPC_PRIVATE, sizeof(struct peter), 0600);
	if (Id == -1) {
		fprintf(stderr, "Nema memorije!\n");
		exit(1);
	}

	p = shmat(Id, NULL, 0);
	shmctl(Id, IPC_RMID, NULL); //može odmah nakon shmat

	p->zastavica[0] = p->zastavica[1] = p->pravo = 0;
	p->ulaz = p->izlaz = p->punih = 0;
	memset(p->buffer, '-', MS_VEL);
	p->buffer[MS_VEL] = '\0';

	if (fork() == 0) {
		proizvodjac();
		exit(0);
	}
	if (fork() == 0) {
		potrosac();
		exit(0);
	}
	wait(NULL);
	wait(NULL);

	shmdt(p);

	return 0;
}
