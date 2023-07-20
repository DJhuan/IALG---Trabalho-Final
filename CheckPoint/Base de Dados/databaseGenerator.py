import random

def listar(filepath):
    file = file = open(filepath)
    lista = file.readlines()
    file.close()
    return lista

comidas = listar("Base de Dados/comidas.txt")
complementos = listar("Base de Dados/complementos.txt")
nomes = listar("Base de Dados/nomes.txt")
sobrenomes = listar("Base de Dados/sobrenomes.txt")

db = []
pratos=[]
nomescompletos=[]

for i in range(100):
    comida = comidas[random.randint(0, len(comidas) -1 )].strip('\n')
    complemento = complementos[random.randint(0, len(complementos) -1 )].strip('\n')

    comida = comida + ' ' + complemento

    nome = nomes[random.randint(0, len(nomes) -1 )].strip('\n')
    sobrenome = sobrenomes[random.randint(0, len(sobrenomes) -1 )].strip('\n')

    nome = nome + ' ' + sobrenome

    nota = str(random.randint(0, 10))
    preco = str(round(random.uniform(27, 217), 2))
    chance = random.random()
    if chance < 0.65:
        selo = "Comum"
    elif chance < 0.85:
        selo = "Vegetariano"
    else:
        selo = "Vegano"

    item = comida +','+ nome +','+ nota +','+ preco +',' + selo + ',0'

    db.append(item + '\n')

saida = open("Base de Dados/saida.txt", 'w')
saida.writelines(db)
saida.close()