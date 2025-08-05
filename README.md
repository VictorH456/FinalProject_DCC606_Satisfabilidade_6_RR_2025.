
# FinalProject_DCC606_Satisfabilidade_VictorCostaEGiovanaOliveira_RR_2025

Este projeto é uma implementação e análise de diferentes algoritmos para resolver o problema de **Satisfatibilidade Booleana (SAT)**, utilizando fórmulas no formato **DIMACS CNF**. A proposta foi desenvolvida como parte do trabalho final da disciplina **DCC606 - Análise de Algoritmos**, com o objetivo de:

- Determinar se uma fórmula booleana é **satisfatível**.
- Comparar o **tempo de execução** de diferentes algoritmos em função do número de variáveis.
- Gerar e analisar instâncias com diferentes tamanhos e características.
- Visualizar os resultados em forma de gráficos.

## Autores

- Victor Hugo Souza Costa
- Giovana Oliveira

## 📂 Estrutura do Projeto

```
.
├── src/                       # Códigos-fonte dos executaveis
│   ├── forca_bruta.c
│   ├── backtracking.c
│   └── 2sat.c
│   └── gerar_cnf.py           # Script Python para gerar arquivos CNF
│   └── gerar_2sat.py          # Script Python para gerar arquivos CNF 2SAT
│   └── gerar_graficos.py      # Script Python para gerar gráficos de comparação
├── entradas/                  # Fórmulas CNF geradas pelo código em python
├── resultados/                # Resultados gerados em CSV
├── graficos/                  # Gráficos gerados pelo script Python
├── run_all.sh                 # Script Bash para rodar todos os solucionadores
└── README.md           
```

## ⚙️ Compilação

Todos os solucionadores estão escritos em **C** e devem ser compilados individualmente com `gcc`:

```bash
gcc src/forca_bruta.c -o src/forca_bruta
gcc src/backtracking.c -o src/backtracking
gcc src/2sat.c -o src/2sat
```

> Certifique-se de que o diretório `src/` tenha permissão de execução.

## 🧪 Gerando Fórmulas de Teste

Execute o seguinte script para gerar fórmulas de teste nos diretórios `entradas/`:

```bash
python3 gerar_cnf.py
python3 gerar_2sat.py
```

- `entradas/` conterá todas as fórmulas de teste.

## 🚀 Execução dos Solucionadores

Use o script Bash `run_all.sh` para executar todos os solucionadores sobre as fórmulas CNF e armazenar os resultados em `resultados/`.

```bash
chmod +x run_all.sh
./run_all.sh
```

O script irá:
- Rodar os arquivos `.cnf` com cada algoritmo.
- Medir o tempo de execução e se a respectiva fórmula é válida.
- Salvar os resultados em arquivos CSV como `forca_bruta.csv`, `backtracking.csv` e `2sat.csv`.

## 📊 Geração de Gráficos

Para gerar os gráficos comparativos execute:

```bash
python3 gerar_graficos.py
```

Os gráficos serão salvos no diretório `graficos/`.

## 📚 Algoritmos Implementados

### 1. Força Bruta
Testa todas as $(2^n)$ atribuições possíveis para (n) variáveis.

- **Complexidade:**  $O(2^n ⋅ m)$

### 2. Backtracking
Realiza atribuições recursivas com poda baseada na verificação parcial das cláusulas.

- **Complexidade (pior caso):**  $O(2^n ⋅ m)$

### 3. 2-SAT
Usa o grafo de implicações + componentes fortemente conexas (Kosaraju) para resolver em tempo linear.

- **Complexidade:** O(n + m)

## 🧠 Aplicações do Problema de SAT

SAT é um problema fundamental na computação teórica e tem aplicações em:

- Verificação formal de hardware/software
- Planejamento e raciocínio em IA
- Compiladores e análise de dependências
- Criptografia e resolução de puzzles lógicos
