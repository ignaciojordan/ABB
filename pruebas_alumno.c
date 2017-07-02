#include "abb.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> //para el random
#define VOLUMEN 45000


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void pruebas_abb_vacio(){
	printf("PRUEBAS ABB VACIO\n");
	abb_t* arbol = abb_crear(strcmp,NULL);
	char* clave = "2";
	print_test("crear abb", arbol != NULL);
	print_test("cantidad 0",abb_cantidad(arbol) == 0);
	print_test("borrar vacio", abb_borrar(arbol,clave) == NULL);
	print_test("obtener vacio",abb_obtener(arbol,clave) == NULL);
	print_test("no pertenece clave en arbol vacio",!abb_pertenece(arbol,clave));
	abb_destruir(arbol);
}

void pruebas_abb_varias(){
	printf("PRUEBAS ABB VARIAS\n");
	abb_t* arbol2 = abb_crear(strcmp,NULL);
	char* clave2 = "11";
	int dato2 = 120;
	char* clave3 = "15";
	int dato3 = 32;
	char* clave4 = "13";
	int dato4 = 42;
	char* clave5 = "16";
	int dato5 = 32112;
	char* clave6 = "10";
	int dato6 = 4434;
	char* clave7 = "12";
	int dato7= 4434;
	print_test("crear abb", arbol2 != NULL);
	print_test("agrego clave 2 con dato 2",abb_guardar(arbol2,clave2,&dato2));
	print_test("cantidad ahora es 1",abb_cantidad(arbol2) == 1);
	print_test("agrego clave 3 con dato 3",abb_guardar(arbol2,clave3,&dato3));
	print_test("cantidad ahora es 2",abb_cantidad(arbol2) == 2);
	print_test("agrego clave 4 con dato 4",abb_guardar(arbol2,clave4,&dato4));
	print_test("agrego clave 5 con dato 5",abb_guardar(arbol2,clave5,&dato5));
	print_test("agrego clave 6 con dato 6",abb_guardar(arbol2,clave6,&dato6));
	print_test("obtener dato 3 con clave 3 da ok",abb_obtener(arbol2,clave3) == &dato3);
	print_test("obtener dato 2 con clave 2 da ok",abb_obtener(arbol2,clave2) == &dato2);
	print_test("clave 2 pertenece al arbol2 da ok", abb_pertenece(arbol2,clave2));
	print_test("clave 3 pertenece al arbol2 da ok", abb_pertenece(arbol2,clave3));
	print_test("cantidad ahora es 5",abb_cantidad(arbol2) == 5);
	print_test("guardo dato 5 con clave 4 que ya pertence al arbol2",abb_guardar(arbol2,clave4,&dato5));
	print_test("obtener dato cambiado con su clave da ok",abb_obtener(arbol2,clave4) == &dato5);
	print_test("cantidad sigue siendo 5",abb_cantidad(arbol2) == 5);
	abb_iter_t* iter = abb_iter_in_crear(arbol2);
	print_test("iterador está al final es false", !abb_iter_in_al_final(iter));
	print_test("ver actual iterador es clave6",strcmp(abb_iter_in_ver_actual(iter),clave6) == 0);
	print_test("iter avanzar",abb_iter_in_avanzar(iter));
	print_test("ver actual es clave 2",strcmp(abb_iter_in_ver_actual(iter),clave2) == 0);
	print_test("iter al final sigue siendo false",!abb_iter_in_al_final(iter));
	print_test("iter avanzar",abb_iter_in_avanzar(iter));
	print_test("ver actual iterador es clave4",strcmp(abb_iter_in_ver_actual(iter),clave4) == 0);
	print_test("iter avanzar",abb_iter_in_avanzar(iter));
	print_test("ver actual iterador es clave3",strcmp(abb_iter_in_ver_actual(iter),clave3) == 0);
	print_test("iter avanzar",abb_iter_in_avanzar(iter));
	print_test("ver actual iterador es clave5",strcmp(abb_iter_in_ver_actual(iter),clave5) == 0);
	print_test("iter avanzar",abb_iter_in_avanzar(iter));
	print_test("iter está al final",abb_iter_in_al_final(iter));
	print_test("pertenece dato invalido",!abb_pertenece(arbol2,"2"));
	print_test("agrego clave 7 con dato 7",abb_guardar(arbol2,clave7,&dato7));
	print_test("borrar clave6",abb_borrar(arbol2,clave2) == &dato2);

	abb_destruir(arbol2);
	abb_iter_in_destruir(iter);
}

void pruebas_abb_alumno() {
	pruebas_abb_vacio();
	pruebas_abb_varias();

}