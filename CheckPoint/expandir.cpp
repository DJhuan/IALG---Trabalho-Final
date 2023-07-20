#include <iostream>
#include <fstream>
    
using namespace std;

struct exemplo{
    int id;
    string nome;
};

exemplo *expandirVetor(exemplo *v, int &tam, int exp);
exemplo *carregar(exemplo *v, int &tam, string arq);

int main (){
    int tam = 1;
    exemplo *v = new exemplo[tam];

    v = carregar(v, tam, "valores.txt");

    for (int i=0; i < tam; i++){
        cout << v[i].id << endl;
        cout << v[i].nome << endl;
    }
    return 0;
}

exemplo *expandirVetor(exemplo *v, int &tam, int exp){
    exemplo *newV = new exemplo[tam + exp];

    copy(v, v + tam, newV);
    tam += exp;
    delete [] v;

    v = newV;

    return v;
}

exemplo *carregar(exemplo *v, int &tam, string arq){
    ifstream entrada("valores.txt");

    int i = 0;
    while (!entrada.eof()){
        if (i == tam ){
            v = expandirVetor(v, tam, 1);
        }
        entrada >> v[i].id;
        entrada >> v[i].nome;
        i++;
    }

    return v;
}