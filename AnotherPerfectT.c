#include <stdio.h>
#include <stdlib.h>

void mirrorMatrix(int** matrix, int rows, int cols) {
    int temp;

    // Parcourir les éléments de la matrice en effectuant le miroir vertical
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols / 2; j++) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[i][cols - 1 - j];
            matrix[i][cols - 1 - j] = temp;
        }
    }
}

void rotateMatrix(int** matrix, int rows, int cols) {
    int temp;
    int maxIndex = rows > cols ? rows : cols;

    // Parcourir les éléments de la matrice en effectuant la rotation
    for (int i = 0; i < maxIndex / 2; i++) {
        for (int j = i; j < maxIndex - i - 1; j++) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][maxIndex - 1 - i];
            matrix[j][maxIndex - 1 - i] = matrix[maxIndex - 1 - i][maxIndex - 1 - j];
            matrix[maxIndex - 1 - i][maxIndex - 1 - j] = matrix[maxIndex - 1 - j][i];
            matrix[maxIndex - 1 - j][i] = temp;
        }
    }
}

void printMatrix(int** matrix, int rows, int cols) {
    // Afficher les éléments de la matrice
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
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

    FILE *file;
    int rows = 0, cols = 0;
    int value;

    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    // Compter le nombre de lignes et de colonnes
    while ((value = fgetc(file)) != EOF) {
        if (value == '\n') {
            rows++;
        }
        else if (rows == 0 && value != ' ') {
            cols++;
        }
    }
    rows++; // Ajouter 1 pour la première ligne

    // Allouer de l'espace pour la matrice
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }

    // Retourner au début du fichier
    rewind(file);

    // Lire les valeurs de la matrice à partir du fichier
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);

    mirrorMatrix(matrix, rows, cols);

    rotateMatrix(matrix, rows, cols);

    printMatrix(matrix, rows, cols);

    // Libérer la mémoire allouée
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
