#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int verifica_total(int **clausulas, int num_clausulas, int *valores)
{
    for (int i = 0; i < num_clausulas; i++)
    {
        int clausula_ok = 0;
        for (int j = 0; clausulas[i][j] != 0; j++)
        {
            int lit = clausulas[i][j];
            int var = abs(lit) - 1;
            if ((lit > 0 && valores[var] == 1) || (lit < 0 && valores[var] == 0))
            {
                clausula_ok = 1;
                break;
            }
        }
        if (!clausula_ok)
            return 0;
    }
    return 1;
}

int verifica_parcial(int **clausulas, int num_clausulas, int *valores)
{
    for (int i = 0; i < num_clausulas; i++)
    {
        int clausula_ok = 0;
        int indefinido = 0;

        for (int j = 0; clausulas[i][j] != 0; j++)
        {
            int lit = clausulas[i][j];
            int var = abs(lit) - 1;

            if (valores[var] == -1)
            {
                indefinido = 1;
                continue;
            }

            if ((lit > 0 && valores[var] == 1) || (lit < 0 && valores[var] == 0))
            {
                clausula_ok = 1;
                break;
            }
        }

        if (!clausula_ok && !indefinido)
            return 0;
    }

    return 1;
}

int backtrack(int **clausulas, int num_clausulas, int num_vars, int *valores, int pos)
{
    if (pos == num_vars)
    {
        return verifica_total(clausulas, num_clausulas, valores);
    }

    for (int v = 0; v <= 1; v++)
    {
        valores[pos] = v;
        if (verifica_parcial(clausulas, num_clausulas, valores))
        {
            if (backtrack(clausulas, num_clausulas, num_vars, valores, pos + 1))
            {
                return 1;
            }
        }
        valores[pos] = -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s arquivo.cnf\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo)
    {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }

    int num_vars, num_clausulas;
    char linha[256];

    do
    {
        if (!fgets(linha, sizeof(linha), arquivo))
        {
            fprintf(stderr, "Erro na leitura do arquivo.\n");
            fclose(arquivo);
            return 1;
        }
    } while (linha[0] == 'c');

    sscanf(linha, "p cnf %d %d", &num_vars, &num_clausulas);

    int **clausulas = malloc(num_clausulas * sizeof(int *));
    for (int i = 0; i < num_clausulas; i++)
    {
        clausulas[i] = malloc((num_vars + 1) * sizeof(int));
    }

    for (int i = 0; i < num_clausulas; i++)
    {
        int literal, j = 0;
        while (fscanf(arquivo, "%d", &literal) == 1 && literal != 0)
        {
            clausulas[i][j++] = literal;
        }
        clausulas[i][j] = 0;
    }

    fclose(arquivo);

    int *valores = malloc(num_vars * sizeof(int));
    for (int i = 0; i < num_vars; i++)
        valores[i] = -1;

    clock_t inicio = clock();
    int resultado = backtrack(clausulas, num_clausulas, num_vars, valores, 0);
    clock_t fim = clock();

    double tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (resultado)
        printf("SAT\n");
    else
        printf("UNSAT\n");

    printf("Tempo: %.6f s\n", tempo_execucao);

    free(valores);
    for (int i = 0; i < num_clausulas; i++)
        free(clausulas[i]);
    free(clausulas);

    return 0;
}
