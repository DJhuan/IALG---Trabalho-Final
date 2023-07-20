#include <iostream>
#include <fstream>

using namespace std;

struct dadosPrato{
    string nome;
    string chefe;
    int avaliacao;
    float preco;
    string selo;
    bool apagado;
};

int main(int argc, char **argv){
    int tam = 3;
    dadosPrato *v = new dadosPrato[tam];
    dadosPrato aux;
    int i = 0;
    ifstream entrada("entradas.txt");

    while (entrada >> aux.nome>>aux.chefe>>aux.avaliacao>>aux.preco>>aux.selo>>aux.apagado){
        v[i] = aux;
        i++;
    }
    entrada.close();
    ofstream saida("dados.dat", ios::binary);
    saida.write((char *)v, tam*sizeof(dadosPrato));
    saida.close();
    cout << "FIM";

    return 0;
}