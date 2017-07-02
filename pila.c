#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
#define INICIAL 17
#define REDIM 2

bool redimension_pilas(pila_t* pila, size_t nuevo_tamanio){
	void* aux = realloc (pila->datos, (nuevo_tamanio * sizeof(void*)));
	if(aux == NULL){
		return false;
	}
	pila->datos = aux;
	pila->capacidad= nuevo_tamanio;
	return true;
}

pila_t* pila_crear(){
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL) {
		return NULL;
	}
	pila->datos = malloc(INICIAL * sizeof(void*));
	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}
	pila->capacidad = INICIAL;
	pila->cantidad = 0;
	return pila;
}

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}


void* pila_ver_tope(const pila_t *pila){
	if (!pila_esta_vacia(pila)){
		return pila->datos[pila->cantidad-1];
	}
	return NULL;
}

void* pila_desapilar(pila_t *pila){
	if(!pila_esta_vacia(pila)){
		if ( pila->cantidad < (pila->capacidad / 4) ){
			redimension_pilas(pila, pila->capacidad/REDIM);
		}
		pila->cantidad--;
		return pila->datos[pila->cantidad];
	}
	return NULL;
}

bool pila_apilar(pila_t *pila, void* valor){
	if (pila->cantidad == pila->capacidad){
		if (!redimension_pilas(pila, pila->capacidad*REDIM)){
			return false;
		}
	}
	pila->datos[pila->cantidad]=valor;
	pila->cantidad++;
	return true;
}
