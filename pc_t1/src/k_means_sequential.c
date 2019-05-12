#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

typedef struct example {
    int *coordinates; 
    int centroid_index;         //Index of closest centroid 
} example;

typedef struct centroid {
    int *coordinates;
    int *old_coordinates;
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

void array_copy(int *array_a, int *array_b, int elements_number) {
    for (int i = 0; i < elements_number; i++) {
        array_b[i] = array_a[i];
    }
}

int centroids_are_equals(centroid centroid[], int centroids_number, int n_attr) {
    for (int i = 0; i < centroids_number; i++) {
        if (!arrays_are_equals(centroid[i].coordinates, centroid[i].old_coordinates, n_attr))
            return 0;
    } 
    return 1;
}

int *get_near_centroid_index(example *examples, int n_examples, int centroid_index) {
    int *near_examples_indexes = NULL, *aux = NULL, near_examples_quantity;

    near_examples_quantity = 0;
    for(int i = 0; i < n_examples; i++) {
        if(examples[i].centroid_index == centroid_index) {
            near_examples_quantity++;
            aux = (int *) realloc(near_examples_indexes, near_examples_quantity * sizeof(int));
            near_examples_indexes = aux;
            near_examples_indexes[near_examples_quantity - 1] = i;
        }
    }
    return near_examples_indexes;
}

int get_near_centroid_quantity(example *examples, int n_examples, int centroid_index) {
    int near_examples_quantity = 0;

    for(int i = 0; i < n_examples; i++) {
        if(examples[i].centroid_index == centroid_index) {
            near_examples_quantity++;
        }
    }
    return near_examples_quantity;
}

void update_centroids(example *examples, int n_examples, centroid *centroids, int n_centroids, int n_attr) {
    int mean, near_examples_quantity;
    int *near_examples_indexes;
  
    for(int i = 0; i < n_centroids; i++) {
        array_copy(centroids[i].coordinates, centroids[i].old_coordinates, n_attr);
        near_examples_quantity = get_near_centroid_quantity(examples, n_examples, i);
        near_examples_indexes = get_near_centroid_index(examples, n_examples, i);
        
        if(near_examples_quantity > 0) {
            for(int j = 0; j < n_attr; j++) {
                mean = 0;
                for(int k = 0; k < near_examples_quantity; k++) {
                    mean += examples[near_examples_indexes[k]].coordinates[j];
                }
            centroids[i].coordinates[j] = mean / near_examples_quantity;
            }
        }
    }
}

int k_means(example *examples, int n_examples, centroid *centroids, int n_centroids, int n_attr) {
    int aux_index, iteration = 0;

    do {
        for(int i = 0; i < n_examples; i++) {
            aux_index = min_centroid_distance_index(examples[i].coordinates, centroids, n_centroids, n_attr);
            if(examples[i].centroid_index != aux_index) {
                examples[i].centroid_index = aux_index;
            }
        }
        update_centroids(examples, n_examples, centroids, n_centroids, n_attr);
        iteration++;
    } while (!centroids_are_equals(centroids, n_centroids, n_attr));
    return iteration;
}

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

void init_old_centroid(centroid *centroids, int n_centroids, int n_attr) {
    for(int i = 0; i < n_centroids; i++) {
        centroids[i].old_coordinates = (int *) malloc(n_attr * sizeof(int));
        for(int j = 0; j < n_attr; j++) {
            centroids[i].old_coordinates[j] = 0;
        }
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

    n_examples = count_lines(example_filename);
    n_centroids = count_lines(centroid_filename);
    
    examples = (example *) malloc(n_examples * sizeof(example));
    centroids = (centroid *) malloc(n_centroids * sizeof(centroid));
    init_old_centroid(centroids, n_centroids, n_attr);

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

    //Print examples for test
    // for(int i = 0; i < n_examples; i++) {
    //     for(int j = 0; j < n_attr; j++) {
    //         printf("%d|", examples[i].coordinates[j]);
    //     }
    //     printf("\n");
    // }

    printf("Total de Iterações do K-Means: %d\n", k_means(examples, n_examples, centroids, n_centroids, n_attr));

    return 0;
}
