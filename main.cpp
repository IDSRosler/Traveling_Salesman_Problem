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
#include <fstream>
#include <list>

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
void TSP_otimo(int **grafo, int vertice_inicial, int V);
void TSP_aproximado(int **grafo, int vertice_inicial, int n_vertices);
void imprime_grafo(int **grafo, int n_vertices);
void destroi_grafo(int **grafo, int n_vertices);
void caminhada(Node *root, vector<int> *caminho);
void completa_matriz(int **grafo, int n_vertices);
vector<int> arvore_ger_minima(int **grafo, int vert_inicial, int n_vertices);
Node* cria_arvore(vector<int> vetor, int n_vertices, int vert_inicial);

int main(int argc, char** argv) {
    int n_vertices;
    int vertice_inicial = atoi(argv[1]);
    int op = atoi(argv[2]) == 0 ? 0 : 1;
    int **grafo;
    ifstream file;
    file.open("./Cria_Grafo/grafo.txt", std::ofstream::in);
    string line;

    if (file.is_open()) {
        getline(file, line);
        n_vertices = stoi(line);
        file.close();
    } else {
        cout << "Problema na leitura do arquivo." << endl;
    }

    grafo = init_matriz_adj(n_vertices);
    completa_matriz(grafo, n_vertices);
    imprime_grafo(grafo, n_vertices);

    switch (op) {
        case 0: TSP_otimo(grafo, vertice_inicial, n_vertices);
            break;
        case 1: TSP_aproximado(grafo, vertice_inicial, n_vertices);
            break;
    }


    destroi_grafo(grafo, n_vertices);

    return 0;
}

void TSP_otimo(int **grafo, int vertice_inicial, int V) {
    vector<int> vertices;
    vector<int> caminho, caminho_final;
    int aux = vertice_inicial;
    for (int i = 0; i < V; i++)
        if (i != vertice_inicial)
            vertices.push_back(i);
    int caminho_minimo = MAX;
    do {
        int tamanho_caminho_atual = 0;
        int k = vertice_inicial;
        caminho.clear();
        caminho.push_back(k);
        for (int i = 0; i < vertices.size(); i++) {
            tamanho_caminho_atual += grafo[k][vertices[i]];
            k = vertices[i];
            caminho.push_back(k);
        }
        tamanho_caminho_atual += grafo[k][vertice_inicial];
        if (caminho_minimo > tamanho_caminho_atual) {
            caminho_final.clear();
            for (int i = 0; i < caminho.size(); i++) {
                caminho_final.push_back(caminho[i]);
            }
            caminho_final.push_back(aux);
        }
        caminho_minimo = min(caminho_minimo, tamanho_caminho_atual);
    } while (next_permutation(vertices.begin(), vertices.end()));
    cout << "Caminho percorrido:\t";
    for (int i = 0; i < caminho_final.size(); i++) {
        cout << caminho_final[i] << " ";
    }
    cout << "\nMenor caminho:\t" << caminho_minimo << endl;
}

void TSP_aproximado(int **grafo, int vertice_inicial, int n_vertices) {
    int status[n_vertices];
    int dist = 0;
    int primeiro = 1;
    int ultimo = 0;
    vector<int> vetor, caminho, caminho_final;
    for (int i = 0; i < n_vertices; i++) {
        status[i] = -1;
    }
    vetor = arvore_ger_minima(grafo, vertice_inicial, n_vertices);
    Node *root = cria_arvore(vetor, n_vertices, vertice_inicial);
    caminhada(root, &caminho);
    for (int i = 0; i < vetor.size(); i++) {
        cout << vetor[i] << " ";
    }
    cout << "\n";
    status[vertice_inicial] = 1;
    ultimo = vertice_inicial;
    for (int i = 0; i < caminho.size() - 1; i++) {
        if (status[caminho[i + 1]] == -1) {
            dist += grafo[ultimo][caminho[i + 1]];
            caminho_final.push_back(ultimo);
            ultimo = caminho[i + 1];
            status[caminho[i + 1]] = 1;
        }
    }
    caminho_final.push_back(ultimo);
    caminho_final.push_back(vertice_inicial);
    dist += grafo[ultimo][vertice_inicial];
    cout << "Caminho percorrido:\t";
    for (int i = 0; i < caminho_final.size(); i++) {
        cout << caminho_final[i] << " ";
    }
    cout << "\nCaminho aproximado:\t" << dist << endl;
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

void completa_matriz(int **grafo, int n_vertices) {
    ifstream file;
    file.open("./Cria_Grafo/grafo.txt", std::ofstream::in);
    string line;

    if (file.is_open()) {
        getline(file, line);
        cout << line << endl;
        for (int i = 0; i < n_vertices; i++) {
            for (int j = 0; j < n_vertices; j++) {
                getline(file, line);
                grafo[i][j] = stoi(line);
            }

        }
        file.close();
    } else {
        cout << "Problema na leitura do arquivo." << endl;
    }
}

void destroi_grafo(int **grafo, int n_vertices) {
    for (int i = 0; i < n_vertices; i++) {
        free(grafo[i]);
    }
    free(grafo);
}

vector<int> arvore_ger_minima(int **grafo, int vert_inicial, int n_vertices) {
    int i, j, dest, primeiro, menor_dist;
    vector<int> vertices;
    int arv_min[n_vertices];
    for (i = 0; i < n_vertices; i++) {
        arv_min[i] = -1;
    }
    arv_min[vert_inicial] = vert_inicial;
    while (1) {
        primeiro = 1;
        for (i = 0; i < n_vertices; i++) {
            vertices.clear();
            for (int x = 0; x < n_vertices; x++) {
                if (i != x)
                    vertices.push_back(x);
            }
            if (arv_min[i] != -1) {
                for (j = 0; j < vertices.size(); j++) {
                    if (arv_min[vertices[j]] == -1) {
                        if (primeiro) {
                            menor_dist = grafo[i][vertices[j]];
                            vert_inicial = i;
                            dest = vertices[j];
                            primeiro = 0;
                        } else {
                            if (menor_dist > grafo[i][vertices[j]]) {
                                menor_dist = grafo[i][vertices[j]];
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
    for (int i = 0; i < n_vertices; i++) {
        arvore[i] = new Node(i);
    }
    for (int i = 0; i < n_vertices; i++) {
        if (i != vetor[i]) {
            arvore[vetor[i]]->add_node(*arvore[i]);
            cout << arvore[vetor[i]]->index << " " << arvore[i]->index << endl;
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