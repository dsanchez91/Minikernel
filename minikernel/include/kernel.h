/*
 *  minikernel/include/kernel.h
 *
 *  Minikernel. Versi�n 1.0
 *
 *  Fernando P�rez Costoya
 *
 */

/*
 *
 * Fichero de cabecera que contiene definiciones usadas por kernel.c
 *
 *      SE DEBE MODIFICAR PARA INCLUIR NUEVA FUNCIONALIDAD
 *
 */

#ifndef _KERNEL_H
#define _KERNEL_H

#define NO_RECURSIVO 0
#define RECURSIVO 1
#include "const.h"
#include "HAL.h"
#include "llamsis.h"

/*
 *
 * Definicion del tipo que corresponde con el BCP.
 * Se va a modificar al incluir la funcionalidad pedida.
 *
 */
typedef struct BCP_t *BCPptr;

struct tiempos_ejec {
    int usuario;
    int sistema;
};

typedef struct BCP_t {
        int id;				/* ident. del proceso */
        int estado;			/* TERMINADO|LISTO|EJECUCION|BLOQUEADO*/
	int sleep_time;			/* Tiempo dormido*/	
	struct tiempos_ejec num_int;	/* N�mero interrupciones */ 
        contexto_t contexto_regs;	/* copia de regs. de UCP */
        void * pila;			/* dir. inicial de la pila */
	BCPptr siguiente;		/* puntero a otro BCP */
	void *info_mem;			/* descriptor del mapa de memoria */
	int id_mutex[NUM_MUT_PROC];
	int rodaja;			/*rodaja de round robin*/
} BCP;

/*
 *
 * Definicion del tipo que corresponde con la cabecera de una lista
 * de BCPs. Este tipo se puede usar para diversas listas (procesos listos,
 * procesos bloqueados en sem�foro, etc.).
 *
 */

typedef struct{
	BCP *primero;
	BCP *ultimo;
} lista_BCPs;


int dentro_zona=0;

typedef struct mutex {
	int creado;
	char nombre[MAX_NOM_MUT];
	int tipo;
	int ocupado;
	int pid_dueno;
};

struct mutex lista_mutex[NUM_MUT];

typedef struct{
	int ocupacion;
	int caracter[TAM_BUF_TERM];
} buffer;


buffer buffer_lectura;


/*
 * Variable global que identifica el proceso actual
 */

BCP * p_proc_actual=NULL;

/*
 * Variable global que representa la tabla de procesos
 */

BCP tabla_procs[MAX_PROC];

/*
 * Variable global que representa la cola de procesos listos
 */
lista_BCPs lista_listos= {NULL, NULL};
lista_BCPs lista_bloqueados= {NULL, NULL};
lista_BCPs lista_bloqueados_lect= {NULL, NULL};

int replanificar=0;
long int num_int = 0;

/*
 *
 * Definici�n del tipo que corresponde con una entrada en la tabla de
 * llamadas al sistema.
 *
 */
typedef struct{
	int (*fservicio)();
} servicio;


/*
 * Prototipos de las rutinas que realizan cada llamada al sistema
 */
int sis_crear_proceso();
int sis_terminar_proceso();
int sis_escribir();
int obtener_id_pr();
int dormir();
int tiempos_proceso();
int leer_caracter();
int crear_mutex();
int abrir_mutex();
int lock();
int unlock();
int cerrar_mutex();


/*
 * Variable global que contiene las rutinas que realizan cada llamada
 */
servicio tabla_servicios[NSERVICIOS]={	{sis_crear_proceso},
					{sis_terminar_proceso},
					{sis_escribir},
					{obtener_id_pr},
					{dormir},
					{tiempos_proceso},
					{leer_caracter}};

#endif /* _KERNEL_H */

