Universidade Estadual de Maringá
Centro de Tecnologia - Departamento de Informática
Ciência da Computação
Disciplina: Programação Concorrente - 6908
Profº. Nilton Luiz Queiroz Junior
Aluno: Diego Kazuo Ono          R.A: 90567
Aluno: Leonardo Vendrame        R.A: 90562

Primeiro Trabalho

Para a criação das aplicações do trabalho foram utilizadas as bibliotecas
pthread.h e math.h presentes na linguagem C. Para a compilação portanto é
necessário passar os parâmetros de linkagem -lpthread e -lm.

A organização dos arquivos se dá da seguinte forma:
src     pasta dos códigos-fonte em C
data    pasta com os arquivos de entrada das aplicações
bin     pasta com os binários gerados após a compilação

Para facilitar na compilação houve a elaboração de um arquivo Makefile, bastando
utilizar o comando "make" em um terminal bash para que as aplicações sejam compiladas
make                compila as duas aplicações
make sequential     compila a aplicação sequencial
make parallel       compila a aplicação paralela
make clean          exclui os binários gerados pelo compilador

Para a execução é necessário passar os parâmetros de entrada, na seguinte ordem:
sequencial  dados da base, dados dos centroides, número de parâmetros por linha da base
paralelo    dados da base, dados dos centroides, número de parâmetros por linha da base, 
            número de threads

Seguem dois exemplo de linha de comando de execução:
./bin/k_means_sequential data/int_base_1601.data data/int_centroid_1601_20.data 1601
./bin/k_means_parallel data/int_base_1601.data data/int_centroid_1601_20.data 1601 4

Foram também criados dois arquivos para a execução de todos os testes que foram realizados:
run.sh              Executa ambas as versões para todos os arquivos de entrada para os 
                    números de threads: 2, 4, 8 e 16 na versão paralela
run_with_times.sh   Faz o mesmo que o acima porém com o comando time para exibir os tempos 
                    de execução

Obs.: Pode ser necessário dar permissão para a execução dos arquivos .sh, para isso utilizar
os comandos:
chmod +x run.sh
chmod +x run_with_times.sh
