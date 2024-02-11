# Simulador-de-Escalonamento-de-Processos

Este repositório contém um simulador de escalonamento de processos em linguagem C. O simulador utiliza filas para representar o estado dos processos em diferentes etapas do escalonamento, como "Pronto", "Espera", "Execução" e "Finalizados". O escalonamento é realizado utilizando um algoritmo de preempção baseado em um intervalo de tempo chamado de "quantum".

Funcionalidades
Leitura de um arquivo de entrada que contém informações sobre os processos, incluindo seus nomes e tempos de execução.
Simulação do escalonamento dos processos, considerando a preempção após o término do quantum.
Exibição do estado atual das filas de processos em cada etapa da simulação.
Impressão de informações sobre a execução de cada processo, como preempção e término.
Liberação de memória alocada para as estruturas de dados ao final da simulação.
