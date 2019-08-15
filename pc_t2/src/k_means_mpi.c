#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <mpi.h>
#define TAG 0

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
int id, size, centroids_size;
unsigned long n_attr, n_examples, n_centroids, iteration;

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

void init_old_centroid() {
    for(int i = 0; i < n_centroids; i++) {
        centroids[i].old_coordinates = (int *) malloc(n_attr * sizeof(int));
        for(int j = 0; j < n_attr; j++) {
            centroids[i].old_coordinates[j] = 0;
        }
    }
}

void k_means() {
    
    int continue_k_means, aux_index, curr_proc, *aux_coordinates, near_examples_quantity, *near_examples_indexes, mean;
    int *aux_centroid_coordinates, *aux_old_coordinates;
    MPI_Status st;
    centroid *centroids_aux;

    init_old_centroid();
    do{
        // Sending centroids to all process
        if(id == 0) {
            for(int i = 0; i < n_centroids; i++) {
                for(int j = 1; j < size; j++) {
                    MPI_Send(centroids[i].coordinates, n_attr, MPI_INT, j, i + j, MPI_COMM_WORLD);
                    MPI_Send(centroids[i].old_coordinates, n_attr, MPI_INT, j, i + 100, MPI_COMM_WORLD);
                    //printf("Enviando Centroid %d para processo %d.\n", i, j);
                }
            }
        }

        if(id !=0) {
            centroids = (centroid *) malloc(n_centroids * sizeof(centroid));
            for(int i = 0; i < n_centroids; i++) {
                aux_centroid_coordinates = (int *) malloc(n_attr * sizeof(int));
                aux_old_coordinates = (int *) malloc(n_attr * sizeof(int));
                MPI_Recv(aux_centroid_coordinates, n_attr, MPI_INT, 0, i + id, MPI_COMM_WORLD, &st);
                centroids[i].coordinates = aux_centroid_coordinates;
                MPI_Recv(aux_old_coordinates, n_attr, MPI_INT, 0, i + 100, MPI_COMM_WORLD, &st);
                centroids[i].old_coordinates = aux_old_coordinates;
                //printf("Processo %d recebendo centroid %d\n", id, i);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        //Calculate distance
        if(id == 0) {
            for(int i = 0; i < n_examples; i++) {
                MPI_Send(examples[i].coordinates, n_attr, MPI_INT, (i % (size - 1) + 1), TAG + i, MPI_COMM_WORLD);
                //printf("Enviando exemplo %d para processo %d", i, (i % (size - 1) + 1));
            }
        }

        if(id != 0) {
            for(int i = id - 1; i < n_examples; i += size - 1) {
                aux_coordinates = (int *) malloc(n_attr * sizeof(int));
                MPI_Recv(aux_coordinates, n_attr, MPI_INT, 0, TAG + i, MPI_COMM_WORLD, &st);
                aux_index = min_centroid_distance_index(aux_coordinates);
                MPI_Send(&aux_index, 1, MPI_INT, 0, TAG + i * 2, MPI_COMM_WORLD);
                //printf("Enviando indice %d do exemplo %d\n", aux_index, i);
                free(aux_coordinates);
            }
        }

        if(id == 0) {
            for(int i = 0; i < n_examples; i++) {
                aux_index = 0;
                MPI_Recv(&aux_index, 1, MPI_INT, (i % (size - 1)) + 1, TAG + i * 2, MPI_COMM_WORLD, &st);
                if(examples[i].centroid_index != aux_index) {
                    examples[i].centroid_index = aux_index;
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        // Send the indexes calculed for each process
        if(id == 0) {
            for(int i = 0; i < n_examples; i++) {
                for(int j = 1; j < size; j++) {
                    MPI_Send(&examples[i].centroid_index, 1, MPI_INT, j, TAG + i + n_examples, MPI_COMM_WORLD);
                }
            }
        }

        if(id != 0) {
            for(int i = 0; i < n_examples; i++) {
                aux_index = 0;
                MPI_Recv(&aux_index, 1, MPI_INT, 0, TAG + i + n_examples, MPI_COMM_WORLD, &st);
                examples[i].centroid_index = aux_index;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

        // Update centroids
        if(id != 0) {
            for(int i = id - 1; i < n_centroids; i += size - 1) {
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
                MPI_Send(centroids[i].coordinates, n_attr, MPI_INT, 0, TAG + i, MPI_COMM_WORLD);
            }
        }

        if(id == 0) {
            for(int i = 0; i < n_centroids; i++) {
                array_copy(centroids[i].coordinates, centroids[i].old_coordinates, n_attr);
                MPI_Recv(centroids[i].coordinates, n_attr, MPI_INT, (i % (size - 1)) + 1, TAG + i, MPI_COMM_WORLD, &st);
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if(id == 0) {
            iteration++;
            continue_k_means = !centroids_are_equals();
            for(int i = 1; i < size; i++) {
                MPI_Send(&continue_k_means, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            }
        }

        if(id != 0) {
            //continue_k_means = -1;
            MPI_Recv(&continue_k_means, 1, MPI_INT, 0, id, MPI_COMM_WORLD, &st);
            //free(centroids);
        }    
        
        MPI_Barrier(MPI_COMM_WORLD);

    } while (continue_k_means);
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


int main(int argc, char *argv[]) {
    int *vet_aux, n_lines;
    int mean, near_examples_quantity, *near_examples_indexes, *aux_coordinates;
    char *example_filename, *centroid_filename;
    FILE *ptr;
    MPI_Status st;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    example_filename = argv[1];
    centroid_filename = argv[2];
    n_attr = atoi(argv[3]);

    n_examples = count_lines(example_filename);
    n_centroids = count_lines(centroid_filename);
        
    examples = (example *) malloc(n_examples * sizeof(example));
    centroids = (centroid *) malloc(n_centroids * sizeof(centroid));

    if(id == 0) {
        if(id == 0) {    
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
        }

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
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Send all examples to the othes process
    if(id == 0) {
        for(int i = 0; i < n_examples; i++) {
            for(int j = 1; j < size; j++) {
                MPI_Send(examples[i].coordinates, n_attr, MPI_INT, j, TAG + i, MPI_COMM_WORLD);
            }
        }
    }

    if(id != 0) {
        examples = (example *) malloc(n_examples * sizeof(example));
        for(int i = 0; i < n_examples; i++) {
            aux_coordinates = (int *) malloc(n_attr * sizeof(int));
            MPI_Recv(aux_coordinates, n_attr, MPI_INT, 0, TAG + i, MPI_COMM_WORLD, &st);
            examples[i].coordinates = aux_coordinates;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD); 

    k_means();

    MPI_Barrier(MPI_COMM_WORLD);

    if(id == 0) {
        printf("Iterações: %ld.\n", iteration);
    }
    
    MPI_Finalize();
}
