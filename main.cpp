/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Igor de Souza Rosler
 *
 * Created on 2 de Outubro de 2019, 14:01
 */

#include <cstdlib>
#include <vector>
#include <cstdint>
#include <iostream>
#include <bits/stdc++.h>
#include <time.h>
#include <random>

using namespace std;

//#define V 8 
#define MAX 99999999

struct Node {
    int index;
    vector<struct Node> prox;

    Node(int index) {
        this->index = index;
        this->prox.clear();
    }

    void add_node(Node node) {
        this->prox.push_back(node);
    }
};

int** init_matriz_adj(int n_vertices);
void cria_grafo_euclidiano(int **grafo, int n_vertices);
void imprime_grafo(int **grafo, int n_vertices);
void destroi_grafo(int **grafo, int n_vertices);
int TSP_otimo(int **grafo, int vertice_inicial, int V);
int TSP_aproximado(int **grafo, int vertice_inicial, int n_vertices);
vector<int> arvore_ger_minima(int **grafo, int vert_inicial, int* arv_min, int n_vertices);
Node* cria_arvore(vector<int> vetor, int n_vertices, int vert_inicial);
void caminhada(Node *root, vector<int> *caminho);

int main(int argc, char** argv) {
    int n_vertices = atoi(argv[1]);
    int vertice_inicial = atoi(argv[2]);
    int op = atoi(argv[3]) == 0 ? 0 : 1;
    int **grafo;

    grafo = init_matriz_adj(n_vertices);
    grafo[0][1] = 10;
    grafo[0][2] = 15;
    grafo[0][3] = 20;
    grafo[1][0] = 10;
    grafo[1][2] = 35;
    grafo[1][3] = 25;
    grafo[2][0] = 15;
    grafo[2][1] = 35;
    grafo[2][3] = 30;
    grafo[3][0] = 20;
    grafo[3][1] = 25;
    grafo[3][2] = 30;
    //cria_grafo_euclidiano(grafo, n_vertices);
    //imprime_grafo(grafo, n_vertices);
    switch (op) {
        case 0: cout << "Menor caminho:\t" << TSP_otimo(grafo, vertice_inicial, n_vertices) << endl;
            break;
        case 1: cout << "Menor caminho aproximado:  " << TSP_aproximado(grafo, vertice_inicial, n_vertices) << endl;
            break;
    }


    destroi_grafo(grafo, n_vertices);

    return 0;
}

int TSP_otimo(int **grafo, int vertice_inicial, int V) {
    vector<int> vertices;
    for (int i = 0; i < V; i++)
        if (i != vertice_inicial)
            vertices.push_back(i);
    int caminho_minimo = MAX;
    do {
        int tamanho_caminho_atual = 0;
        int k = vertice_inicial;
        for (int i = 0; i < vertices.size(); i++) {
            tamanho_caminho_atual += grafo[k][vertices[i]];
            k = vertices[i];
        }
        tamanho_caminho_atual += grafo[k][vertice_inicial];
        caminho_minimo = min(caminho_minimo, tamanho_caminho_atual);
    } while (next_permutation(vertices.begin(), vertices.end()));
    return caminho_minimo;
}

int TSP_aproximado(int **grafo, int vertice_inicial, int n_vertices) {
    int *arv_min = (int *) malloc(n_vertices * sizeof (int));
    int status[n_vertices];
    int dist = 0;
    int primeiro = 1;
    int ultimo = 0;
    vector<int> vetor, caminho;
    for (int i = 0; i < n_vertices; i++) {
        status[i] = -1;
    }
    status[vertice_inicial] = 1;
    vetor = arvore_ger_minima(grafo, vertice_inicial, arv_min, n_vertices);
    Node *root = cria_arvore(vetor, n_vertices, vertice_inicial);
    caminhada(root, &caminho);
    // Mostra caminho percorrido
    //    cout << "Full Walk:\t";
    //    for (int i = 0; i < caminho.size(); i++) {
    //        cout << caminho[i] << " ";
    //    }
    //    cout << "\n" << endl;
    for (int i = 0; i < caminho.size() - 1; i++) {
        if (status[caminho[i + 1]] == -1 || primeiro == 1) {
            dist += grafo[caminho[i]][caminho[i + 1]];
            status[caminho[i]] = 1;
            if (status[caminho[i]] == -1) {
                ultimo = caminho[i];
            }
            primeiro = 0;
        } else {
            for (int j = i; j < caminho.size() - 1; j++) {
                if (status[caminho[j + 1]] == -1) {
                    dist += grafo[caminho[i]][caminho[j + 1]];
                    status[caminho[j]] = 1;
                    i = j;
                    if (status[caminho[j]] == -1) {
                        ultimo = caminho[j];
                    }
                    break;
                } else {
                    if (status[caminho[j]] == -1) {
                        ultimo = caminho[j];
                    }
                }
            }
        }
    }
    dist += grafo[ultimo][vertice_inicial];

    return dist;
}

void cria_grafo_euclidiano(int **grafo, int n_vertices) {
    srand(time(0));
    int i = 0, j = 0, valor = 0;
    for (; i < n_vertices;) {
        for (; j < n_vertices;) {
            if (i == j) {
                grafo[i][j] = 0;
            } else {
                while (valor == 0) {
                    valor = rand() % 20;
                    if (valor != 0)
                        grafo[i][j] = valor;
                }
            }
            j++;
            valor = 0;
        }
        i++;
        j = i;
    }
    i = 1;
    j = 0;
    for (; i < n_vertices;) {
        for (; j < i;) {
            grafo[i][j] = grafo[j][i];
            j++;
        }
        i++;
        j = 0;
    }
}

void imprime_grafo(int **grafo, int n_vertices) {
    for (int i = 0; i < n_vertices; i++) {
        for (int j = 0; j < n_vertices; j++) {
            cout << grafo[i][j] << "\t";
        }
        cout << "\n";
    }
}

int** init_matriz_adj(int n_vertices) {
    int **grafo = (int **) malloc(n_vertices * sizeof (int*));
    for (int i = 0; i < n_vertices; i++) {
        grafo[i] = (int *) calloc(n_vertices, sizeof (int));
    }
    return grafo;
}

void destroi_grafo(int **grafo, int n_vertices) {
    for (int i = 0; i < n_vertices; i++) {
        free(grafo[i]);
    }
    free(grafo);
}

vector<int> arvore_ger_minima(int **grafo, int vert_inicial, int* arv_min, int n_vertices) {
    int i, j, dest, primeiro, menor_dist, aux;
    vector<int> vertices;
    for (i = 0; i < n_vertices; i++) {
        arv_min[i] = -1;
    }
    aux = vert_inicial;
    arv_min[vert_inicial] = vert_inicial;
    while (1) {
        primeiro = 1;
        for (i = 0; i < n_vertices; i++) {
            for (int x = 0; x < n_vertices; x++)
                if (x != i)
                    vertices.push_back(x);
            if (arv_min[i] != -1) {
                for (j = 0; j < vertices.size(); j++) {
                    if (arv_min[vertices[j]] == -1) {
                        if (primeiro) {
                            menor_dist = grafo[i][j];
                            vert_inicial = i;
                            dest = vertices[j];
                            primeiro = 0;
                        } else {
                            if (menor_dist > grafo[i][j]) {
                                menor_dist = grafo[i][j];
                                vert_inicial = i;
                                dest = vertices[j];
                            }
                        }
                    }
                }
            }
        }
        if (primeiro == 1)
            break;
        arv_min[dest] = vert_inicial;
    }
    vector<int> vetor;
    for (int i = 0; i < n_vertices; i++) {
        vetor.push_back(arv_min[i]);
    }
    return vetor;
}

Node* cria_arvore(vector<int> vetor, int n_vertices, int vert_inicial) {
    Node **arvore = (Node**) malloc(n_vertices * sizeof (Node*));
    Node **atual = (Node**) malloc(n_vertices * sizeof (Node*));
    for (int i = 0; i < n_vertices; i++) {
        //Node novo = Node(i);
        arvore[i] = new Node(i);
    }
    for (int j = 0; j < n_vertices; j++) {
        atual[j] = arvore[j];
        for (int i = 0; i < n_vertices; i++) {
            if (i != j) {
                if (vetor[i] == atual[j]->index) {
                    atual[j]->add_node(*arvore[i]);
                }
            }
        }
    }
    return arvore[vert_inicial];
}

void caminhada(Node *root, vector<int> *caminho) {
    caminho->push_back(root->index);
    if (root->prox.size() == 0) {
        return;
    } else {
        for (int i = 0; i < root->prox.size(); i++) {
            caminhada(&root->prox[i], caminho);
            caminho->push_back(root->index);
        }
    }
}
