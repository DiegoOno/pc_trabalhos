#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

typedef struct{
    int *example;
    int centroid_index;
} EXAMPLE;

typedef struct{
    int *centroid;
} CENTROID;

int euclidean_distance(int *example, int *centroid, int n_attr) {
    int distance = 0;

    for(int i = 0; i < n_attr; i++) {
        distance += (example[i] - centroid[i]) * (example[i] - centroid[i]);
    }
    distance = sqrt(distance);
    return distance;
}

// void show_help() {
//     fprintf(stderr, "-h      Ajuda.\n-e      Nome do arquivo de exemplos.\n-c      Nome do arquivo de centroides.\n-a      Quantidade de atributos dos exemplos e dos centroídes.\n");
// }

int count_lines(char *file_name) {
    int count_lines = 0;
    char c;
    FILE *file;

    file = fopen(file_name, "r");  
    c = getc(file);
    while(c != EOF) {
        if(c == '\n') {
            count_lines++;
        }
        c = getc(file);
    }
    close(file);
    return count_lines;
}

int main(int argc, char *argv[]) {
    int opt;
    int *vet_aux;
    char read;
    char *example_filename, *centroid_filename, *aux;
    unsigned long n_attr, n_examples, n_centroids, i, n_lines;
    size_t len;
    EXAMPLE *examples;
    CENTROID *centroids;
    FILE *ptr;

    example_filename = argv[1];
    centroid_filename = argv[2];
    n_attr = atoi(argv[3]);

    examples = (EXAMPLE *) malloc(count_lines(example_filename) * sizeof(EXAMPLE));
    printf("Alocou exemplos.\n");
    centroids = (CENTROID *) malloc(count_lines(centroid_filename) * sizeof(CENTROID));
    printf("Alocou centroides.\n");

    //Get data of examples file
    ptr = fopen(example_filename, "r");
    n_examples = 0;
    while((read = getline(&aux, &len, ptr)) != -1) {
        aux = strtok(aux, ",");
        n_examples++;
        i = 0;
        vet_aux = malloc(n_attr * sizeof(int));
        while(aux != NULL) {
            vet_aux[i] = atoi(aux);
            aux = strtok (NULL, " ,.-");
            i++;
        }
        examples[n_examples - 1].example = vet_aux;
    }
    close(ptr);

    //Get data of centroids file
    ptr = fopen(centroid_filename, "r");
    n_centroids = 0;
    while((read = getline(&aux, &len, ptr)) != -1) {
        aux = strtok(aux, ",");
        n_centroids++;
        i = 0;
        vet_aux = malloc(n_attr * sizeof(int));
        while(aux != NULL) {
            vet_aux[i] = atoi(aux);
            aux = strtok (NULL, " ,.-");
            i++;
        }
        centroids[n_centroids - 1].centroid = vet_aux;
    }
    close(ptr);

    // Print examples for test
    for(int i = 0; i < n_examples; i++) {
        for(int j = 0; j < n_attr; j++) {
            printf("%d|", examples[i].example[j]);
        }
        printf("\n");
    }

    return 0;
}
