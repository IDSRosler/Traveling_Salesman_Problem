/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Igor de Souza Rosler
 *
 * Created on 9 de Outubro de 2019, 15:52
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

using namespace std;

/*
 * 
 */

struct Ponto {
    int x, y;

    Ponto(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

void cria_matriz_adj(Ponto **pontos, int **matriz, int n_vertices);
void print_pontos(Ponto **pontos, int n_vertices);
void cria_pontos(Ponto **pontos, int n_vertices);
void escreve_arq(int **matriz, int n_vertices);
int** matriz_init(int n_vertices);

int main(int argc, char** argv) {

    int n_vertices = atoi(argv[1]);
    int **matriz;
    Ponto **pontos = (Ponto**) malloc(n_vertices * sizeof (Ponto*));

    matriz = matriz_init(n_vertices);
    cria_pontos(pontos, n_vertices);
    cria_matriz_adj(pontos, matriz, n_vertices);
    print_pontos(pontos, n_vertices);
    escreve_arq(matriz, n_vertices);

    return 0;
}

void cria_pontos(Ponto **pontos, int n_vertices) {
    int range = 100;
    int x, y;
    srand(time(0));
    for (int i = 0; i < n_vertices; i++) {
        x = rand() % range;
        y = rand() % range;
        pontos[i] = new Ponto(x, y);
    }
}

void cria_matriz_adj(Ponto **pontos, int **matriz, int n_vertices) {
    int i = 0, j = 0, valor = 0;
    for (i = 0; i < n_vertices; i++) {
        for (j = 0; j < n_vertices; j++) {
            valor = (int) sqrt(pow((pontos[i]->x - pontos[j]->x), 2) + pow((pontos[i]->y - pontos[j]->y), 2));
            matriz[i][j] = valor;
            valor = 0;
        }
    }
}

int** matriz_init(int n_vertices) {
    int **matriz = (int **) malloc(n_vertices * sizeof (int*));
    for (int i = 0; i < n_vertices; i++) {
        matriz[i] = (int *) calloc(n_vertices, sizeof (int));
    }
    return matriz;
}

void print_pontos(Ponto **pontos, int n_vertices) {
    for (int i = 0; i < n_vertices; i++) {
        cout << "X = " << pontos[i]->x << "\tY = " << pontos[i]->y << endl;
    }

}

void escreve_arq(int **matriz, int n_vertices) {
    ofstream file;
    file.open("grafo.txt", std::ofstream::out);
    file << n_vertices << endl;
    
    for (int i = 0; i < n_vertices; i++) {
        for (int j = 0; j < n_vertices; j++) {
            file << matriz[i][j] << endl;
        }
    }
    file.close();
}