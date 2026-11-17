/* Módulo de lectura del archivo fuente
	Debe de:
	- Abrir un archivo fuente					| Ya
	- Leerlo línea por líneas					| Ya
	- Almacenar las líneas en memoria dinámica	| Ya?
	- Conservar el número de la líneas			| Ya
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	// Esta estructura es sacada de clase :D
	FILE *archivo;
	char caracter[];

	if (argc<2){
		printf("Error, faltan argumentos\n");
		return -1;
	}
	archivo = fopen(argv[1],"r");
	if(archivo == NULL){
		printf("No se pudo abrir el archivo\n");
		return -1;
	}

	char **lineas;			// Doble apuntador para almacenar todas las líneas
	char *linea_actual;		// Apuntador a la línea que se está leyendo
	int len = 0;			// Tamaño de la cadena
	int n = 1;				// Contador de los caracteres

	/* ¿Como funciona getline? Este, a grandes razgos, pide memoria nueva
		mediante malloc, al pasarle linea_actual (la cual es NULL) y len
		(el cual es 0), pide memoria mediante malloc para guardar la linea.
		Además, la lee hasta que encuentra un \n
	*/
	while(getline(&linea_actual, &len, archivo)){

		lineas = realloc(lineas, (nL+1)*sizeof(char*));

		lineas[n] = linea_actual;
		linea_actual = NULL;
		n++;
	}


	fclose(archivo);

	return 0;
}