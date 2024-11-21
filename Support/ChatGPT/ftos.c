#include <stdio.h>
#include <stdlib.h>

char* FUNCftos(FILE* stream) {
	int i, block = 8, NBytes = 0;
	char caracter;
	char* value = NULL;

	value = (char*)malloc(block * sizeof(char)); // Initial allocation
	if (value == NULL) {
		perror("ftos at malloc");
		return NULL; // Return NULL on allocation failure
	}

	for (i = 0; (caracter = getc(stream)) != EOF; i++) {
		if (i == NBytes) {
			NBytes += block; // Increase the size
			char* new_value = (char*)realloc(value, NBytes * sizeof(char));
			if (new_value == NULL) {
				free(value); // Free the original memory
				perror("ftos at realloc");
				return NULL; // Return NULL on allocation failure
			}
			value = new_value; // Update the pointer
		}
		value[i] = caracter; // Store the character
	}

	value[i] = '\0'; // Null-terminate the string

	return value;
}
