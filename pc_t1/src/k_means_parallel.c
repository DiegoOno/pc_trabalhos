#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

typedef struct example {
    int *coordinates; 
    int centroid_index;         //Index of closest centroid 
} example;

typedef struct centroid {
    int *coordinates;
    int *old_coordinates;
} centroid;

//Global variables
example *examples;
centroid *centroids;
unsigned long n_attr, n_examples, n_centroids, iteration;
int n_threads;
pthread_barrier_t barrier1, barrier2;

int euclidean_distance(int *example, int *centroid) {
    int distance = 0;

    for(int i = 0; i < n_attr; i++) {
        distance += (example[i] - centroid[i]) * (example[i] - centroid[i]);
    }
    distance = sqrt(distance);
    return distance;
}

int min_centroid_distance_index(int *example) {
    int centroid_index = 0;
    int distance = euclidean_distance(example, centroids[0].coordinates);
    int aux_distance;

    for(int i = 1; i < n_centroids; i++) {
        aux_distance = euclidean_distance(example, centroids[i].coordinates);
        if(aux_distance < distance) {
            distance = aux_distance;
            centroid_index = i;
        }
    }
    return centroid_index;
}

int arrays_are_equals(int *array_a, int *array_b, int elements_number) {
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

int centroids_are_equals() {
    for (int i = 0; i < n_centroids; i++) {
        if (!arrays_are_equals(centroids[i].coordinates, centroids[i].old_coordinates, n_attr))
            return 0;
    } 
    return 1;
}

int *get_near_centroid_index(int centroid_index) {
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

int get_near_centroid_quantity(int centroid_index) {
    int near_examples_quantity = 0;

    for(int i = 0; i < n_examples; i++) {
        if(examples[i].centroid_index == centroid_index) {
            near_examples_quantity++;
        }
    }
    return near_examples_quantity;
}

void *k_means(void *arg) {
    int aux_index, mean, near_examples_quantity;
    int *near_examples_indexes;
    int curr_thread = (int) arg;

    iteration = 0;
    do {
        for(int i = curr_thread; i < n_examples; i += n_threads) {
            aux_index = min_centroid_distance_index(examples[i].coordinates);
            if(examples[i].centroid_index != aux_index) {
                examples[i].centroid_index = aux_index;
            }
        }
        pthread_barrier_wait(&barrier1);
    
        for(int i = curr_thread; i < n_centroids; i += n_threads) {
            array_copy(centroids[i].coordinates, centroids[i].old_coordinates, n_attr);
            near_examples_quantity = get_near_centroid_quantity(i);
            near_examples_indexes = get_near_centroid_index(i);
        
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
        pthread_barrier_wait(&barrier2);
        iteration++;
    } while (!centroids_are_equals());
    
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

void init_old_centroid() {
    for(int i = 0; i < n_centroids; i++) {
        centroids[i].old_coordinates = (int *) malloc(n_attr * sizeof(int));
        for(int j = 0; j < n_attr; j++) {
            centroids[i].old_coordinates[j] = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    int *vet_aux;
    char *example_filename, *centroid_filename;
    int n_lines;
    FILE *ptr;
    pthread_t *threads;

    example_filename = argv[1];
    centroid_filename = argv[2];
    n_attr = atoi(argv[3]);
    n_threads = atoi(argv[4]);

    n_examples = count_lines(example_filename);
    n_centroids = count_lines(centroid_filename);
    
    examples = (example *) malloc(n_examples * sizeof(example));
    centroids = (centroid *) malloc(n_centroids * sizeof(centroid));
    init_old_centroid();

    threads = (pthread_t *) malloc(n_threads * sizeof(pthread_t));
    pthread_barrier_init(&barrier1, NULL, n_threads);
    pthread_barrier_init(&barrier2, NULL, n_threads);
    
    //Get data of examples file
    ptr = fopen(example_filename, "r");
    for(int i = 0; i < n_examples; i++) {
        vet_aux = malloc(n_attr * sizeof(int));
        for(int j = 0; j < n_attr - 1; j++) {
            fscanf(ptr, "%d,", &vet_aux[j]);
        }
        fscanf(ptr, "%d\n", &vet_aux[n_attr - 1]);
        examples[i].coordinates = vet_aux;
    }
    fclose(ptr);

    //Get data of centroids file
    ptr = fopen(centroid_filename, "r");
    for(int i = 0; i < n_centroids; i++) {
        vet_aux = malloc(n_attr * sizeof(int));
        for(int j = 0; j < n_attr - 1; j++) {
            fscanf(ptr, "%d,", &vet_aux[j]);
        }
        fscanf(ptr, "%d\n", &vet_aux[n_attr - 1]);
        centroids[i].coordinates = vet_aux;
    }        
    fclose(ptr);

    //Print examples for test
    // for(int i = 0; i < n_examples; i++) {
    //     for(int j = 0; j < n_attr; j++) {
    //         printf("%d|", examples[i].coordinates[j]);
    //     }
    //     printf("\n");
    // }
    //printf("Total de Iterações do K-Means: %d\n", k_means());
    for(int i = 0; i < n_threads; i++) {
        pthread_create(&threads[i], NULL, k_means, (void *) i);
    }

    for(int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // printf("Numero de Iterações: %ld\n", iteration/n_threads);
    // for(int i = 0; i < n_examples; i++) {
    //     printf("ID: %d    Centroid_ID: %d\n", i,examples[i].centroid_index);
    // }

    pthread_exit(NULL);
}