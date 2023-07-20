#include <iostream>
#include <fstream>
#include <cstring>
#include <string>


#define TAM_PRATO 30
#define TAM_CHEFE 20
#define TAM_LINES 100

using namespace std;

struct dadosPrato{
    char nome[TAM_PRATO];
    char chefe[TAM_CHEFE];
    int avaliacao;
    float preco;
    char selo[12];
    bool apagado;
};

int main(int argc, char **argv){
    ifstream a("baseU.csv");

    string unused;
    int tam = 0;
    while (getline(a, unused) )
        ++tam;
    a.close();
    cout << "O arquivo contem um total de "<< tam << " linhas\n\n.";

    dadosPrato *v = new dadosPrato[tam];

    ifstream entrada("baseU.csv");

    int i = 0;
    string buffer;
    char lixo;
    while(!entrada.eof()){
        getline(entrada, buffer, ',');
        strcpy(v[i].nome, buffer.c_str());
        getline(entrada, buffer, ',');
        strcpy(v[i].chefe, buffer.c_str());
        entrada >> v[i].avaliacao;
        entrada >> lixo;
        entrada >> v[i].preco;
        entrada >> lixo;
        getline(entrada, buffer, ',');
        strcpy(v[i].selo, buffer.c_str());
        entrada >> v[i].apagado;
        entrada.ignore();
        entrada.ignore();

        i++;
    }
    entrada.close();
    ofstream saida("baseDados.dat", ios::binary);
    for (int i=0; i < tam; i++){
        cout << v[i].nome << endl;
    }
    saida.write((char *) v, sizeof(dadosPrato)*tam);
    saida.close();
    return 0;
}