#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int euclidean_distance(int *example, int *centroid, int n_attr) {
    int distance = 0;

    for(int i = 0; i < n_attr; i++) {
        distance += (example[i] - centroid[i]) * (example[i] - centroid[i]);
    }
    distance = sqrt(distance);
    return distance;
}

void show_help() {
    fprintf(stderr, "-h      Ajuda.\n-e      Nome do arquivo de exemplos.\n-c      Nome do arquivo de centroides.\n-a      Quantidade de atributos dos exemplos e dos centroídes.\n");
}

int main(int argc, char *argv[]) {
    int opt;
    char *example_filename, *centroid_filename;
    int n_attr;
    FILE *ptr;

    example_filename = argv[1];
    centroid_filename = argv[2];
    n_attr = atoi(argv[3]);

    return 0;
}
