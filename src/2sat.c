#include <stdio.h>
#include <stdlib.h>

#define MAXVARS 1000
#define MAXNODES (2 * MAXVARS)
#define MAXEDGES 100000

typedef struct {
    int to;
    int next;
} Edge;

Edge edges[MAXEDGES];
int head[MAXNODES];
int edge_count = 0;

Edge rev_edges[MAXEDGES];
int rev_head[MAXNODES];
int rev_count = 0;

int visited[MAXNODES];
int order[MAXNODES], order_idx = 0;
int component[MAXNODES];


int var_index(int x) {
    int v = abs(x) - 1;
    return (x > 0) ? (2 * v) : (2 * v + 1);
}

int neg(int x) {
    return x ^ 1; 
}

void add_edge(int from, int to) {
    edges[edge_count].to = to;
    edges[edge_count].next = head[from];
    head[from] = edge_count++;
}

void add_rev_edge(int from, int to) {
    rev_edges[rev_count].to = to;
    rev_edges[rev_count].next = rev_head[from];
    rev_head[from] = rev_count++;
}

// Kosaraju

void dfs1(int u) {
    visited[u] = 1;
    for (int e = head[u]; e != -1; e = edges[e].next) {
        int v = edges[e].to;
        if (!visited[v])
            dfs1(v);
    }
    order[order_idx++] = u;
}

void dfs2(int u, int cid) {
    component[u] = cid;
    visited[u] = 1;
    for (int e = rev_head[u]; e != -1; e = rev_edges[e].next) {
        int v = rev_edges[e].to;
        if (!visited[v])
            dfs2(v, cid);
    }
}

int verifica_2sat(int num_vars) {
    for (int i = 0; i < 2 * num_vars; i++)
        visited[i] = 0;

    for (int i = 0; i < 2 * num_vars; i++) {
        if (!visited[i])
            dfs1(i);
    }

    for (int i = 0; i < 2 * num_vars; i++)
        rev_head[i] = -1;
    rev_count = 0;

    for (int u = 0; u < 2 * num_vars; u++) {
        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            add_rev_edge(v, u);
        }
    }

    for (int i = 0; i < 2 * num_vars; i++)
        visited[i] = 0;

    int cid = 0;
    for (int i = 2 * num_vars - 1; i >= 0; i--) {
        int u = order[i];
        if (!visited[u])
            dfs2(u, cid++);
    }

    for (int i = 0; i < num_vars; i++) {
        if (component[2 * i] == component[2 * i + 1])
            return 0; 
    }

    return 1;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s arquivo.cnf\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir %s\n", argv[1]);
        return 1;
    }

    char linha[256];
    int num_vars = 0, num_clausulas = 0;

    while (fgets(linha, sizeof(linha), f)) {
        if (linha[0] == 'c')
            continue;
        if (linha[0] == 'p') {
            sscanf(linha, "p cnf %d %d", &num_vars, &num_clausulas);
            break;
        }
    }

    for (int i = 0; i < 2 * num_vars; i++)
        head[i] = -1;
    edge_count = 0;

    int a, b;
    for (int i = 0; i < num_clausulas; i++) {
        int r = fscanf(f, "%d %d 0", &a, &b);
        if (r != 2) {
            fprintf(stderr, "Erro: cláusula com mais de 2 literais (não é 2-SAT)\n");
            return 2;
        }

        int u = var_index(a);
        int v = var_index(b);

        add_edge(neg(u), v); // ¬a → b
        add_edge(neg(v), u); // ¬b → a
    }

    fclose(f);

    if (verifica_2sat(num_vars))
        printf("SAT\n");
    else
        printf("UNSAT\n");

    return 0;
}
