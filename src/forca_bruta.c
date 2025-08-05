#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int verificar_satisfatibilidade(int **clausulas, int num_clausulas, int num_vars)
{
    int total_atribuicoes = 1 << num_vars;

    for (int i = 0; i < total_atribuicoes; i++)
    {
        int satisfaz = 1;
        int valores[num_vars];

        for (int j = 0; j < num_vars; j++)
        {
            valores[j] = (i >> j) & 1;
        }

        for (int c = 0; c < num_clausulas; c++)
        {
            int satisfaz_clausula = 0;
            for (int l = 0; clausulas[c][l] != 0; l++)
            {
                int literal = clausulas[c][l];
                int var_index = abs(literal) - 1;
                int valor = valores[var_index];

                if ((literal > 0 && valor == 1) || (literal < 0 && valor == 0))
                {
                    satisfaz_clausula = 1;
                    break;
                }
            }

            if (!satisfaz_clausula)
            {
                satisfaz = 0;
                break;
            }
        }

        if (satisfaz)
        {
            return 1;  // SAT
        }
    }

    return 0;  // UNSAT
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

    clock_t inicio = clock();
    int resultado = verificar_satisfatibilidade(clausulas, num_clausulas, num_vars);
    clock_t fim = clock();

    double tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (resultado)
        printf("SAT\n");
    else
        printf("UNSAT\n");

    printf("Tempo: %.6f s\n", tempo_execucao);

    for (int i = 0; i < num_clausulas; i++)
        free(clausulas[i]);
    free(clausulas);

    return 0;
}
