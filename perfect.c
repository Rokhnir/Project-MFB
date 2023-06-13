#include <stdio.h>
#include <stdlib.h>

#define SIZE 24 // Taille de la matrice

void mirrorMatrix(int matrix[][SIZE]) {
    int temp;

    // Parcourir les éléments de la matrice en effectuant le miroir vertical
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE / 2; j++) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[i][SIZE - 1 - j];
            matrix[i][SIZE - 1 - j] = temp;
        }
    }
}
void try(int matrix[][SIZE]) {
    int temp;

    // Parcourir les éléments de la matrice en effectuant la rotation
    for (int i = 0; i < SIZE / 2; i++) {
        for (int j = i; j < SIZE - i - 1; j++) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][SIZE - 1 - i];
            matrix[j][SIZE - 1 - i] = matrix[SIZE - 1 - i][SIZE - 1 - j];
            matrix[SIZE - 1 - i][SIZE - 1 - j] = matrix[SIZE - 1 - j][i];
            matrix[SIZE - 1 - j][i] = temp;
        }
    }
}


void printMatrix(int matrix[][SIZE]) {
    // Afficher les éléments de la matrice
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Utilisation : %s <fichier>\n", argv[0]);
        return 1;
    }

    int matrix[SIZE][SIZE];
    FILE *file;

    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);

    mirrorMatrix(matrix);

    try(matrix);

    printMatrix(matrix);

    return 0;
}
