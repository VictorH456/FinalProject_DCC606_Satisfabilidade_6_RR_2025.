import random
import os

os.makedirs("entradas", exist_ok=True)

def gerar_cnf(nome_arquivo, num_vars, num_clausulas, max_literals_por_clausula=3, repeticoes=5):
    """Gera múltiplas fórmulas CNF genéricas com num_vars variáveis e num_clausulas cláusulas."""
    for i in range(repeticoes):
        with open(f"{nome_arquivo}_{i}.cnf", "w") as f:
            f.write("c Fórmula gerada automaticamente\n")
            f.write(f"p cnf {num_vars} {num_clausulas}\n")
            for _ in range(num_clausulas):
                num_literals = random.randint(1, max_literals_por_clausula)
                clausula = set()
                while len(clausula) < num_literals:
                    var = random.randint(1, num_vars)
                    sinal = random.choice([-1, 1])
                    clausula.add(sinal * var)
                f.write(" ".join(map(str, clausula)) + " 0\n")

# Gera CNFs genéricas
for n in range(3, 13):
    num_clausulas = n * 5
    gerar_cnf(f"entradas/auto_{n}vars", n, num_clausulas)
    print(f"Fórmulas CNF para {n} variáveis geradas.")
