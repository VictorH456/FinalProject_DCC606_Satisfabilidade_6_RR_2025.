#!/bin/bash

# Caminho do executável
EXEC="./src/teste"

ENTRADAS="entradas_grandes"
mkdir -p "$ENTRADAS"

RESULTADOS="resultados"
mkdir -p "$RESULTADOS"

CSV="$RESULTADOS/resultados1.csv"
echo "arquivo,num_vars,num_clausulas,resultado,tempo" > "$CSV"

if [ ! -f "$EXEC" ]; then
    echo "Erro: executável '$EXEC' não encontrado!"
    echo "Compile com: gcc -o src/forca_bruta src/forca_bruta.c"
    exit 1
fi

total=$(ls "$ENTRADAS"/*.cnf 2>/dev/null | wc -l)
contador=0

echo "Executando testes SAT em $total arquivos CNF..."
echo "==============================================="

for arquivo in "$ENTRADAS"/*.cnf; do
    ((contador++))
    nome=$(basename "$arquivo")
    printf "[%2d/%2d] %-25s: " "$contador" "$total" "$nome"

    # Extrair num_vars e num_clausulas
    read num_vars num_clausulas < <(grep "^p cnf" "$arquivo" | awk '{print $3, $4}')

    # Executar e medir tempo + capturar saída
    inicio=$(date +%s.%N)
    resultado_saida=$(timeout 60s "$EXEC" "$arquivo")
    status=$?
    fim=$(date +%s.%N)

    tempo=$(echo "$fim - $inicio" | bc)

    if [ $status -eq 0 ]; then
        # Extrair SAT ou UNSAT da saída do programa
        resultado=$(echo "$resultado_saida" | grep -Eo "SAT|UNSAT")
        printf "%s in %ss ✓\n" "$resultado" "$tempo"
        echo "$nome,$num_vars,$num_clausulas,$resultado,$tempo" >> "$CSV"
    elif [ $status -eq 124 ]; then
        echo "TIMEOUT"
        echo "$nome,$num_vars,$num_clausulas,TIMEOUT,TIMEOUT" >> "$CSV"
    else
        echo "ERRO (código $status)"
        echo "$nome,$num_vars,$num_clausulas,ERRO,ERRO" >> "$CSV"
    fi
done

echo "==============================================="
echo "Resultados salvos em: $CSV"
