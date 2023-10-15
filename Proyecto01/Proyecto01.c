#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<malloc.h>
#include <time.h>

//Declarar estructuras
struct Carta;

struct Carta {
	char name[50];
	char desc[200];
	char class[50];
	int hp, ap, dp;
	struct Carta* next;
};

struct Player {
	char name[50];
	struct Carta* mazo;
	struct Carta* mano[3];
	int hp;
};

//Declarar funciones
int ContarCartas(struct Carta*);
struct Carta* SacarCartaMazo(struct Carta**);
struct Player* CrearJugador(char[]);
void AnadirCartaMazo(struct Carta**, struct Carta*);
void RepartirCartas(struct Carta* mazo, struct Carta** mazo1, struct Carta** mazo2, int maxCartas);
void LimpiarMazo(struct Carta**);
void Turno(struct Player*);
void TurnoBot(struct Player*);

//Crear Carta
struct Carta* CrearCarta(char name[], char desc[], char class[], int hp, int ap, int dp) {
	struct Carta* carta = malloc(sizeof(struct Carta));
	if (carta != NULL) {
		strcpy(carta->name, name);
		strcpy(carta->desc, desc);
		strcpy(carta->class, class);
		carta->hp = hp;
		carta->ap = ap;
		carta->dp = dp;
		carta->next = NULL;
	}
	return carta;
}

int main() {
	srand((unsigned int)time(NULL));
	char file_path[200];

	// --SECCION DE CODIGO EDITABLE-- //
	strcpy(file_path, "cards.csv"); //Remplazar "cards.csv" por la dirección donde guarde sus cartas

	//Abre el loop de juego
	bool game = true;
	int elec;
	do
	{
		int NofScannedArguments = 0; //Evitar warning fscan

		//Abre el archivo .csv
		FILE* file;
		file = fopen(file_path, "r");
		if (file == NULL) {
			printf("Error: No se ha podido abrir el archivo.");
			return 1;
		}

		//Crear Mazos
		struct Carta* mazo = NULL;
		char line[200];
		char name[50], desc[200], class[50];
		int hp, ap, dp;

		//Leer cartas desde el archivo
		fgets(line, 200, file);
		while (fgets(line, 200, file))
		{
			strcpy(name, strtok(line, ";"));
			strcpy(desc, strtok(NULL, ";"));
			strcpy(class, strtok(NULL, ";"));
			hp = atoi(strtok(NULL, ";"));
			ap = atoi(strtok(NULL, ";"));
			dp = atoi(strtok(NULL, ";"));
			AnadirCartaMazo(&mazo, CrearCarta(name, desc, class, hp, ap, dp));
		}
		fclose(file);

		//Ingresar Jugador
		printf("Bienvenido jugador");
		printf("\nEscriba su nombre: ");
		char nameplayer[50];
		NofScannedArguments = (int) scanf(" %[^\n]s", nameplayer);
		struct Player* Jugador = CrearJugador(nameplayer);
		fflush(stdin);
		printf("Jugador: %s", Jugador->name);

		//Ingresar Bot
		struct Player* Bot = CrearJugador("Bot");

		//Repartir Cartas
		struct Carta* mazo1 = NULL;
		struct Carta* mazo2 = NULL;
		RepartirCartas(mazo, &mazo1, &mazo2, 15);
		mazo = NULL;
		Jugador->mazo = mazo1;
		Bot->mazo = mazo2;

		//Añadir Cartas del mazo a la Mano
		for (int i = 0; i < 3; i++)
		{
			AnadirCartaMazo(&Jugador->mano[i], SacarCartaMazo(&Jugador->mazo));
			AnadirCartaMazo(&Bot->mano[i], SacarCartaMazo(&Bot->mazo));
		}

		//Preparar campo de batalla
		struct Carta* battlefield1 = NULL;
		struct Carta* battlefield2 = NULL;

		//Gameplay
		int turno = rand() % 2;
		system("cls");
		do
		{	
			//Mostrar campo de batalla
			for (int i = 0; i < ContarCartas(battlefield1); i++)
			{
				printf("__________ ");
			}
			printf("\n");
			for (int i = 0; i < ContarCartas(battlefield1); i++)
			{
				printf("|        | ");
			}
		} while (Jugador->hp > 0 && Bot->hp > 0);

		//Fin del juego
		free(Jugador);
		free(Bot);
		free(mazo);
		LimpiarMazo(&mazo2);
		LimpiarMazo(&mazo1);
		do
		{
			printf("\n¿Quieres volver a jugar?");
			printf("\n1- Si");
			printf("\n2- No");
			printf("\nEleccion: ");
			NofScannedArguments = (int) scanf("%i", &elec);
			if (elec == 1) {
				game = true;
			}
			if (elec == 2) {
				game = false;
			}
		} while (elec != 1 && elec != 2);
	} while (game);
	return 0;
}

//Añade una carta a un mazo (PUSH)
void AnadirCartaMazo(struct Carta** mazo, struct Carta* carta) {
	if (*mazo == NULL) {
		carta->next = NULL;
		*mazo = carta;
	}
	else {
		carta->next = *mazo;
		*mazo = carta;
	}
}

//Saca una carta de la pila (POP)
struct Carta* SacarCartaMazo(struct Carta** mazo) {
	struct Carta* carta = NULL;
	if (*mazo != NULL) {
		carta = *mazo;
		*mazo = (*mazo)->next;
		carta->next = NULL;
		return carta;
	}
	return NULL;
}

//Crea un jugador
struct Player* CrearJugador(char name[]) {
	struct Player* jugador = malloc(sizeof(struct Player));
	if (jugador != NULL) {
		strcpy(jugador->name, name);
		jugador->hp = 5;
		jugador->mazo = NULL;
		for (int i = 0; i < 3; i++)
		{
			jugador->mano[i] = NULL;
		}
	}
	return jugador;
}

//Contar cartas
int ContarCartas(struct Carta* mazo) {
	struct Carta* current = mazo;
	int count = 0;
	while (current != NULL) {
		count++;
		current = current->next;
	}
	return count;
}

//LimpiarMazo
void LimpiarMazo(struct Carta** mazo) {
	while (*mazo != NULL) {
		struct Carta* carta = SacarCartaMazo(mazo);
		free(carta);
	}
}

//Repartir Cartas
void RepartirCartas(struct Carta* mazo, struct Carta** mazo1, struct Carta** mazo2, int maxCartas) {

	int cartasRepartidas = 0;
	while (mazo != NULL && cartasRepartidas < maxCartas * 2) {
		int numeroCartas = ContarCartas(mazo);
		int cartaElegida = rand() % numeroCartas;
		struct Carta* carta = mazo;
		struct Carta* cartaAnterior = NULL;
		while (cartaElegida > 0) {
			cartaAnterior = carta;
			carta = carta->next;
			cartaElegida--;
		}
		if (cartaAnterior == NULL) {
			mazo = carta->next;
		}
		else {
			cartaAnterior->next = carta->next;
		}
		carta->next = NULL;
		if (cartasRepartidas % 2 == 0) {
			AnadirCartaMazo(mazo1, carta);
		}
		else {
			AnadirCartaMazo(mazo2, carta);
		}
		cartasRepartidas++;
	}
}

void Turno(struct Player* player) {
	
}

void TurnoBot(struct Player* player) {

}

void Atacar(struct Carta* atacante, struct Carta* defensor) {

}