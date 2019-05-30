#!/bin/bash

echo ">>> Sequencial <<<"
time ./bin/k_means_sequential data/int_base_59.data data/int_centroid_59_20.data 59
time ./bin/k_means_sequential data/int_base_161.data data/int_centroid_161_20.data 161
time ./bin/k_means_sequential data/int_base_256.data data/int_centroid_256_20.data 256
time ./bin/k_means_sequential data/int_base_1380.data data/int_centroid_1380_20.data 1380
time ./bin/k_means_sequential data/int_base_1601.data data/int_centroid_1601_20.data 1601

echo ">>> Paralelo 2 Threads <<<"
time ./bin/k_means_parallel data/int_base_59.data data/int_centroid_59_20.data 59 2
time ./bin/k_means_parallel data/int_base_161.data data/int_centroid_161_20.data 161 2
time ./bin/k_means_parallel data/int_base_256.data data/int_centroid_256_20.data 256 2
time ./bin/k_means_parallel data/int_base_1380.data data/int_centroid_1380_20.data 1380 2
time ./bin/k_means_parallel data/int_base_1601.data data/int_centroid_1601_20.data 1601 2

echo ">>> Paralelo 4 Threads <<<"
time ./bin/k_means_parallel data/int_base_59.data data/int_centroid_59_20.data 59 4
time ./bin/k_means_parallel data/int_base_161.data data/int_centroid_161_20.data 161 4
time ./bin/k_means_parallel data/int_base_256.data data/int_centroid_256_20.data 256 4
time ./bin/k_means_parallel data/int_base_1380.data data/int_centroid_1380_20.data 1380 4
time ./bin/k_means_parallel data/int_base_1601.data data/int_centroid_1601_20.data 1601 4

echo ">>> Paralelo 8 Threads <<<"
time ./bin/k_means_parallel data/int_base_59.data data/int_centroid_59_20.data 59 8
time ./bin/k_means_parallel data/int_base_161.data data/int_centroid_161_20.data 161 8
time ./bin/k_means_parallel data/int_base_256.data data/int_centroid_256_20.data 256 8
time ./bin/k_means_parallel data/int_base_1380.data data/int_centroid_1380_20.data 1380 8
time ./bin/k_means_parallel data/int_base_1601.data data/int_centroid_1601_20.data 1601 8

echo ">>> Paralelo 16 Threads <<<"
time ./bin/k_means_parallel data/int_base_59.data data/int_centroid_59_20.data 59 16
time ./bin/k_means_parallel data/int_base_161.data data/int_centroid_161_20.data 161 16
time ./bin/k_means_parallel data/int_base_256.data data/int_centroid_256_20.data 256 16
time ./bin/k_means_parallel data/int_base_1380.data data/int_centroid_1380_20.data 1380 16
time ./bin/k_means_parallel data/int_base_1601.data data/int_centroid_1601_20.data 1601 16