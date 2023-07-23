// Alunos: Ana Clara Carvalho Nascimento, Jhuan Carlos Sabaini Dassie, Marina Hermógenes Siqueira 
// IALG turma 10B

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

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

void imprimir_menu();
void imprimir_dados(string nome_arq);
dadosPrato *ler_dados(string nome_arq, int &tam);
void shell_sort(string nome_arq);
dadosPrato *expandir_vetor(dadosPrato *v, int &tam, int exp);
void organizar_apagados(string nome_arq, int indice=-1);
int seletor_de_filtro(string &filtroSelo);
bool comparador(dadosPrato itemA, dadosPrato itemB, string filtroSelo, int tipo);
void insercao(string nome_arq);
void remocao(string nome_arq);
int divisao_apagados(string nome_arq, int &linhas);

int main(){

    const string NOME_ARQUIVO = "baseDados.dat";

    cout << "*********************************\n";
    cout << "*********** Bem-Vindo ***********\n";
    cout << "*********************************\n\n";

    // Impressao do menu
    imprimir_menu();

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
                insercao(NOME_ARQUIVO);
                break;
            case '3':
                remocao(NOME_ARQUIVO);
                break;
            case '4':
                //edicao(pratos, qntPratos);
                break;
            case '5':
                imprimir_dados(NOME_ARQUIVO);
                break;
            case 'E':
            case 'e':
                caracterValido = true;
                break;
            case 'm':
            case 'M':
                imprimir_menu();
                break;
            case 'i':
                organizar_apagados(NOME_ARQUIVO);
                break;
            default:
                cout << "Infelizmente nao existe esse comando! \n";
                break;
        }
    }

    cout << "\n*********************************\n";
    cout << "************* Adeus *************\n";
    cout << "*********************************\n\n";

    //bool status_armazenamento = armazenar_dados(pratos, qnt_pratos, arquivo);

    return 0;
}

void remocao(string nome_arq){
    int linhas, i_itensApagados;
    i_itensApagados = divisao_apagados(nome_arq, linhas);

	cout << "Deseja remover algum dos pratos fornecidos nos dados?" << endl;
    cout << "Sim / Nao (1 / 2): ";
    string resposta;
    bool correto = false;
    
    while(!correto){
        cin>> resposta;
        if(resposta == "2"){
            cout << endl;
            correto = true;
        }
        else if(resposta == "1"){
            cout << "Digite o identificador do prato que quer remover: ";
            int removerID;
            cin >> removerID;
            int pos = removerID-1;
            dadosPrato dados;
            cout << i_itensApagados << "FLAAAAGQ!!\n";
            fstream arquivo (nome_arq, ios::in | ios::out | ios::binary | ios::ate);
            if (pos > i_itensApagados){
                cout << "! Identificador INVALIDO !\n";
            } else {
                arquivo.seekg (pos * sizeof(dadosPrato), ios::beg);
                arquivo.read((char*) &dados, sizeof(dadosPrato));
                if (dados.apagado){
                    //Identificador ja apagado
                    cout << "! Identificador INVALIDO !\n";
                } else {
                    dados.apagado = true;
                    arquivo.seekp (pos * sizeof(dadosPrato), ios::beg);
                    arquivo.write((char*) &dados, sizeof(dadosPrato));
                    correto = true;
                    arquivo.close();
                    organizar_apagados(nome_arq, pos+1);
                    cout << "! ITEM REMOVIDO !\n";
                }
            }
        }
        else
            cout << "\nCaracter nao reconhecido!\nTente novamente: ";
    }
}

void insercao(string nome_arq){
    int linhas, i_itensApagados;
    i_itensApagados = divisao_apagados(nome_arq, linhas);

    fstream saida(nome_arq, ios::binary | ios::in | ios::ate | ios::out);
    saida.seekp(i_itensApagados+1 * sizeof(dadosPrato), ios::beg);

    dadosPrato *novoItem = new dadosPrato;
    cout << "Entre com as informações do novo prato: \nNome: ";
    cin.ignore();
    cin.getline(novoItem->nome, sizeof(novoItem->nome));
    cout << "Chefe: ";
    cin.getline(novoItem->chefe, sizeof(novoItem->chefe));
    do {
        cout << "Avaliacao (0 até 10): ";
        cin >> novoItem->avaliacao;
    } while (novoItem->avaliacao < 0 or novoItem->avaliacao > 10);
    cout << "Preco: ";
    cin >> novoItem->preco;
    cin.ignore();

    cout << "Escolha o selo: \n[1] - Vegano\n[2] - Vegetariano\n[3] - Comum\nEscolha: ";
    int escolha = -1;
    while (escolha < 1 or escolha > 3){
        cin >> escolha;
        if (escolha < 1 or escolha > 3)
            cout << "Entrada invalida!\nEscolha novamente: ";
    }
    if (escolha == 1)
        strncpy(novoItem->selo, "Vegano", sizeof(char[12]));
    else if (escolha == 2)
        strncpy(novoItem->selo, "Vegetariano", sizeof(char[12]));
    else
        strncpy(novoItem->selo, "Comum", sizeof(char[12]));

    novoItem->apagado = false;

    saida.seekp(((i_itensApagados+1) * sizeof(dadosPrato)), ios::beg);
    saida.write((char *) novoItem, sizeof(dadosPrato));
    saida.close();
}

int seletor_de_filtro(string &filtroSelo){
    cout << "\nPreferencia de ordenacao: \n";
    cout << "Voce deseja ordenar por: \n[1] - Avaliacao\n[2] - Preco\n[3] - Selo";
    int filtroAtributo = 0;
    filtroSelo = "NADA";

    while (filtroAtributo < 1 or filtroAtributo > 3){
        cout << "\nSelecione: ";
        cin >> filtroAtributo;

        if (filtroAtributo < 1 or filtroAtributo > 3){
            cout << "!! Escolha Inválida !!\n";
        } else if (filtroAtributo == 3){
            int opcao = -1;
            cout << "Escolha o selo: \n[1] - Vegano\n[2] - Vegetariano\n[3] - Comum";
            while (opcao < 1 or opcao > 3){
                cout << "\nSelecione: ";
                cin >> opcao;
                if (opcao == 1){
                    filtroSelo = "Vegano";
                } else if (opcao == 2){
                    filtroSelo = "Vegetariano";
                } else if (opcao == 3){
                    filtroSelo = "Comum";
                } else {
                    cout << "!! Escolha Inválida !!\n";
                }
            }
        }
    }
    return filtroAtributo;
}

bool comparador(dadosPrato itemA, dadosPrato itemB, string filtroSelo, int tipo=0){
    /* 
    Retorno: True - itemA >= itemB
             False - itemB > itemA
    Comparações (tipo): 0-Avaliacao; 1-Preço; 2-Selo
    */

    switch (tipo){
        case 1:
            if (itemA.avaliacao >= itemB.avaliacao)
                return true;
            else
                return false;
            break;
        case 2:
            if (itemA.preco >= itemB.preco)
                return true;
            else
                return false;
            break;
        case 3:
            if (itemA.selo == filtroSelo)
                return true;
            else
                return false;
            break;
        default:
                return false;
            break;
    }
}

void shell_sort(string nome_arquivo){
    
    string filtroSelo;
    int filtroAtributo = seletor_de_filtro(filtroSelo);

    int tam;
    dadosPrato *v =  ler_dados(nome_arquivo, tam);

    int ciura[7] = {1,4,10,23,57,132,301};
    int i_gap = 6;
    while (ciura[i_gap] > tam){
        i_gap--;
    }

    // Encontra a divisão onde os itens apagados estão armazenados
    int i_itensApagados = tam - 1;
    while (v[i_itensApagados].apagado){
        i_itensApagados--;
    }
    

    int gap, j;
    dadosPrato pivo;

    while (i_gap >= 0){
        gap  = ciura[i_gap];
        for (int i=gap; i <= i_itensApagados; i++){
            pivo = v[i];
            j = i;

            while (j >= gap and comparador(pivo, v[j-gap], filtroSelo, filtroAtributo)){
                v[j] = v[j-gap];
                j -= gap;
            }

            v[j] = pivo;
        }
        i_gap--;
    }

    ofstream saida(nome_arquivo, ios::binary);
    saida.seekp(0, ios::beg);
    saida.write((char *) v, sizeof(dadosPrato[tam]));
    saida.close();
    delete [] v;

    cout << "\n*** ORDENACAO CONCLUIDA ***\n\n";
}

dadosPrato *expandir_vetor(dadosPrato *v, int &tam, int exp){
    dadosPrato *newV = new dadosPrato[tam + exp];

    copy(v, v + tam, newV);
    tam += exp;
    delete [] v;

    v = newV;

    return v;
}

dadosPrato *ler_dados(string nome_arq, int &tam){
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

void imprimir_dados(string nome_arq){

    int linhas, i_itensApagados;
    i_itensApagados = divisao_apagados(nome_arq, linhas);

    ifstream entrada(nome_arq, ios::binary | ios::ate);
    entrada.seekg(0);

    cout << "Escolha uma faixa de impressão (Ex.: 9 17): ";
    int limSup, limInf;
    cin >> limInf >> limSup;

    bool indices_validos = false;
        while (!indices_validos){
        if ((limInf + limSup) == 0){
            limInf = 0;
            limSup = linhas-1;
            indices_validos = true;
        } else if ((limInf <= limSup) and (limInf-1 >= 0) and (limSup-1 <= i_itensApagados)){
            limInf--;
            limSup--;
            indices_validos = true;
        } else {
            cout << "Limites inválidos!!\n";
            cout << "Escolha novos limites: ";
            cin >> limInf >> limSup;
        }
    }

    // Impressao do cabeçalho
    cout << setfill(' ');
    cout << "\nIndice" << setw(30) << "Prato" << setw(20) << "Chefe" << 
    setw(10) << "Avaliacao" << setw(8) << "Preco" << setw(12) << "Selo" << endl;

    entrada.seekg(limInf * sizeof(dadosPrato), ios::beg);

    dadosPrato *buffer = new dadosPrato;

    int indice = limInf + 1;
    while (limSup >= limInf){
        entrada.read((char *) buffer, sizeof(dadosPrato));
        if (!(buffer->apagado)){
            cout << setw(6) << indice << setw(30) << buffer->nome<< setw(20) << buffer->chefe << 
            setw(10) << buffer->avaliacao << setw(8) << buffer->preco << setw(12) << buffer->selo << endl;
            indice++;
        }
        limSup--;
    }
    delete buffer;
}

void organizar_apagados(string nome_arq, int indice){
    int tam;
    dadosPrato *v = ler_dados(nome_arq, tam);

    int i_itensApagados = tam - 1;

    while (v[i_itensApagados].apagado){
        i_itensApagados--;
    }

    if (indice == -1){
        for (int j=0; j < tam and j < i_itensApagados; j++){
            if (v[j].apagado){
                swap(v[j], v[i_itensApagados]);
                i_itensApagados--;
            }
        }
    } else {
        indice -= 1; // Subtraindo para que o índice em tela corresponda ao do vetor
        swap(v[indice], v[i_itensApagados]);
    }
    
    ofstream saida(nome_arq, ios::binary);
    saida.write((char *) v, sizeof(dadosPrato[tam]));
    saida.close();

    delete [] v;
}

void imprimir_menu(){
    cout << "Escolha uma opcao para comecar" << endl;
    cout << "1 - Buscar um item\n"
            "2 - Inserir um item\n"
            "3 - Apagar um item\n"
            "4 - Editar um item\n"
            "5 - Exibir valores\n"
            "M - Exibir comandos\n"
            "E - Sair\n";
}

int divisao_apagados(string nome_arq, int &linhas){
    // Retorna o maior indice dos itens não apagados e a quantidade total de itens por referencia
    ifstream entrada(nome_arq, ios::binary | ios::ate);
    linhas = entrada.tellg() / sizeof(dadosPrato);

    dadosPrato *buffer = new dadosPrato;
    int pos_cursor = entrada.tellg();
    entrada.seekg(pos_cursor - (sizeof(dadosPrato)));
    entrada.read((char *) buffer, sizeof(dadosPrato));
    int i_itensApagados = linhas - 1;

    while (buffer->apagado){
        i_itensApagados--;
        entrada.seekg(pos_cursor -= 2*sizeof(dadosPrato));
        entrada.read((char *) buffer, sizeof(dadosPrato));
    }
    entrada.close();

    delete buffer;

    return i_itensApagados;
}