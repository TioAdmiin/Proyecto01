#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

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
	struct Carta mazo;
	int hp;
};

//Declarar funciones
struct Carta* CrearCarta(char[], char[], char[], int, int, int);
struct Player* CrearJugador(char[]);
void PrintCarta(struct Carta*);


int main() {

	// --SECCION DE CODIGO EDITABLE-- //

	char file_path[50] = "cards.csv";

	//_______________________________//
	 

	//Abre el archivo .csv
	FILE* file;
	file = fopen(file_path, "r");
	if (file == NULL) {
		printf("Error: No se ha podido abrir el archivo.");
		return 1;
	}

	//Abre el loop de juego
	bool game = true;
	int elec;
	do
	{
		int read = 0;
		int records = 0;
		//Crear Mazo


		//Ingresar Jugador
		printf("Bienvenido jugador");
		printf("\nEscriba su nombre: ");
		char nameplayer[50];
		gets(nameplayer);
		struct Player* Jugador = CrearJugador(nameplayer);
		printf("Jugador: %s", Jugador->name);

		//Ingresar Bot
		struct Player* Bot = CrearJugador("Bot");


		//Fin del juego
		do
		{
			printf("\n¿Quieres volver a jugar?");
			printf("\n1- Si");
			printf("\n2- No");
			printf("\nEleccion: ");
			scanf("%i", &elec);
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


void AnadirCartaMazo(struct Carta* mazo, struct Carta* carta) {
	mazo->next = carta;
}

void PrintCarta(struct Carta* carta) {
	printf("\nCarta: %s",carta->name);
}

struct Player* CrearJugador(char name[]) {
	struct Player* jugador = malloc(sizeof(struct Player));
	if (jugador != NULL) {
		strcpy(jugador->name, name);
		jugador->hp = 5;
	}
	return jugador;
}