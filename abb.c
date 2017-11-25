#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE
#include "abb.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pila.h"
typedef struct nodo_abb {
	char* clave;
	void* dato;
	struct nodo_abb* izq;
	struct nodo_abb* der;	
}nodo_abb_t;


struct abb {
	nodo_abb_t* raiz;
	abb_destruir_dato_t destructor;
	size_t cantidad;
	abb_comparar_clave_t cmp;
};

struct abb_iter {
	pila_t* pila;
};
/*******auxiliares***********/

nodo_abb_t* nodo_crear(const char *clave, void *dato){
	nodo_abb_t* nodo = calloc(1,sizeof(nodo_abb_t));
    char* aux = strdup(clave);
    nodo->clave = aux;
    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

void nodo_guardar(abb_t *a, nodo_abb_t* actual, nodo_abb_t* a_insertar){
	if (a->cmp(actual->clave, a_insertar->clave) < 0){
		if(!actual->der){
			a->cantidad++;
			actual->der = a_insertar;
		}else{
			nodo_guardar(a,actual->der,a_insertar);
		}

	}else if (a->cmp(actual->clave, a_insertar->clave) > 0){
		if(!actual->izq){
			a->cantidad++;
			actual->izq = a_insertar;
		}else{
			nodo_guardar(a,actual->izq,a_insertar);
		}
	}else if(a->cmp(actual->clave, a_insertar->clave) == 0){
		if(a->destructor){
			void* a_destruir = actual->dato;
			a->destructor(a_destruir);
		}
		actual->dato = a_insertar->dato;
		free(a_insertar->clave);
		free(a_insertar);
	}
	return;
}

nodo_abb_t* busqueda(abb_comparar_clave_t cmp, nodo_abb_t* actual, const char* clave){
	if(!actual){
		return NULL;
	}
	if (cmp(clave,actual->clave) == 0){
		return actual;
	} else if(cmp(clave,actual->clave) > 0){
		nodo_abb_t* buscado = busqueda(cmp,actual->der, clave);
		return buscado;
	}else if(cmp(clave,actual->clave) < 0){
		nodo_abb_t* buscado = busqueda(cmp,actual->izq, clave);
		return buscado;
	}
	return NULL;
}
nodo_abb_t* buscar_padre_min(nodo_abb_t* nodo) {
	nodo_abb_t* aux = nodo->izq;
	if (!aux){
		return nodo;
	}
	if(!aux->izq){
		return nodo;
	}
	return buscar_padre_min(nodo->izq);
}

nodo_abb_t* borrar_sin_hijos(abb_t* a, nodo_abb_t *actual, nodo_abb_t* padre){
	nodo_abb_t* a_borrar = actual;
	if (padre->izq == actual){
		padre->izq = NULL;
	}
	if(padre->der == actual){
		padre->der = NULL;
	}
	if(a->raiz == a_borrar){
		a->raiz = NULL;
	}
	return a_borrar;
}

nodo_abb_t* borrar_con_1hijo(abb_t* a, nodo_abb_t *actual, nodo_abb_t* padre){
	nodo_abb_t* a_borrar = actual;	
	if(actual->izq && !actual->der){
		actual = actual->izq;
		if (padre->izq == a_borrar){
			padre->izq = actual;
		}
		if(padre->der == a_borrar){
			padre->der = actual;
		}
		if(a->raiz == a_borrar){
			a->raiz = actual;
		}
	}else if (actual->der && !actual->izq){
		actual= actual->der;
		if (padre->izq == a_borrar){
			padre->izq = actual;
		}
		if(padre->der == a_borrar){
			padre->der = actual;
		}
		if(a->raiz == a_borrar){
			a->raiz = actual;
		}
	}
	return a_borrar;
}

nodo_abb_t* nodo_borrar(abb_t* a, nodo_abb_t *actual, nodo_abb_t* padre, const char* clave){
	nodo_abb_t* a_borrar = NULL;	
	if (!actual){
		return NULL;
	}
	if (a->cmp(actual->clave, clave) == 0){
		if (!actual->izq && !actual->der){
			return borrar_sin_hijos(a,actual,padre);
		}else if (actual->der && actual->izq){
//			char* clave_a_borrar = actual->clave;
			void* dato_a_devolver = actual->dato;
			a_borrar = actual;
			nodo_abb_t* buscado = buscar_padre_min(actual->der);
			if(!buscado->izq){
				char* aux = strdup(actual->clave);
				free(actual->clave);
				actual->clave = actual->der->clave;
				actual->dato = actual->der->dato;
				actual->der->clave = aux;
				actual->der->dato = dato_a_devolver;
				a_borrar = nodo_borrar(a,actual->der,actual,clave);
			}else if(buscado->izq){
				char* aux = strdup(actual->clave);
				free(actual->clave);
				actual->clave = buscado->izq->clave;
				actual->dato = buscado->izq->dato;
				buscado->izq->clave = aux;
				buscado->izq->dato = dato_a_devolver;
				a_borrar = nodo_borrar(a,buscado->izq,buscado, clave);
			}
			return a_borrar;
		}else if(actual->der || actual->izq){
			return borrar_con_1hijo(a,actual,padre);
		}
	}
	padre = actual;
	if(a->cmp(clave, actual->clave) > 0){
		a_borrar = nodo_borrar(a,actual->der,padre,clave);
	}
	padre = actual;
	if(a->cmp(clave, actual->clave) < 0){
		a_borrar = nodo_borrar(a,actual->izq,padre,clave);
	}
	return a_borrar;
}

void nodo_destruir(nodo_abb_t* actual,abb_destruir_dato_t destruir_dato){
	if(actual == NULL){
		return;
	}
	if(destruir_dato){
		void* a_borrar = actual->dato;
		destruir_dato(a_borrar);
	}
	nodo_destruir(actual->izq,destruir_dato);
	nodo_destruir(actual->der,destruir_dato);
	free(actual->clave);
	free(actual);
	return;
}

/******primitivas arbol******/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){

	abb_t* abb = calloc(1,sizeof(abb_t));
	if (!abb) return NULL;
	abb->cantidad = 0;
	abb->destructor = destruir_dato;
	abb->cmp = cmp;
	return abb;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cantidad;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	nodo_abb_t* a_guardar = nodo_crear(clave,dato);
	if(!a_guardar){
		return false;
	}
	if (abb_cantidad(arbol) == 0){
		arbol->cantidad++;
		arbol->raiz = a_guardar;
		return true;
	}else{
		nodo_guardar(arbol, arbol->raiz, a_guardar);
	}
	return true;
}

bool abb_pertenece(const abb_t *arbol,const char* clave){
	if(!arbol->raiz){
		return false;
	}
	nodo_abb_t* buscado = busqueda(arbol->cmp, arbol->raiz,clave);
	if(!buscado){
		return false;
	}
	return true;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	nodo_abb_t* buscado = busqueda(arbol->cmp, arbol->raiz,clave);
	if(!buscado){
		return NULL;
	}
	return buscado->dato;
}

void* abb_borrar(abb_t* a, const char* clave) {
 	if (!a->raiz){
 		return NULL;
 	}
 	nodo_abb_t* padre = a->raiz;
	nodo_abb_t* buscado = nodo_borrar(a,a->raiz,padre,clave);
	if(!buscado){
		return NULL;
	}
	void* dato = buscado->dato;
	free(buscado->clave);
	free(buscado);
	a->cantidad--;
	return dato;
}

void abb_destruir(abb_t *arbol){
	if(abb_cantidad(arbol) > 0){
		nodo_destruir(arbol->raiz, arbol->destructor);
	}
	free(arbol);
	return;
}



/****primitivas iterador********/


abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {

	abb_iter_t* iterador = calloc(1,sizeof(abb_iter_t));
	if (!iterador){
		return NULL;
	}
	pila_t* aux2 = pila_crear();	
	if (!aux2){
		return NULL;
	}
	iterador->pila = aux2;
	nodo_abb_t* aux = arbol->raiz;
	bool fin = false;// a partir de hasta lo siguiente que marque es la modificacion para el tp2
	while (!fin && aux) {
		int comparacion = arbol->cmp(aux->clave,inicio);
		if(comparacion == 0){
			pila_apilar(iterador->pila,aux);
			fin = true;
		}else if(comparacion > 0){
			pila_apilar(iterador->pila,aux);
			aux = aux->izq;
		}else{
			aux = aux->der;
		}
	}// hasta aca despues en tu .h podes cambiar la firma de esta funcion por abb_iter_t *abb_iter_in_crear(const abb_t *arbol, char* inicio);
	/*while (aux) {
		pila_apilar(iterador->pila,aux); esto comentado es el iterador sin cambios
		aux = aux->izq;
	}*/	
	return iterador;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {	
	if(!abb_iter_in_al_final(iter)){
		nodo_abb_t*	 aux = pila_desapilar(iter->pila);
		if(aux->der) {
			pila_apilar(iter->pila,aux->der);
			aux = aux->der;
			while (aux->izq){
				pila_apilar(iter->pila, aux->izq);
				aux = aux->izq;
			}
		}
	}
	else {
		return false;
	}
	return true;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(iter->pila);
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if(!pila_esta_vacia(iter->pila)){
		nodo_abb_t* nodo = pila_ver_tope(iter->pila);
		char* aux = nodo->clave;
		return aux;
	}
	return NULL;
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}


bool recorrido(nodo_abb_t* nodo, bool visitar(const char *, void *, void *), void* extra){
	if(!nodo){
		return true;
	}
	if(!recorrido(nodo->izq, visitar,extra) || !visitar(nodo->clave, nodo->dato, extra)){
		return false;
	}
	return recorrido(nodo->der,visitar,extra);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if (abb_cantidad(arbol) == 0){
		return;
	}
	if(!recorrido(arbol->raiz->izq,visitar,extra)){
		return;
	}
	if(!visitar(arbol->raiz->clave, arbol->raiz->dato,extra)){
		return;
	}
	if(!recorrido(arbol->raiz->der,visitar,extra)){
		return;
	}
	return;
}
