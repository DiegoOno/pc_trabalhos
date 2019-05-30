#!/bin/bash

./bin/k_means_sequential data/int_base_59.data data/int_centroid_59_20.data 59
./bin/k_means_sequential data/int_base_161.data data/int_centroid_161_20.data 161
./bin/k_means_sequential data/int_base_256.data data/int_centroid_256_20.data 256
./bin/k_means_sequential data/int_base_1380.data data/int_centroid_1380_20.data 1380
./bin/k_means_sequential data/int_base_1601.data data/int_centroid_1601_20.data 1601

./bin/k_means_parallel data/int_base_59.data data/int_centroid_59_20.data 59 2
./bin/k_means_parallel data/int_base_161.data data/int_centroid_161_20.data 161 2
./bin/k_means_parallel data/int_base_256.data data/int_centroid_256_20.data 256 2
./bin/k_means_parallel data/int_base_1380.data data/int_centroid_1380_20.data 1380 2
./bin/k_means_parallel data/int_base_1601.data data/int_centroid_1601_20.data 1601 2

./bin/k_means_parallel data/int_base_59.data data/int_centroid_59_20.data 59 4
./bin/k_means_parallel data/int_base_161.data data/int_centroid_161_20.data 161 4
./bin/k_means_parallel data/int_base_256.data data/int_centroid_256_20.data 256 4
./bin/k_means_parallel data/int_base_1380.data data/int_centroid_1380_20.data 1380 4
./bin/k_means_parallel data/int_base_1601.data data/int_centroid_1601_20.data 1601 4

./bin/k_means_parallel data/int_base_59.data data/int_centroid_59_20.data 59 8
./bin/k_means_parallel data/int_base_161.data data/int_centroid_161_20.data 161 8
./bin/k_means_parallel data/int_base_256.data data/int_centroid_256_20.data 256 8
./bin/k_means_parallel data/int_base_1380.data data/int_centroid_1380_20.data 1380 8
./bin/k_means_parallel data/int_base_1601.data data/int_centroid_1601_20.data 1601 8

./bin/k_means_parallel data/int_base_59.data data/int_centroid_59_20.data 59 16
./bin/k_means_parallel data/int_base_161.data data/int_centroid_161_20.data 161 16
./bin/k_means_parallel data/int_base_256.data data/int_centroid_256_20.data 256 16
./bin/k_means_parallel data/int_base_1380.data data/int_centroid_1380_20.data 1380 16
./bin/k_means_parallel data/int_base_1601.data data/int_centroid_1601_20.data 1601 16