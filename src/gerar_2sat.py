import os
import random

os.makedirs("entradas", exist_ok=True)

def gerar_2sat(nome_arquivo, num_vars, num_clausulas, repeticoes=5):
    """Gera múltiplas fórmulas 2-SAT com num_vars variáveis e num_clausulas cláusulas."""
    for i in range(repeticoes):
        with open(f"{nome_arquivo}_{i}.cnf", "w") as f:
            f.write("c Fórmula gerada automaticamente para 2-SAT\n")
            f.write(f"p cnf {num_vars} {num_clausulas}\n")
            for _ in range(num_clausulas):
                while True:
                    x = random.randint(1, num_vars)
                    y = random.randint(1, num_vars)
                    if x != y:
                        break
                sinal1 = random.choice([-1, 1])
                sinal2 = random.choice([-1, 1])
                f.write(f"{sinal1 * x} {sinal2 * y} 0\n")

# Gera CNFs para 2-SAT
for n in range(3, 13):
    num_clausulas = n * 4
    gerar_2sat(f"entradas/2sat_{n}vars", n, num_clausulas)
    print(f"Fórmulas 2-SAT para {n} variáveis geradas.")
