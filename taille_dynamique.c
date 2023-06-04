#include <stdio.h>
#include <stdlib.h>

void getMapDimensions(const char* filename, int* height, int* width) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    int ch;
    int tempWidth = 0;
    int rowCount = 0;
    int maxWidth = 0;

    // Calcul de la largeur et de la hauteur de la carte
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            rowCount++;
            if (tempWidth > maxWidth) {
                maxWidth = tempWidth;
            }
            tempWidth = 0;
        } else if (ch != ' ') {
            tempWidth++;
        }
    }

    *height = rowCount;
    *width = maxWidth;

    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Veuillez fournir le nom du fichier en argument.\n");
        return 1;
    }

    const char* filename = argv[1];
    int height, width;
    getMapDimensions(filename, &height, &width);
    printf("Hauteur : %d\n", height);
    printf("Largeur : %d\n", width);

    return 0;
}
