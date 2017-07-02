#ifndef ABB_H
#define ABB_H
#include <stdbool.h>
#include <stddef.h>

// estructuras a utilizar 
typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

//crea el arbol vacio
//pos: arbol creado con referencia a raiz NULL, destructor pasado por parametro al igual que la función para comparar. 
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);
//borra el dato asociado a la clave
//pre: el arbol fue creado
//pos: si pertenecia al arbol el dato fue borrado, y tiene un elemento menos, si no returna NULL
void *abb_borrar(abb_t *arbol, const char *clave);

//obtiene el dato asociado a la clave
//pre: el arbol fue creado
//pos: devuelve el puntero al dato asociado a la clave
void *abb_obtener(const abb_t *arbol, const char *clave);

//devuelve verdadero si el dato está en el arbol, falso caso contrario
//pre: el arbol fue creado
bool abb_pertenece(const abb_t *arbol, const char *clave);
//guarda un dato en el arbol
//pre: el arbol fue creado
//pos: el dato fue guardado
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

//devuelve la cantidad del arbol
//pre: el arbol fue creado
size_t abb_cantidad(abb_t *arbol);

//destruye el arbol
//pre: el arbol fue creado
//pos: arbol destruido
void abb_destruir(abb_t *arbol);

typedef struct abb_iter abb_iter_t;

//crea un iterador
//pre: arbol creado
//pos: iterador creado
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);
//avanza el iterador
//pre: arbol e iterador creado
//pos: si no está al final, avanzó una posicion
bool abb_iter_in_avanzar(abb_iter_t *iter);
//
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
//devuelve true en caso de que esté al final, false caso contrario
bool abb_iter_in_al_final(const abb_iter_t *iter);
//destruye la pila
//pos: pila destruida
void abb_iter_in_destruir(abb_iter_t* iter);
#endif // ABB_H
