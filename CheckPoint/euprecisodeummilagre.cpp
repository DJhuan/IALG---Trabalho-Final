#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

// Registro principal
struct dadosPrato{
    string nome;
    string chefe;
    int avaliacao;
    float preco;
    string selo;
    bool apagado;
};

// Função que salva as alterações feitas pelo usuário
bool armazenarDados(dadosPrato *vetor, int &tam, string nomeArq);

// Função que expande o vetor de dados, além de realocar a os dados para nova posição na memória
dadosPrato *expandirVetor(dadosPrato *v, int &tam, int exp);

// Função que administra uma saída segura do programa
bool sequenciaSaida(dadosPrato *pratos, int tam, string nomeArq);

// Função que lê os dados do arquivo e armazena num vetor alocado dinâmicamente
dadosPrato *lerDados(dadosPrato *v, int &tam, string arq);

// Procedimento que busca um item para o usuário baseado em uma categoria
void busca (dadosPrato *pratos, int &tam);

// Função que adiciona um novo prato ao vetor que armazena os dados de acordo com a disponibilidade de alocação
dadosPrato *insercao (dadosPrato *pratos, int &tam);

// Procedimento que marca a remoção de um prato
void remocao(dadosPrato pratos[], int &tam);

// Procedimento que edita os campos de um prato pedido pelo usuário
void edicao(dadosPrato *pratos, int &tam);

// Procedimento que imprime o conteúdo do vetor pratos para o usuário
void imprimirDados(dadosPrato vetor[], int tam);

// Procedimento que imprime os comandos disponíveis na tela do usuário
void imprimirMenu();

int main (){

    cout << "*********************************\n";
    cout << "*********** Bem-Vindo ***********\n";
    cout << "*********************************\n\n";

    string nomeBaseDeDados = "baseDados.csv";

    // Leitura dos dados
    int qntPratos = 1;

    dadosPrato *pratos = new dadosPrato[qntPratos];

    pratos = lerDados(pratos, qntPratos, nomeBaseDeDados);
    
    // Menu
    imprimirMenu();
    bool caracterValido = false;
    string opcao;
    while (!caracterValido){
        cout << "Comando: ";
        cin >> opcao;
        string decisao = "0";
        switch (opcao[0]){
            case '1':
                busca(pratos, qntPratos);
                break;
            case '2':
                pratos = insercao(pratos, qntPratos);
                break;
            case '3':
                remocao(pratos, qntPratos);
                break;
            case '4':
                edicao(pratos, qntPratos);
                break;
            case '5':
                imprimirDados(pratos, qntPratos);
                break;
            case 'E':
            case 'e':
                    caracterValido = sequenciaSaida(pratos, qntPratos, nomeBaseDeDados);
                break;
            case 'm':
            case 'M':
                imprimirMenu();
                break;
            default:
                cout << "Infelizmente nao existe esse comando! \n";
                cout << qntPratos << endl;
                break;
        }
    }

    cout << "\n*********************************\n";
    cout << "************* Adeus *************\n";
    cout << "*********************************\n\n";

    return 0;
}

// 1.0 Subprogramas não acessíveis pelo usuário ---------------------------------------

int encontrarIndice(dadosPrato *vetor, int tam, int indiceUsuario){
    int i = 0, k = 1;
    int indiceReal;
    bool encontrado = false;
    for (i=0; i < tam-1; i++){
        if (!vetor[i].apagado and k != indiceUsuario)
            k++;
        else if(k == indiceUsuario and !encontrado){
            indiceReal = i;
            encontrado = true;
        }
    }
    if (encontrado)
        return indiceReal;
    else
        return i-1;
}

dadosPrato *expandirVetor(dadosPrato *v, int &tam, int exp){
    dadosPrato *newV = new dadosPrato[tam + exp];

    copy(v, v + tam, newV);
    tam += exp;
    delete [] v;

    v = newV;

    return v;
}

bool sequenciaSaida(dadosPrato *pratos, int tam, string nomeArq){
    cout << "Salvando dados, aguarde!\n";
    bool armazenado = armazenarDados(pratos, tam, nomeArq);

    string escolha;
    
    if (armazenado){
        cout << "Os dados foram salvos com sucesso!\n";
        cout << "Deseja sair?(1 - Sim / Qualquer tecla - Nao)\n";
        cin >> escolha;
    } else {
        cout << "Os dados do programa não foram salvos corretamente\n";
        cout << "Deseja sair?(1 - Sim / Qualquer tecla - Nao)\n";
        cin >> escolha;
    }

    if (escolha == "1"){
        return true;
    } else {
        return false;
    }
}

// 1.1 Subprogramas de escrita e leitura de dados -------------------------------------

bool armazenarDados(dadosPrato *vetor, int &tam, string nomeArq){
    ofstream saida(nomeArq);
    int lim = tam;
    int i = 0;
    for (i=0; i < lim; i++){
        saida << fixed << setprecision(2);
        saida << vetor[i].nome <<','<< vetor[i].chefe <<','<<  vetor[i].avaliacao <<','<< 
                 vetor[i].preco <<','<< vetor[i].selo <<',';
        if (vetor[i].apagado){
            saida << "1";
        } else {
            saida << "0";
        }
        if (!(i == lim-1))
            saida << '\n';
    }

    if (i == lim){
        // Dados armazenados com sucesso
        return true;
    } else {
        // Erro ao armazenar dados
        return false;
    }
}

dadosPrato *lerDados(dadosPrato *v, int &tam, string arq){
    ifstream entrada(arq);
    int i = 0;
    string nome, chefe;
    while (getline(entrada, nome, ',')){
        v[i].nome = nome;
        char lixeira;
        
        getline(entrada, chefe, ',');
        v[i].chefe = chefe;
        
        entrada >> v[i].avaliacao;
        entrada >> lixeira;

        entrada >> v[i].preco;
        entrada >> lixeira;

        getline (entrada, v[i].selo, ',');
        entrada >> v[i].apagado;
        entrada.ignore();      

        i++;
        v = expandirVetor(v, tam, 1);
    }

    return v;
}

// Fim da seção 1.0 -------------------------------------------------------------------

// 2.0 Subprogramas acessíveis pelo usuário -------------------------------------------
void busca (dadosPrato *pratos, int &tam){ 

    bool opcaoValida = false;
    int campo;
    //Compara a info buscada com as informaçoes do vetor
    do {
        cout << "Em qual campo deseja buscar? " << endl;
        cout << "1. Nome do prato" << endl;
        cout << "2. Chefe Criador" << endl;
        cout << "3. Avaliação" << endl;
        cout << "4. Preço" << endl;
        cout << "5. Selo" << endl;
        cout << "Digite o número referente ao campo: ";
        cin >> campo;
    
        if (campo == 1 or campo == 2 or campo == 3 or campo == 4 or campo == 5) {
            opcaoValida = true;
        } else {
            cout << "Opção inválida. Por favor, escolha uma opção entre 1 e 5." << endl;
        }
    } while (!opcaoValida);

    string pratoBuscado, chefeBuscado, seloBuscado;
    int avaliacaoBuscada;
    float precoBuscado;

    switch (campo){
        case (1): {
            bool encontrado = false;
            cout << "Digite o nome do prato que está buscando: ";
            cin.ignore();
            getline(cin, pratoBuscado);
            for (int i=0; i<tam; i++){
                if (pratoBuscado == pratos[i].nome){
                    cout << "Nome: " << pratos[i].nome << "  " << "Chefe: " << pratos[i].chefe << "  " << "Avaliacao: " << pratos[i].avaliacao << "  ";
                    cout << "Preco: " << pratos[i].preco << "  " << "Selo: " << pratos[i].selo << endl;
                    encontrado = true;
                }
            }
            if (!encontrado) {
                cout << "Não foi encontrado nenhum prato com esse nome." << endl;
            }
            break;
        }
        case (2): {
            bool encontrado = false;
            cout << "Digite o nome do chefe que está buscando: ";
            cin.ignore();
            getline(cin, chefeBuscado);
            for (int i=0; i<tam; i++){
                if (chefeBuscado == pratos[i].chefe){
                    cout << "Nome: " << pratos[i].nome << "  " << "Chefe: " << pratos[i].chefe << "  " << "Avaliacao: " << pratos[i].avaliacao << "  ";
                    cout << "Preco: " << pratos[i].preco << "  " << "Selo: " <<  pratos[i].selo << endl;
                    encontrado = true;
                }
            }
            if (!encontrado) {
                cout << "Não foi encontrado nenhum chefe com esse nome." << endl;
            }
            break;
        }
        case (3): {
            bool encontrado = false;
            cout << "Digite a avaliação que está buscando: ";
            cin >> avaliacaoBuscada;
            for (int i=0; i<tam; i++){
                if (avaliacaoBuscada == pratos[i].avaliacao){
                    cout << "Nome: " << pratos[i].nome << "  " << "Chefe: " << pratos[i].chefe << "  " << "Avaliacao: " << pratos[i].avaliacao << "  ";
                    cout << "Preco: " << pratos[i].preco << "  " << "Selo: " << pratos[i].selo << endl;
                    encontrado = true;
                }
            }
            if (!encontrado) {
                cout << "Não foi encontrado nenhum prato com essa avaliação." << endl;
            }
            break;
        }
        case (4): {
            bool encontrado = false;
            cout << "Digite o preço que está buscando: ";
            cin >> precoBuscado;
            for (int i=0; i<tam; i++){
                if (precoBuscado == pratos[i].preco){
                    cout << "Nome: " << pratos[i].nome << "  " << "Chefe: " << pratos[i].chefe << "  " << "Avaliacao: " << pratos[i].avaliacao << "  ";
                    cout << "Preco: " << pratos[i].preco << "  " << "Selo: " << pratos[i].selo << endl;
                    encontrado = true;
                }
            }
            if (!encontrado) {
                cout << "Não foi encontrado nenhum prato com esse preço." << endl;
            }
            break;
        }
        case (5): {
            bool encontrado = false;
            cout << "Digite o selo que está buscando: ";
            cin.ignore();
            getline(cin, seloBuscado);
            for (int i=0; i<tam; i++){
                if (seloBuscado == pratos[i].selo){
                    cout << "Nome: " << pratos[i].nome << "  " << "Chefe: " << pratos[i].chefe << "  " << "Avaliacao: " << pratos[i].avaliacao << "  ";
                    cout << "Preco: " << pratos[i].preco << "  " << "Selo: " << pratos[i].selo << endl;
                    encontrado = true;
                }
            }
            if (!encontrado) {
                cout << "Não foi encontrado nenhum prato com esse selo." << endl;
            }
            break;
        }
        default:
            cout << "Opção inválida." << endl;
            break;
    }
}

dadosPrato *insercao (dadosPrato *pratos, int &tam){

    cout << "----- Cadastro de novo prato -----" << endl;
    dadosPrato novoPrato;

    cout << "Nome do prato: ";
    cin.ignore();
    getline(cin, novoPrato.nome);

    cout << "Nome do Chefe: ";
    getline(cin, novoPrato.chefe);

    do {
        cout << "Avaliação (1-10): ";
        cin >> novoPrato.avaliacao;

        if (novoPrato.avaliacao < 1 or novoPrato.avaliacao > 10) {
            cout << "Avaliação inválida. Por favor, insira um valor entre 1 e 10." << endl;
        }
    } while (novoPrato.avaliacao < 1 or novoPrato.avaliacao > 10);

    cout << "Preço: ";
    cin >> novoPrato.preco;

    int escolha = 0;
    do {
        cout << "Selo (comum, vegano, vegetariano); (1, 2, 3): ";
        cin >> escolha;

        if (escolha < 1 and escolha > 3) {
            cout << "Selo inválido. Por favor, escolha entre:\n1 - Comum;\n2 - Vegano;\n3-Vegetariano." << endl;
        } else if (escolha == 1){
            novoPrato.selo = "Comum";
        } else if (escolha == 2){
            novoPrato.selo = "Vegano";
        } else if (escolha == 3){
            novoPrato.selo = "Vegetariano";
        }
    } while (escolha < 1 and escolha > 3);
    novoPrato.apagado = false;

    bool substituido = false;
    int i = 0;
    while (i < tam and !substituido){
        if (pratos[i].apagado){
            pratos[i] = novoPrato;
            substituido = true;
        }
        i++;
    }

    if (!substituido){
        pratos = expandirVetor(pratos, tam, 1);
        pratos[tam-1].nome = novoPrato.nome;
        pratos[tam-1].avaliacao = novoPrato.avaliacao;
        pratos[tam-1].chefe = novoPrato.chefe;
        pratos[tam-1].selo = novoPrato.selo;
        pratos[tam-1].preco = novoPrato.preco;
        pratos[tam-1].apagado = false;
    }

    cout << "Prato cadastrado com sucesso!" << endl;

    return pratos;
}

void remocao(dadosPrato pratos[], int &tam){
	cout << "Deseja remover algum dos pratos fornecidos nos dados?" << endl;
    cout << "Se sim, digite 1" << endl << "Se nao, digite 2" << endl;
    string resposta;
    bool correto= false;
    cin>> resposta;
    
    while(!correto){
        if(resposta == "2"){
            cout << endl;
            correto = true;
        }
        else if(resposta == "1"){
            cout << "Digite o identificador do prato que quer remover: " << endl;
            int removerID;
            cin >> removerID;
            removerID = encontrarIndice(pratos, tam, removerID);

            pratos[removerID].apagado = true;
            correto = true;
        }
        else cout << "Caracter não reconhecido" << endl;
    }
}

void edicao(dadosPrato *pratos, int &tam) {
    cout << "Deseja editar algum dos campos dos dados?" << endl;
    cout << "Se sim, digite 1" << endl << "Se nao, digite 2" << endl;
    string resposta1;
    bool corretoEditar = false;
    int resposta3;
    bool campoReconhecido = false;

    while (!corretoEditar) {
        cin >> resposta1;
        if (resposta1 == "2") {
            corretoEditar = true;
            cout << endl;
        }
        else if (resposta1 == "1") {
            corretoEditar = true;
            cout << "Digite o respectivo numero que representa o campo que sera editado: " << endl;
            cout << " 1 para editar o nome" << endl;
            cout << " 2 para editar o chefe" << endl;
            cout << " 3 para editar a avaliacao" << endl;
            cout << " 4 para editar o preco" << endl;
            cout << " 5 para editar o selo" << endl;
            int resposta2;

            while (!campoReconhecido) {
                cin >> resposta2;
                bool idEncontrado = false;
                if (resposta2 == 1 or resposta2 == 2 or resposta2 == 3 or resposta2 == 4 or resposta2 == 5) {
                    int repetir = 1;
                    while (!idEncontrado) {
                        if (repetir == 1) {
                            cout << "Digite o identificador do campo que sera editado: " << endl;
                            repetir--;
                        }
                        cin >> resposta3;
                        resposta3 = encontrarIndice(pratos, tam, resposta3);

                        if (resposta3 <= tam) {
                            idEncontrado = true;
                            if (resposta2 == 1) {
                                campoReconhecido = true;
                                cout << "Digite o novo nome para o prato: " << endl;
                                string resposta4;
                                cin.ignore();
                                getline(cin, resposta4);
                                pratos[resposta3].nome = resposta4;
                            }
                            else if (resposta2 == 2) {
                                campoReconhecido = true;
                                cout << "Digite o novo nome do chefe do prato: " << endl;
                                string resposta4;
                                cin.ignore();
                                getline(cin, resposta4);
                                pratos[resposta3].chefe = resposta4;
                            }
                            else if (resposta2 == 3) {
                                campoReconhecido = true;
                                cout << "Digite a nova avaliacao para o prato: " << endl;
                                int resposta4;
                                cin >> resposta4;
                                if (resposta4 > 10)
                                    resposta4 = 10;
                                if (resposta4 < 0)
                                    resposta4 = 0;
                                pratos[resposta3].avaliacao = resposta4;
                            }
                            else if (resposta2 == 4) {
                                campoReconhecido = true;
                                cout << "Digite o novo preco do prato: " << endl;
                                float resposta4;
                                cin >> resposta4;
                                pratos[resposta3].preco = resposta4;
                            }
                            else if (resposta2 == 5) {
                                campoReconhecido = true;
                                cout << "Digite o respectivo numero que representa o selo escolhido: " << endl;
                                cout << "1 para comum" << endl;
                                cout << "2 para vegano" << endl;
                                cout << "3 para vegetariano" << endl;
                                int resposta4;
                                bool existeSelo = false;

                                while (!existeSelo) {
                                    cin >> resposta4;
                                    if (resposta4 == 1) {
                                        existeSelo = true;
                                        pratos[resposta3].selo = "Comum";
                                    }
                                    else if (resposta4 == 2) {
                                        existeSelo = true;
                                        pratos[resposta3].selo = "Vegano";
                                    }
                                    else if (resposta4 == 3) {
                                        existeSelo = true;
                                        pratos[resposta3].selo = "Vegetariano";
                                    }
                                    else
                                        cout << "Nao existe esse selo" << endl;
                                }
                            }
                        }
                        else
                            cout << "Nao ha esse identificador" << endl;
                    }
                }
                else
                    cout << "Caracter nao reconhecido" << endl;
            }
        }
        else
            cout << "Caracter nao reconhecido" << endl;
    }

    if (campoReconhecido)
        cout << pratos[resposta3].nome << ", " << pratos[resposta3].chefe << ", " << pratos[resposta3].avaliacao << ", " << pratos[resposta3].preco << ", " << pratos[resposta3].selo << endl;
}

void imprimirDados(dadosPrato vetor[], int tam){
    cout << "Escolha a faixa de impressão. Ex.: 14 ate 21\n"
            "Para imprimir todos os valores escolha -1 -1: ";
    int limInferior = 0;
    int limSuperior = tam;
    bool entradasAceitas = false;
    
    do {
        cin >> limInferior >> limSuperior;
        if (limSuperior < limInferior){
            cout << "O limite superior nao pode ser menor que o inferior!\n";
        } else if (limInferior < -1 or limSuperior < -1){
            cout << "Estes limites nao sao validos\n";
        } else if (limSuperior == 0 or limInferior == 0){
            cout << "Estes limites nao sao validos\n";
        } else if (limInferior >= 0 and limSuperior > encontrarIndice(vetor, tam+1, tam+1)){
            limSuperior = encontrarIndice(vetor, tam+1, tam+1);
            entradasAceitas = true;
        } else if(limInferior >= 1 and limSuperior >= 1){
            entradasAceitas = true;
        } else if(limInferior == -1 and limSuperior == -1){
            entradasAceitas = true;
        } else {
            cout << "Estes limites nao sao validos\n";
        }
    } while (!entradasAceitas);

    int indice = 1;
    if (limInferior == -1 and limSuperior == -1){
        limInferior = 0;
        limSuperior = tam-2;
    } else if (limSuperior == limInferior){
        indice = limInferior;
        limSuperior = encontrarIndice(vetor, tam, limSuperior)-1;
        limInferior = limSuperior;
    } else if (limInferior > 0 and limSuperior > 0){
        indice = limInferior;
        limInferior = encontrarIndice(vetor, tam, limInferior);
        limSuperior = encontrarIndice(vetor, tam, limSuperior);
    } 


    cout << setfill(' '); 
    cout << "\nIndice" << 
            setw(31) << "Prato" <<
            setw(22) << "Chefe" << 
            setw(10) << "Avaliacao" << 
            setw(8) << "Preco" <<
            setw(12) << "Selo\n";
    int i = limInferior;
    while (i <= limSuperior){
        if (!vetor[i].apagado){
            cout << setw(6) << indice <<
                    setw(31) << vetor[i].nome << 
                    setw(22) << vetor[i].chefe << 
                    setw(10) << vetor[i].avaliacao << 
                    setw(8) << vetor[i].preco << 
                    setw(12) << vetor[i].selo << endl; 
            indice++;
        }
        i++;
    }
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

// Fim da seção 2.0 --------------------------------------------------------------------
