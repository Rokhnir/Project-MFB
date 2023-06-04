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

int main() {
    int rows, cols;

    printf("Entrez le nombre de lignes de la matrice : ");
    scanf("%d", &rows);
    printf("Entrez le nombre de colonnes de la matrice : ");
    scanf("%d", &cols);

    // Allouer de l'espace pour la matrice
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }

    printf("Entrez les éléments de la matrice :\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

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
