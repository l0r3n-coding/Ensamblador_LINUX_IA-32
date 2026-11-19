/*	Hacía mucho no tocabamos c eh?
	Lo usual, si se olvida como pushear el codigo:
	git add .
	git commit -m "comentario del commit"
	git push


*/

/* Módulo de lectura del archivo fuente
	Debe de:
	- Abrir un archivo fuente					| Ya
	- Leerlo línea por líneas					| Ya
	- Almacenar las líneas en memoria dinámica	| Ya
	- Conservar el número de la líneas			| Ya
*/

#include <stdio.h>
#include <stdlib.h>

void liberar_lineas(char **lineas, int n);						// Prototipo de la función que libera la memoria dinamica

int main(int argc, char *argv[])
{
	// Esta estructura es sacada de clase :D
	FILE *archivo;

	if (argc<2){
		printf("Error, faltan argumentos\n");
		return -1;
	}
	archivo = fopen(argv[1],"r");
	if(archivo == NULL){
		printf("No se pudo abrir el archivo\n");
		return -1;
	}

	char **lineas = NULL;			// Doble apuntador, almacena la direccion de cada cadena
	char *linea_actual = NULL;		// Apuntador a la línea que se está leyendo
	int n = 0;						// Contador del numero de linea
	size_t len = 0;					// Tamaño de la cadena
									// 	size_t es REQUERIDO por el getline de abajo, el cual es básicamente
									// 	un int de puros positivos. Se pensaba dejar int pero daba errores y pues.

	
																/* ¿Como funciona getline? Este, a grandes razgos, pide memoria nueva
																	mediante malloc, al pasarle linea_actual (la cual es NULL) y len
																	(el cual es 0), pide memoria mediante malloc para guardar la linea.
																	Además, la lee hasta que encuentra un \n, y cuando se queda sin lineas,
																	arroja un -1				*/
	while(getline(&linea_actual, &len, archivo)!= -1){			// No, no hay manera más sencilla, ESTA es la manera sencilla xd

		linea_actual[strcspn(linea_actual, "\n")] = '\0';		// Como estamos utilizando getline, es necesario quitarle el salto

		// lineas = realloc(lineas, (n+1)*sizeof(char*));		// Originalmente, esta era la linea, pero se tiene que preever que
																// realloc falle y llegue a devolver NULL, para eso se usa una var. temp.
																/*	realloc reajusta el arreglo de lineas (Como se dijo antes, arreglo de direcciones)
																	añadiendo otro espacio de dirección (char*). Además de que es multiplicado por el
																	numero de lineas hasta ahora, para aumentarlo de uno en uno			*/

		char **temp = realloc(lineas, (n+1)*sizeof(char*));		// Nueva línea que es identica, pero utilizando un doble apuntador temporal
		if (temp == NULL){										// Si de alguna manera llega a fallar en asignar memoria:
			perror("Error en REALLOC al asignar memoria!!");	// invitado especial perro
			liberar_lineas(lineas, n);							// Se manda a llamar la función que libera las variables
			return -1;											// Regresa
		}
		lineas = temp;											// Si no hubo error, ahora si, se pasa la linea al doble apuntador

		lineas[n] = linea_actual;								// Se copia la dirección de memoria donde getline guardó la linea leída, en la
		linea_actual = NULL;									//   posición actual del arreglo (por ejemplo, 0)
		len = 0													// Y reiniciamos todo
		n++;
	}
	free(linea_actual);											// Aparentemente getline deja una linea actual demás entonces se limpia

	// AQUI VA LA CONEXIÓN AL LEXER: no se como se hace xdd

	liberar_lineas(lineas, n);									// La misma función sirve para liberarlas, independiente de si hubo error
																// 	Y cierra el archivo, entonces no se cierra aquí
	return 0;
}

void liberar_lineas(char **lineas, int n){
	free(linea_actual);											// Libera la linea actual
	for (int i = 0; i < n; i++) free(lineas[i]);				// Libera cada valor del arreglo
	free(lineas);												// Libera el apuntador en si (Eso creo no me citen en esto)
	fclose(archivo);											// Cerramos el archivo por seguridad
	return;
}