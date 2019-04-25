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

    if( argc > 2) {
        while(opt = getopt(argc, argv, "h:e:c:a")) {
            switch (opt){
                case 'h':
                    //show_help();
                    break;
                
                case 'e':
                    example_filename =  optarg;
                    break;
                
                case 'c':
                    centroid_filename = optarg;
                
                case 'a':
                    n_attr = atoi(optarg);
                    break;
                default:
                    fprintf(stderr, "Parâmetro inválido.\n");
                    return -1;
            }
        }
    } else {
        show_help();
    }
}
