import subprocess
import re
import re



for i in range(1000):
    if i%2==0:
        subprocess.run(
            ["./bin/Linux/Debug/dicewars", "-r","./bin/Linux/Debug/referee.so","-m","./bin/Linux/Release/genmap.so","-s","./bin/Linux/Release/strategytest.so","-s","./bin/Linux/Debug/strategy.so","-g","./bin/Linux/Debug/guistats.so"],
            check=False,
            stdout=subprocess.DEVNULL,  # ne pas afficher la sortie normale
        )
    else:
        subprocess.run(
            ["./bin/Linux/Debug/dicewars", "-r","./bin/Linux/Debug/referee.so","-m","./bin/Linux/Release/genmap.so","-s","./bin/Linux/Debug/strategy.so","-s","./bin/Linux/Release/strategytest.so","-g","./bin/Linux/Debug/guistats.so"],
            check=False,
            stdout=subprocess.DEVNULL,  # ne pas afficher la sortie normale
        )

nom_fichier = "resstats.txt"

with open(nom_fichier, 'r') as input_file:
    contenu = input_file.read()  # Lecture du contenu du fichier
    # list_contenu = contenu.split("\n\n")
    list_contenu = contenu.split("==") 
    # list_contenu = contenu.split("\n")
    # print(list_contenu)
    list_contenu.remove('')
    for i in range (len(list_contenu)):
        list_contenu[i] = list_contenu[i].split('\n')
    # print(list_contenu[0])

    #chaque element de list_contenu contient les stats d'une partie
    list_parties = []

    for p in list_contenu:
        stat_dico = {}
        stat_dico["nb_joueurs"] = 0
        stat_dico["stratégies"] = []
        stat_dico["1er_joueur"] = -1
        stat_dico["stratégie_gagnante"] = ""
        stat_dico["gagnant"] = -1
        for e in p:
            if re.search("players", e, flags=0) != None:
                stat_dico["nb_joueurs"] = int(e[3])
            if re.search("-- P", e, flags=0) != None:
                stat_dico["stratégies"].append(re.search(r":\s*(.*?)\s*\(", e).group(1) +" "+ re.search(r"\((?:.*/)?([^/]+)\)", e).group(1))
            if e!= '' and e[0] == "W":
                stat_dico["gagnant"] = int(e[5])
                stat_dico["stratégie_gagnante"] = e[26:]
        list_parties.append(stat_dico)
# print(list_parties)

print(f'nombre de parties jouées : {len(list_parties)}')
# print(f'Pourcentage de victoire de la stratégie {}')

nb_joueurs = list_parties[-1]["nb_joueurs"]
nb_vic_j = nb_joueurs*[0]
vic_strategies = {}
for j in list_parties[-1]["stratégies"]:
    vic_strategies[j] = 0

for e in list_parties :
    for i in range(nb_joueurs):
        if e["gagnant"] == i:
            nb_vic_j[i] +=1

    for j in e["stratégies"]:
        if re.search(e["stratégie_gagnante"][:-1], j, flags=0) != None :
            vic_strategies[j] +=1
            break
print(vic_strategies)


for i in range(nb_joueurs):
    print(f'pourcentage de victoire du joueur {i} : {nb_vic_j[i]/len(list_parties)*100} %')

for k, v  in vic_strategies.items() :
    print(f'pourcentage de victoire de la stratégie {k} : {v/len(list_parties)*100} % ')