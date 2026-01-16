import random

# Configuration
NUM_STATIONS = 1000
NUM_EDGES = 3000
OUTPUT_FILE = "./metro_long.txt"

prefixes = ["Porte de", "Saint", "Place", "Rue", "Boulevard", "Avenue", "Église", "Mairie de", "Pont", "Général", "Château"]
noms = ["Vincennes", "Italie", "Clichy", "Orléans", "Martin", "Michel", "Lazare", "Cloud", "Sèvres", "Lecourbe", "Hugo", "Jaurès", "Pasteur", "Monceau", "Dumas", "Voltaire"]
suffixes = ["- La Chapelle", "- Université", "Centre", "Hôpital", "Sud", "Nord", "Village"]

def generer_nom():
    return f"{random.choice(prefixes)} {random.choice(noms)}"

lines = []
lines.append("# Reseau de metro long créé par le script python (généré par IA)")
lines.append("# Format :\n# STATION;id;nom\n# EDGE;id_depart;id_arrivee;temps_en_minutes")

real_ids = []

# --- GÉNÉRATION DES STATIONS (AVEC ERREURS CRÉATIVES) ---
for i in range(NUM_STATIONS):
    s_id = str(i)
    name = generer_nom()
    r = random.random()

    if r < 0.01:
        # 1% - Mot clé mal orthographié
        lines.append(f"STATIO;{s_id};{name}")
        real_ids.append(s_id)

    elif r < 0.03:
        # 2% - Station Fantôme sans ID
        lines.append("STATION;;Fantôme")

    elif r < 0.04:
        # 1% - ID dupliqué
        if i > 0:
            lines.append(f"STATION;{i-1};Doublon {name}")
            real_ids.append(str(i-1))

    elif r < 0.06:
        # 2% - Nom coupé
        lines.append(f"STATION;{s_id};Pont de ")
        real_ids.append(s_id)

    elif r < 0.08:
        # 2% - TEST / debug
        lines.append(f"STATION;{s_id};TEST_DO_NOT_USE_{i}")
        real_ids.append(s_id)

    elif r < 0.10:
        # 2% - Mauvais délimiteur
        lines.append(f"STATION,{s_id},{name}")
        real_ids.append(s_id)

    elif r < 0.13:
        # 3% - ID alphanumérique
        bad_id = f"{i}B"
        lines.append(f"STATION;{bad_id};{name}")
        real_ids.append(bad_id)

    elif r < 0.17:
        # 4% - Colonne en trop
        lines.append(f"STATION;{s_id};{name};En travaux")
        real_ids.append(s_id)

    else:
        # 83% - Station correcte
        lines.append(f"STATION;{s_id};{name}")
        real_ids.append(s_id)

# --- GÉNÉRATION DES EDGES (AVEC ERREURS CRÉATIVES) ---
# Base connectée
for i in range(len(real_ids) - 1):
    lines.append(f"EDGE;{real_ids[i]};{real_ids[i+1]};3")

# Remplissage avec erreurs
current_edges = len(real_ids) - 1
while current_edges < NUM_EDGES:
    src = random.choice(real_ids)
    dst = random.choice(real_ids)
    temps = str(random.randint(1, 10))
    r = random.random()
    
    if src == dst: continue


    if r < 0.01:
        lines.append(f"EDGE;{src};{dst};{temps}min")

    elif r < 0.02:
        lines.append(f"EDGE;{src};{dst};-5")

    elif r < 0.03:
        lines.append(f"EDGE;{src};99999;10")

    elif r < 0.04:
        lines.append(f"EDGE;{src};{src};1")

    elif r < 0.05:
        lines.append(f"EDGE;{src};{dst};")

    elif r < 0.07:
        lines.append(f"EDGE;{src};{dst};0")

    elif r < 0.09:
        lines.append(f"EDGE|{src}|{dst}|{temps}")

    elif r < 0.11:
        lines.append(f"LINK;{src};{dst};{temps}")

    elif r < 0.14:
        lines.append(f"EDGE;{src};{dst};{temps};Ligne14")

    elif r < 0.17:
        lines.append(f"EDGE;douze;treize;{temps}")

    elif r < 0.20:
        lines.append(f"EDGE;{src};{dst};2.5")

    else:
        lines.append(f"EDGE;{src};{dst};{temps}")
        
    current_edges += 1

with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print(f"Fichier '{OUTPUT_FILE}' généré avec des erreurs créatives.")