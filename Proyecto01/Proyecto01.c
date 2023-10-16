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
	struct Carta* mano;
	int hp;
};

//Declarar funciones
int ContarCartas(struct Carta*);
struct Carta* SacarCartaMazo(struct Carta**);
struct Player* CrearJugador(char[]);
struct Carta* ObtenerCarta(struct Carta**, int);
void AnadirCartaMazo(struct Carta**, struct Carta*);
void RepartirCartas(struct Carta* mazo, struct Carta** mazo1, struct Carta** mazo2, int maxCartas);
void LimpiarMazo(struct Carta**);
void Turno(struct Player*, struct Carta**, struct Carta**, struct Player**);
void TurnoBot(struct Player*, struct Carta**, struct Carta**, struct Player**);
void MostrarCampo(struct Carta*);
void MostrarMano(struct Carta*);
void Atacar(struct Carta**, struct Carta**);

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
			AnadirCartaMazo(&Jugador->mano, SacarCartaMazo(&Jugador->mazo));
			AnadirCartaMazo(&Bot->mano, SacarCartaMazo(&Bot->mazo));
		}

		//Preparar campo de batalla
		struct Carta* battlefield1 = NULL;
		struct Carta* battlefield2 = NULL;

		//Gameplay
		int turno = rand() % 2;
		bool juego = true;
		do
		{
			
			if (Jugador->hp == 0 | Bot->hp == 0) {
				juego = false;
			}
			system("cls");
			printf("CAMPO DE BATALLA ENEMIGO:\n");
			MostrarCampo(battlefield1);
			printf("-------------------------------------------------\n");
			printf("CAMPO DE BATALLA ALIADO:\n");
			MostrarCampo(battlefield2);
			printf("-------------------------------------------------\n");
			if (turno % 2 == 0) {
				AnadirCartaMazo(&Jugador->mano, SacarCartaMazo(&Jugador->mazo));
				Turno(Jugador, &battlefield2, &battlefield1, &Bot);
			}
			else {
				AnadirCartaMazo(&Bot->mano, SacarCartaMazo(&Bot->mazo));
				TurnoBot(Bot, &battlefield1, &battlefield2, &Jugador);
			}
			turno++;
		} while (juego);

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
	if (carta == NULL) {
		return;
	}
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
		jugador->mano = NULL;
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

void Turno(struct Player* player, struct Carta** campo, struct Carta** campoenemigo, struct Player** bot) {
	int elec;
	int nowarning;
	bool atacado = false, dejado = false;
	do
	{
		if ((*bot)->hp <= 0) {
			system("cls");
			printf("FELICIDADES, HAS GANADO!\n");
			return;
		}
		printf("ES TU TURNO:\nEscoge una accion:\n1)Dejar una carta en el campo de batalla.\n2)Atacar\n3)Terminar Turno\nEleccion: ");
		nowarning = (int) scanf(" %i", &elec);
		if (elec == 1) {
			if (dejado) {
				printf("Ya dejaste una carta\n");
			}
			else {
				if (ContarCartas(player->mano) != 0) {
					int selec = -1;
					MostrarMano(player->mano);
					printf("0) Cancelar\n\n");
					do
					{
						printf("Escoge una carta:\n");
						nowarning = scanf(" %i", &selec);
					} while (selec < 0 || selec > ContarCartas(player->mano));
					if (selec != 0) {
						AnadirCartaMazo(campo, ObtenerCarta(&player->mano, selec));
						dejado = true;
					}
					system("cls");
					printf("CAMPO DE BATALLA ENEMIGO:\n\n");
					MostrarCampo(*campoenemigo);
					printf("-------------------------------------------------\n\n");
					printf("CAMPO DE BATALLA ALIADO:\n\n");
					MostrarCampo(*campo);
					printf("-------------------------------------------------\n");
				}
				else {
					printf("No tienes cartas en tu mano\n");
				}
			}
		}
		if (elec == 2) {
			if (atacado) {
				printf("Ya atacaste este turno.\n");
			}
			else {
				int selecc = -1;
				int selecc2 = -1;
				if (ContarCartas(*campo) != 0) {
					MostrarMano(*campo);
					do
					{
						printf("Escoge una carta: ");
						nowarning = scanf(" %i", &selecc);
					} while (selecc < 0 || selecc > ContarCartas(*campo));
					atacado = true;
					if (ContarCartas(*campoenemigo) == 0) {
						(*bot)->hp--;
					}
					else {
						ContarCartas(*campo);
						MostrarMano(*campoenemigo);
						do
						{
							printf("Escoge una carta para atacar: ");
							nowarning = scanf(" %i", &selecc2);
						} while (selecc2 < 0 || selecc2 > ContarCartas(*campoenemigo));
					}
				}
				else {
					printf("No hay cartas en el mazo de batalla\n");
				}
			}
		}
	} while (elec != 3);
}

void TurnoBot(struct Player* player, struct Carta** campo, struct Carta** campoenemigo, struct Player** enemigo) {
	//Primero deja una carta en el campo
	AnadirCartaMazo(campo, player->mano);
	//Luego ataca con la carta que deja
	if (ContarCartas(*campoenemigo) == 0) {
		(*enemigo)->hp--;
	}
	else {
		//Ataca a la carta con menor vida + defensa
		Atacar(campo, campoenemigo);
	}
}

//Mostrar Campo de batalla
void MostrarCampo(struct Carta* campo) {
	//Mostrar campo de batalla
	if (campo == NULL) {
		printf("El campo de batalla esta vacio.\n\n");
		return;
	}
	int contador = 1;
	struct Carta* current = campo;
	while (current != NULL) {
		printf("%i) %s: %s - [HP: %i|AP: %i|DP: %i]\n", contador, current->name, current->class, current->hp, current->ap, current->dp);
		printf("%s\n\n", current->desc);
		current = current->next;
		contador++;
	}
}

//Mostrar las cartas de la mano del juegador
void MostrarMano(struct Carta* mano) {
	printf("TU MANO:\n");
	int contador = 1;
	if (ContarCartas(mano) == 0) {
		printf("No quedan cartas en tu mano\n");
		return;
	}
	struct Carta* current = mano;
	while (current != NULL) {
		printf("%i) %s: %s - [HP: %i|AP: %i|DP: %i]\n", contador, current->name, current->class, current->hp, current->ap, current->dp);
		printf("\"%s\"\n\n", current->desc);
		current = current->next;
		contador++;
	}
}

//Eliminar una carta con un indice
void EliminarCarta(struct Carta** mano, int indice) {
	if (*mano == NULL) {
		return;
	}
	if (indice == 1) {
		struct Carta* temp = *mano;
		*mano = (*mano)->next;
		free(temp);
		return;
	}
	struct Carta* prev = *mano;
	struct Carta* current = (*mano)->next;
	int i = 2;
	while (current != NULL && i < indice) {
		prev = current;
		current = current->next;
		i++;
	}
	if (current == NULL) {
		return;
	}
	prev->next = current->next;
	free(current);
}

//Sacar una carta con un indice
struct Carta* ObtenerCarta(struct Carta** mano, int indice) {
	if (*mano == NULL) {
		return NULL;
	}
	if (indice == 1) {
		struct Carta* temp = *mano;
		*mano = (*mano)->next;
		temp->next = NULL;
		return temp;
	}
	struct Carta* prev = *mano;
	struct Carta* current = (*mano)->next;
	int i = 2;
	while (current != NULL && i < indice) {
		prev = current;
		current = current->next;
		i++;
	}
	if (current == NULL) {
		return NULL;
	}
	prev->next = current->next;
	current->next = NULL;
	return current;
}

//Obtener una carta con un indice
struct Carta* SelecCarta(struct Carta* campo, int indice) {
	if (campo == NULL) {
		return NULL;
	}
	struct Carta* current = campo;
	int i = 1;
	while (current != NULL && i < indice) {
		current = current->next;
		i++;
	}
	if (current == NULL) {
		return NULL;
	}
	return current;
}

void Atacar(struct Carta** atacante, struct Carta** defensor) {
	if (*atacante == NULL || *defensor == NULL) {
		return;
	}
	if ((*atacante)->ap > (*defensor)->dp) {
		(*defensor)->hp -= (*atacante)->ap - (*defensor)->dp;
	}
}