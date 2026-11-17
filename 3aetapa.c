/* Módulo de lectura del archivo fuente
	Debe de:
	- Abrir un archivo fuente					| Ya
	- Leerlo línea por líneas					| Ya
	- Almacenar las líneas en memoria dinámica	| Ya
	- Conservar el número de la líneas			| Ya
*/

#include <stdio.h>
#include <stdlib.h>

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

	char **lineas;				// Doble apuntador, almacena la direccion de cada cadena
	char *linea_actual = NULL;	// Apuntador a la línea que se está leyendo
	int len = 0;				// Tamaño de la cadena
	int n = 0;					// Contador del numero de linea

	/* ¿Como funciona getline? Este, a grandes razgos, pide memoria nueva
		mediante malloc, al pasarle linea_actual (la cual es NULL) y len
		(el cual es 0), pide memoria mediante malloc para guardar la linea.
		Además, la lee hasta que encuentra un \n, y cuando se queda sin lineas,
		arroja un -1				*/
	while(getline(&linea_actual, &len, archivo)!= -1){
		lineas = realloc(lineas, (n+1)*sizeof(char*));
		/*	realloc reajusta el arreglo de lineas (Como se dijo antes, arreglo de direcciones)
			añadiendo otro espacio de dirección (char*). Además de que es multiplicado por el
			numero de lineas hasta ahora, para aumentarlo de uno en uno			*/

		// Se copia la dirección de memoria donde getline guardó la linea leída, en la
		//   posición actual del arreglo (por ejemplo, 0)
		lineas[n] = linea_actual;
		linea_actual = NULL;
		n++;
	}


	fclose(archivo);

	return 0;
}