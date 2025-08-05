#!/bin/bash

# Lista de executáveis
SOLVERS=("./src/forca_bruta" "./src/backtracking" "./src/2sat")

ENTRADAS="entradas"
mkdir -p "$ENTRADAS"

RESULTADOS="resultados"
mkdir -p "$RESULTADOS"

# Verifica se há arquivos CNF
cnf_files=("$ENTRADAS"/*.cnf)
if [ ${#cnf_files[@]} -eq 0 ]; then
    echo "Nenhum arquivo .cnf encontrado em $ENTRADAS/"
    exit 1
fi

for SOLVER in "${SOLVERS[@]}"; do
    SOLVER_NAME=$(basename "$SOLVER")
    CSV="$RESULTADOS/${SOLVER_NAME}.csv"

    if [ ! -x "$SOLVER" ]; then
        echo "Erro: executável '$SOLVER' não encontrado ou não é executável!"
        continue
    fi

    echo "Rodando com: $SOLVER_NAME"
    echo "arquivo,num_vars,num_clausulas,tempo,resultado" > "$CSV"

    count=0
    for ARQ in "$ENTRADAS"/*.cnf; do
        nome=$(basename "$ARQ")

        # Se for 2sat, só processa arquivos que começam com "2sat_"
        if [[ "$SOLVER_NAME" == "2sat" && "$nome" != 2sat_*.cnf ]]; then
            continue
        fi

        ((count++))
        read num_vars num_clausulas < <(grep "^p cnf" "$ARQ" | awk '{print $3, $4}')

        printf "[%2d] %-25s: " "$count" "$nome"

        inicio=$(date +%s.%N)
        # Captura a saída do solver
        output=$(timeout 60s "$SOLVER" "$ARQ")
        status=$?
        fim=$(date +%s.%N)

        if [ $status -eq 0 ]; then
            tempo=$(echo "$fim - $inicio" | bc)
            
            # Detecta SAT/UNSAT pela saída
            if echo "$output" | grep -q -i "UNSAT"; then
                resultado="UNSAT"
            elif echo "$output" | grep -q -i "SAT"; then
                resultado="SAT"
            else
                resultado="INDEFINIDO"
            fi

            echo "$tempo s ✓ ($resultado)"
            echo "$nome,$num_vars,$num_clausulas,$tempo,$resultado" >> "$CSV"
        elif [ $status -eq 124 ]; then
            echo "TIMEOUT"
            echo "$nome,$num_vars,$num_clausulas,TIMEOUT,TIMEOUT" >> "$CSV"
        else
            echo "ERRO (código $status)"
            echo "$nome,$num_vars,$num_clausulas,ERRO,ERRO" >> "$CSV"
        fi
    done

    echo "Resultados salvos em $CSV"
    echo "-----------------------------"
done
