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

void imprimirMenu();
void imprimirDados(string nome_arq);
dadosPrato *lerDados(string nome_arq, int &tam);
bool armazenarDados(dadosPrato *vetor, int tam, string nomeArq);
void shell_sort(string nome_arq);
dadosPrato *expandirVetor(dadosPrato *v, int &tam, int exp);

int main(){

    const string NOME_ARQUIVO = "baseDados.dat";

    cout << "*********************************\n";
    cout << "*********** Bem-Vindo ***********\n";
    cout << "*********************************\n\n";

    // Impressao do menu
    imprimirMenu();

    // Loop principal - Leitura dos comandos
    bool caracterValido = false;
    string opcao;
    while (!caracterValido){
        cout << "Comando: ";
        cin >> opcao;
        string decisao = "0";
        switch (opcao[0]){
            case '1':
                //busca(pratos, qntPratos);
                break;
            case '2':
                //pratos = insercao(pratos, qntPratos, nomeBaseDeDados);
                break;
            case '3':
                //remocao(pratos, qntPratos);
                break;
            case '4':
                //edicao(pratos, qntPratos);
                break;
            case '5':
                imprimirDados(NOME_ARQUIVO);
                break;
            case 'E':
            case 'e':
                    caracterValido = true;//sequenciaSaida(pratos, qntPratos, nomeBaseDeDados);
                break;
            case 'm':
            case 'M':
                imprimirMenu();
                break;
            case 'i':
                shell_sort(NOME_ARQUIVO);
                break;
            default:
                cout << "Infelizmente nao existe esse comando! \n";
                break;
        }
    }

    cout << "\n*********************************\n";
    cout << "************* Adeus *************\n";
    cout << "*********************************\n\n";

    //bool status_armazenamento = armazenarDados(pratos, qnt_pratos, arquivo);

    return 0;
}

void shell_sort(string nome_arquivo){
    int tam;
    dadosPrato *v =  lerDados(nome_arquivo, tam);

    int ciura[7] = {1,4,10,23,57,132,301};
    int i_gap = 6;
    
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

    ofstream saida(nome_arquivo, ios::binary);
    saida.write((char *) v, sizeof(dadosPrato[tam]));

    delete [] v;
}

dadosPrato *expandirVetor(dadosPrato *v, int &tam, int exp){
    dadosPrato *newV = new dadosPrato[tam + exp];

    copy(v, v + tam, newV);
    tam += exp;
    delete [] v;

    v = newV;

    return v;
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

void imprimirDados(string nome_arq){
    cout << setfill(' ');
    cout << "\nIndice" << 
    setw(30) << "Prato" <<
    setw(20) << "Chefe" << 
    setw(10) << "Avaliacao" << 
    setw(8) << "Preco" <<
    setw(13) << "Selo" << endl;

    ifstream entrada(nome_arq, ios::binary | ios::ate);
    int linhas = entrada.tellg();
    linhas = linhas / sizeof(dadosPrato);

    dadosPrato *buffer = new dadosPrato;
    int indice = 1;

    entrada.seekg(0, ios::beg);

    while (linhas >= 1){
        entrada.read((char *) buffer, sizeof(dadosPrato));
        if (!(buffer->apagado)){
            cout << setw(6) << indice <<
                    setw(30) << buffer->nome<< 
                    setw(20) << buffer->chefe << 
                    setw(10) << buffer->avaliacao << 
                    setw(8) << buffer->preco << 
                    setw(12) << buffer->selo << endl;
            indice++;
        }
        linhas--;
    }
}

bool armazenarDados(dadosPrato *vetor, int tam, string nomeArq){
    ofstream saida(nomeArq);
    
    saida.write((char *) vetor, sizeof(dadosPrato)*tam);

    return true;
}

void imprimirMenu(){
    cout << "Escolha uma opcao para comecar" << endl;
    cout << "1 - Buscar um item\n"
            "2 - Inserir um item\n"
            "3 - Apagar um item\n"
            "4 - Editar um item\n"
            "5 - Exibir valores\n"
            "M - Exibir comandos\n"
            "E - Sair\n";
}