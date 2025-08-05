import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

PASTA = "resultados"
GRAFICOS = "graficos"
os.makedirs(GRAFICOS, exist_ok=True)

ARQUIVOS = {
    "forca_bruta": "Força Bruta",
    "backtracking": "Backtracking",
    "2sat": "2-SAT"
}

def carregar_csv(nome_arquivo):
    caminho = os.path.join(PASTA, f"{nome_arquivo}.csv")
    df = pd.read_csv(caminho)

    df = df[pd.to_numeric(df["tempo"], errors="coerce").notnull()]
    df["tempo"] = df["tempo"].astype(float)
    df["num_vars"] = df["num_vars"].astype(int)
    df["arquivo"] = df["arquivo"].astype(str)
    df["algoritmo"] = nome_arquivo 
    return df

def gerar_boxplot(df, titulo, nome_saida):
    plt.figure(figsize=(10, 6))
    sns.boxplot(data=df, x="num_vars", y="tempo", hue="algoritmo")

    plt.yscale("log")
    plt.xlabel("Número de Variáveis")
    plt.ylabel("Tempo de Execução (log escala)")
    plt.title(titulo)
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout()

    saida = os.path.join(GRAFICOS, f"{nome_saida}_boxplot.png")
    plt.savefig(saida)
    plt.close()
    print(f"✔ Boxplot salvo em: {saida}")

def main():
    df_fb = carregar_csv("forca_bruta")
    df_bt = carregar_csv("backtracking")
    df_2sat = carregar_csv("2sat")

    # --- Boxplot normal (auto_)
    df_normal = pd.concat([
        df_fb[df_fb["arquivo"].str.contains("auto_")],
        df_bt[df_bt["arquivo"].str.contains("auto_")]
    ])
    gerar_boxplot(df_normal, "Boxplot: Tempo vs. Número de Variáveis (Instâncias aleatórias)", "boxplot_normal")

    # --- Boxplot 2SAT
    df_2sat_all = pd.concat([
        df_fb[df_fb["arquivo"].str.contains("2sat_")],
        df_bt[df_bt["arquivo"].str.contains("2sat_")],
        df_2sat[df_2sat["arquivo"].str.contains("2sat_")]
    ])
    gerar_boxplot(df_2sat_all, "Boxplot: Tempo vs. Número de Variáveis (Instâncias 2-SAT)", "boxplot_2sat")

if __name__ == "__main__":
    main()
