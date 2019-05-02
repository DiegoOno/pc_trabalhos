#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

typedef struct example {
    int* coordinates;
    int* coordinates_number;

} example;

typedef struct centroid {
    int* coordinates;
    int* old_coordinates;
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

void show_help() {
    fprintf(stderr, "-h      Ajuda.\n-e      Nome do arquivo de exemplos.\n-c      Nome do arquivo de centroides.\n-a      Quantidade de atributos dos exemplos e dos centroídes.\n");
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
    char *example_filename, *centroid_filename;
    int n_attr;
    FILE *ptr;

    example_filename = argv[1];
    centroid_filename = argv[2];
    n_attr = atoi(argv[3]);
    return 0;
}
