#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define COLUNAS 6
using namespace std;

struct dadosPrato{
    string nome;
    string chefe;
    int avaliacao;
    float preco;
    string selo;
    bool apagado;
};

bool ctbool(string palavra){
    if (palavra == "0")
        return false;
    else
        return true;
}

void carregarDados(vector <dadosPrato> &vetor, string nomeArq){
    ifstream entrada(nomeArq);
    dadosPrato novaLinha;

    while(!entrada.eof()){
        string leitura;
        getline(entrada, novaLinha.nome, ',');
        getline(entrada, novaLinha.chefe, ',');
        getline(entrada, leitura, ',');
        novaLinha.avaliacao = stoi(leitura);
        getline(entrada, leitura, ',');
        novaLinha.preco = stof(leitura);
        getline(entrada, novaLinha.selo, ',');
        getline(entrada, leitura, '\n');
        novaLinha.apagado = ctbool(leitura);
        vetor.push_back(novaLinha);
    }
}

// void lerCSV(vector <dadosPrato> v, string nomeArq){
//     ifstream entrada(nomeArq);
//     string linhaLida, temp;
    
//     string linha[COLUNAS];

//     while (!entrada.eof()){
//         getline(entrada, linhaLida);
//         cout << linhaLida;

//         stringstream ss(linhaLida);

//         for (int i=0; i < COLUNAS; i++){
//             string palavra;
//             getline(ss, palavra, ',');
//             linha[i] = palavra;
//         }

//         dadosPrato empurrar;
//         empurrar.nome = linha[0];
//         empurrar.chefe = linha[1];
//         empurrar.avaliacao = stoi(linha[2]);
//         empurrar.preco = stof(linha[3]);
//         empurrar.selo = linha[4];
//         empurrar.apagado = stbool(linha[5]);

//         v.push_back(empurrar);
//     }
// }

int main (){
    vector <dadosPrato> pratos;
    string c = "saida.csv";
    //dlerCSV(pratos, c);

    carregarDados(pratos, c);

    for (unsigned int i=0; i < pratos.size(); i++){
        cout << pratos[i].nome << '\t';
        cout << pratos[i].chefe << '\t';
        cout << pratos[i].avaliacao << '\t';
        cout << pratos[i].preco << '\t';
        cout << pratos[i].selo << '\t' << endl;
    }
    cout << endl << endl;
    return 0;
}