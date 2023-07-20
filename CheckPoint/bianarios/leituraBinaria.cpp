dadosPrato *lerDados(dadosPrato *v, int &tam, string arq){
    ifstream entrada(arq);
    int i = 0;
    while (!entrada.eof()){
        string nome, chefe;
        getline(entrada, nome, ',');
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