#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

typedef struct example {
    int *coordinates;
    int *coordinates_number;

} example;

typedef struct centroid {
    int *coordinates;
    int *old_coordinates;
    int coordinates_number;

} centroid;

int arrays_are_equals(int array_a[], int array_b[], int elements_number) {
    for (int i = 0; i < elements_number; i++) {
        if (array_a[i] != array_b[i]) {
            return 0;
        }
    }
    return 1;
}

int centroids_are_equals(centroid centroid[], int centroids_number) {
    for (int i = 0; i < centroids_number; i++) {
        if (!arrays_are_equals(centroid[i].coordinates, centroid[i].old_coordinates, centroid[i].coordinates_number))
            return 0;
    } 
    return 1;
}

void k_means(centroid centroids[], int centroids_number) {
    do {
        //TODO K-Means
    } while (!centroids_are_equals(centroids, centroids_number));
}

void array_copy(int array_a[], int array_b[], int elements_number) {
    for (int i = 0; i < elements_number; i++) {
        array_b[i] = array_a[i];
    }
}

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
    fclose(file);
    return count_lines;
}

void test_centroids_arrays() {

    centroid c_a, c_b;
    centroid array_c[2];

    int a[3] = {1, 2, 3};
    int b[3];

    array_copy(a, b, 3);

    c_a.coordinates = a;
    c_a.coordinates_number = 3;
    c_a.old_coordinates = b;
    
    c_b.coordinates = a;
    c_b.coordinates_number = 3;
    c_b.old_coordinates = b;

    array_c[0] = c_a;
    array_c[1] = c_b;

    if (arrays_are_equals(a, b, 3)){
        printf("\nIguais\n");
    }

    if (centroids_are_equals(array_c, 2)) {
        printf("Todos os centroids estão iguais\n");
    }
}

int main(int argc, char *argv[]) {
    int opt;
    int *vet_aux;
    char read;
    char *example_filename, *centroid_filename, *aux;
    unsigned long n_attr, n_examples, n_centroids, i, n_lines;
    size_t len;
    example *examples;
    centroid *centroids;
    FILE *ptr;

    example_filename = argv[1];
    centroid_filename = argv[2];
    n_attr = atoi(argv[3]);

    examples = (example *) malloc(count_lines(example_filename) * sizeof(example));
    printf("Alocou exemplos.\n");
    centroids = (centroid *) malloc(count_lines(centroid_filename) * sizeof(centroid));
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
        examples[n_examples - 1].coordinates = vet_aux;
    }
    fclose(ptr);

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
        centroids[n_centroids - 1].coordinates = vet_aux;
    }
    fclose(ptr);

    // Print examples for test
    for(int i = 0; i < n_examples; i++) {
        for(int j = 0; j < n_attr; j++) {
            printf("%d|", examples[i].coordinates[j]);
        }
        printf("\n");
    }

    return 0;
}
