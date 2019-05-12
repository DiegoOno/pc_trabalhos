#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

typedef struct example {
    int *coordinates;
    int *coordinates_number; 
    int centroid_index;         //Index of closest centroid 
} example;

typedef struct centroid {
    int *coordinates;
    int *old_coordinates;
    int coordinates_number;

} centroid;

int euclidean_distance(int *example, int *centroid, int n_attr) {
    int distance = 0;

    for(int i = 0; i < n_attr; i++) {
        distance += (example[i] - centroid[i]) * (example[i] - centroid[i]);
    }
    distance = sqrt(distance);
    return distance;
}

int min_centroid_distance_index(int *example, centroid *centroids, int n_centroids, int n_attr) {
    int centroid_index = 0;
    int distance = euclidean_distance(example, centroids[0].coordinates, n_attr);
    int aux_distance;

    for(int i = 1; i < n_centroids; i++) {
        aux_distance = euclidean_distance(example, centroids[i].coordinates, n_attr);
        if(aux_distance < distance) {
            distance = aux_distance;
            centroid_index = i;
        }
    }
    printf("Menor distancia: %d\n", distance);
    return centroid_index;
}

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

void k_means(example *examples, int n_examples, centroid *centroids, int n_centroids, int n_attr) {
    int aux_index;

    do {
        //TODO K-Means
        for(int i = 0; i < n_examples; i++) {
            aux_index = min_centroid_distance_index(examples[i].coordinates, centroids, n_centroids, n_attr);
            if(examples[i].centroid_index != aux_index) {
                examples[i].centroid_index = aux_index;
            }
        }
        //update_centroids    
    } while (!centroids_are_equals(centroids, n_centroids));
}

void array_copy(int array_a[], int array_b[], int elements_number) {
    for (int i = 0; i < elements_number; i++) {
        array_b[i] = array_a[i];
    }
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
    centroids = (centroid *) malloc(count_lines(centroid_filename) * sizeof(centroid));

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

    k_means(examples, n_examples, centroids, n_centroids, n_attr);

    return 0;
}
