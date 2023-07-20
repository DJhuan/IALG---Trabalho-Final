// Alunos: Ana Clara Carvalho Nascimento, Jhuan Carlos Sabaini Dassie, Marina Hermógenes Siqueira 
// IALG turma 10B

#include <iostream>
#include <fstream>
#include <iomanip>

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

void imprimirDados(dadosPrato *v, int tam);
dadosPrato *lerDados(string nome_arq, int &tam);
bool armazenarDados(dadosPrato *vetor, int tam, string nomeArq);

void insertion_sort(dadosPrato *v, int tam){
    int ciura[7] = {1,4,10,23,57,132,301};
    int i_gap = 7;
    
    while (ciura[i_gap] > tam){
        i_gap--;
    }

    int gap, j;
    dadosPrato pivo;

    while (i_gap >= 0){
        gap  = ciura[i_gap];
        for (int i=gap; i < tam; i++){
            pivo = v[i];
            j = i;

            while (j >= gap and v[j-gap].avaliacao < pivo.avaliacao){
                v[j] = v[j-gap];
                j -= gap;
            }

            v[j] = pivo;
        }
        i_gap--;
    }
}

int main(int argc, char **argv){
    const string arquivo = "baseDados.dat";

    int qnt_pratos;
    dadosPrato *pratos = lerDados(arquivo, qnt_pratos);

    imprimirDados(pratos, qnt_pratos);
    insertion_sort(pratos, qnt_pratos);
    imprimirDados(pratos, qnt_pratos);

    //bool status_armazenamento = armazenarDados(pratos, qnt_pratos, arquivo);

    return 0;
}

dadosPrato *lerDados(string nome_arq, int &tam){
    // Descobre o tamanho do arquivo e a quantidade de linhas
    ifstream entrada(nome_arq, ios::binary | ios::ate);

    tam = entrada.tellg();
    tam /= sizeof(dadosPrato);

    dadosPrato *v = new dadosPrato[tam];

    // Retornar o cursor para o início do arquivo;
    entrada.seekg(0);

    entrada.read((char *) v, sizeof(dadosPrato[tam]));

    entrada.close();

    return v;
}

void imprimirDados(dadosPrato *v, int tam){
    int limSuperior = tam - 1;
    cout << setfill(' '); 
    cout << "\nIndice" << 
            setw(30) << "Prato" <<
            setw(20) << "Chefe" << 
            setw(10) << "Avaliacao" << 
            setw(8) << "Preco" <<
            setw(13) << "Selo\n";
    int i = 0, indice = 1;

    while (i <= limSuperior){
        if (!v[i].apagado){
            cout << setw(6) << indice <<
                    setw(30) << v[i].nome << 
                    setw(20) << v[i].chefe << 
                    setw(10) << v[i].avaliacao << 
                    setw(8) << v[i].preco << 
                    setw(12) << v[i].selo << endl; 
            indice++;
        }
        i++;
    }
}

bool armazenarDados(dadosPrato *vetor, int tam, string nomeArq){
    ofstream saida(nomeArq);
    
    saida.write((char *) vetor, sizeof(dadosPrato)*tam);

    return true;
}