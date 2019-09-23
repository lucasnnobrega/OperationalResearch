
// TODO: Coisas A fazer
//// Coisas completamente erradoas
//? perguntas a serem solucionadas
//! possiveis erros de dar merda
// comentario normal
//* comentario mais claro

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <math.h>

#include "../include/read_write.h"
#include "../include/project.h"

#include "../include/argparser.hpp"

using namespace std;

void WFSP(int number_of_symbols, int m, int priorities[], char verbose)
{
    // Criando o Ambiente
    IloEnv env;
    IloModel modelo(env);

    //     ██╗ ██╗          ██████╗  █████╗ ██████╗  ██████╗ ███████╗
    //    ██╔╝██╔╝▄ ██╗▄    ██╔══██╗██╔══██╗██╔══██╗██╔═══██╗██╔════╝
    //   ██╔╝██╔╝  ████╗    ██║  ██║███████║██║  ██║██║   ██║███████╗
    //  ██╔╝██╔╝  ▀╚██╔▀    ██║  ██║██╔══██║██║  ██║██║   ██║╚════██║
    // ██╔╝██╔╝     ╚═╝     ██████╔╝██║  ██║██████╔╝╚██████╔╝███████║
    // ╚═╝ ╚═╝              ╚═════╝ ╚═╝  ╚═╝╚═════╝  ╚═════╝ ╚══════╝
    //* Dados

    // Numero de Simbolos
    if (verbose == 'v')
        std::cout << GREEN << "Inside WFSP function" << RESET << std::endl;
    int n = number_of_symbols;
    if (verbose == 'v')
        std::cout << "Number of symbols: " << number_of_symbols << " n: " << n << std::endl;

    // Conjunto de Simbolos X1 ... Xn
    int X[n];
    if (verbose == 'v')
        cout << "X created" << endl;

    // Numero Maximo de copias do simbolo Xi E X
    int M[n];

    for (int i = 0; i < n; i++)
    {
        M[i] = n; // CONFERIR ok
    }
    if (verbose == 'v')
        cout << "M created: M[0] = " << M[0] << endl;

    // Conjunto de indices das cópias xi E X,  Ki = {1, ..., Mi}
    // i  = {1,...N}
    // Array de ponteiros
    volatile int *K[n]; // Force declaration without optimization

    for (int i = 0; i < n; i++)
    {

        int *aux = new int(M[i]);
        K[i] = aux;

        for (int j = 0; j < M[i]; j++)
        {
            K[i][j] = j;
        }
    }

    // Tamanho maximo das sequencias viaveis
    int TMAX = m * n;

    // Prioridades do símbolo Xi E X
    int c[n];
    //vector<int> c[n];

    std::cout << std::endl;
    for (int i = 0; i < n; i++)
    {
        c[i] = priorities[i];
        char var[100];
        sprintf(var, "Priority: %d", c[i]);
        if (verbose == 'v')
            std::cout << "Priority: " << c[i] << " ";
    }
    std::cout << std::endl;

    // Conjunto de posicoes que podem ser ocupadas pela k-ésima cópia do simbolo Xi E X
    // Hik = {k, ... ,TMAX}
    int **H[n];
    if (verbose == 'v')
        std::cout << "Creating the array Hik:" << std::endl;

    for (int i = 0; i < n; i++)
    {
        H[i] = new int *[M[i]];

        for (int k = 0; k < M[i]; k++)
        {
            int aux = 0;

            H[i][k] = new int[TMAX - k];

            for (int j = k; j < TMAX; j++)
            {
                H[i][k][aux] = j;
                aux++;
            }
        }
    }

    //* Verify input data
    if (verbose == 'v')
    {
        std::cout << "#####################################" << std::endl;
        std::cout << "Verify input data: " << std::endl;
        std::cout << "Number of symbols: " << n << std::endl;
        std::cout << "TMAX: " << TMAX << std::endl;
        std::cout << "#####################################" << std::endl;

        std::cout << "Created data, now creating the variables: " << std::endl;
    }

    // ██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██╗   ██╗███████╗██╗███████╗    ██████╗ ██╗
    // ██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██║   ██║██╔════╝██║██╔════╝    ██╔══██╗██║
    // ██║   ██║███████║██████╔╝██║███████║██║   ██║█████╗  ██║███████╗    ██████╔╝██║
    // ╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║╚██╗ ██╔╝██╔══╝  ██║╚════██║    ██╔═══╝ ██║
    //  ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║ ╚████╔╝ ███████╗██║███████║    ██║██╗  ██║
    //   ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝╚══════╝    ╚═╝╚═╝  ╚═╝

    //* Variaveis
    // y_ikh , variavel binaria
    // Dimensão n pois i vai até n
    IloArray<IloArray<IloBoolVarArray>> y(env, n);

    for (int i = 0; i < n; i++)
    {
        IloArray<IloBoolVarArray> vetorAux(env, M[i]);
        y[i] = vetorAux;

        for (int k = 0; k < M[i]; k++)
        {
            IloBoolVarArray vetorAux2(env, TMAX - k);
            y[i][k] = vetorAux2;
        }
    }

    // Adicionando variavel no modelo
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            for (int h = 0; h < TMAX - k; h++)
            {
                char var[100];
                sprintf(var, "y(%d,%d,%d)", i, k, H[i][k][h]);
                y[i][k][h].setName(var);
                modelo.add(y[i][k][h]);
            }
        }
    }
    if (verbose == 'v')
        std::cout << "y created" << std::endl;

    // d_i,k,k+1
    // Dimensao n, pois i vai ate n
    //Distance between the k-th and (k + 1)-th copies of symbol xi ∈ X, k ∈ Ki \ {Mi}.
    IloArray<IloIntVarArray> d(env, n);

    for (int i = 0; i < n; i++)
    {
        //IloIntVarArray vetorAux(env, M[i], 0, IloIntMax);
        IloIntVarArray vetorAux(env, M[i], 0, IloIntMax);
        d[i] = vetorAux;
    }

    // Adicionando variavel no modelo
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            char var[100];
            if (k == M[i] - 1)
            {
                sprintf(var, "d(%d, %d, %d)", i, k, 0);
            }
            else
            {
                sprintf(var, "d(%d, %d, %d)", i - 1, k, k);
            }
            d[i][k].setName(var);
            modelo.add(d[i][k]);
        }
    }
    if (verbose == 'v')
        std::cout << "d created" << std::endl;

    // D_i
    // Maior Distancia entre duas copias consecutivas do simbolo x_i E X
    //IloIntVarArray D(env, n, 0, IloIntMax);
    IloIntVarArray D(env, n, 0, IloIntMax);

    // Adicionando variavel no modelo
    for (int i = 0; i < n; i++)
    {
        char var[100];
        sprintf(var, "D(%d)", i);
        D[i].setName(var);
        modelo.add(D[i]);
    }
    if (verbose == 'v')
        std::cout << "D created" << std::endl;

    // P
    //? Implementado como um valor unico
    // Maior produto D_i*c_i, para todo x_i E X
    IloIntVar P(env, 0, IloIntMax);

    // Adicionando variavel no modelo
    char var[100];
    sprintf(var, "P");
    P.setName(var);
    modelo.add(P);
    if (verbose == 'v')
        std::cout << "P created" << std::endl;

    // p_ik
    // Posicao da k-esima copia do simbolo x_i E X
    IloArray<IloIntVarArray> p(env, n);
    for (int i = 0; i < n; i++)
    {
        IloIntVarArray vetorAux(env, M[i], 0, IloIntMax);
        p[i] = vetorAux;
    }

    // Adicionando variavel no modelo
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            char var[100];
            sprintf(var, "p(%d,%d)", i, k);
            p[i][k].setName(var);
            modelo.add(p[i][k]);
        }
    }

    if (verbose == 'v')
        std::cout << "p created" << std::endl;

    // new variable
    //     ██╗ ██╗    ███╗   ██╗███████╗██╗    ██╗    ██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██████╗ ██╗     ███████╗
    //    ██╔╝██╔╝    ████╗  ██║██╔════╝██║    ██║    ██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██╔══██╗██║     ██╔════╝
    //   ██╔╝██╔╝     ██╔██╗ ██║█████╗  ██║ █╗ ██║    ██║   ██║███████║██████╔╝██║███████║██████╔╝██║     █████╗
    //  ██╔╝██╔╝      ██║╚██╗██║██╔══╝  ██║███╗██║    ╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║██╔══██╗██║     ██╔══╝
    // ██╔╝██╔╝       ██║ ╚████║███████╗╚███╔███╔╝     ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║██████╔╝███████╗███████╗
    // ╚═╝ ╚═╝        ╚═╝  ╚═══╝╚══════╝ ╚══╝╚══╝       ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝

    IloFloatVarArray E(env, TMAX - 1, 0, IloIntMax);

    // Adicionando variavel no modelo
    for (int h = 0; h < TMAX - 1; h++)
    {
        char var[100];
        sprintf(var, "E(%d)", h);
        E[h].setName(var);
        modelo.add(E[h]);
    }
    if (verbose == 'v')
        std::cout << "E created" << std::endl;

    // sum{i in X, k in K[i]} y_ikh ≤ 1, ∀h ∈ {1, . . . , T MAX}
    for (int h = 0; h < TMAX - 1; h++)
    {
        IloExpr soma(env);
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < M[i]; k++)
            {
                if (h < TMAX - k && h >= H[i][k][0])
                    soma += y[i][k][h];
            }
        }
        modelo.add(E[h] >= soma - 1);
    }
    if (verbose == 'v')
        std::cout << "New restriction created" << std::endl;

    // ███████╗   ██████╗
    // ██╔════╝  ██╔═══██╗
    // █████╗    ██║   ██║
    // ██╔══╝    ██║   ██║
    // ██║       ╚██████╔╝
    // ╚═╝        ╚═════╝
    // Criando a Função Objetivo (FO)
    // min P
    IloExpr soma(env);
    for (int h = 0; h < TMAX - 1; h++)
    {
        soma += E[h];
    }

    IloObjective obj(env, (P + soma * TMAX * c[0]), IloObjective::Minimize);
    modelo.add(obj);

    if (verbose == 'v')
        std::cout << "Objective function created" << std::endl;

    // ███████╗    █████╗
    // ██╔════╝   ██╔══██╗
    // ███████╗   ███████║
    // ╚════██║   ██╔══██║
    // ███████║██╗██║  ██║██╗
    // ╚══════╝╚═╝╚═╝  ╚═╝╚═╝
    // Constraints

    // (4.2) (3)
    // P >= D_i * c_i, para i = 1, ... , n
    for (int i = 0; i < n; i++)
    {
        modelo.add(P >= D[i] * c[i]);
    }
    if (verbose == 'v')
        std::cout << "Restriction 3 created" << std::endl;

    // (4.3) (4)
    // D_i ≥ d_i,k,k+1 , para todo i = 1, . . . , n,  para todo k ∈ K_i \ {M_i }
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i] - 1; k++)
        {
            modelo.add(D[i] >= d[i][k]);
        }
    }
    if (verbose == 'v')
        std::cout << "Restriction 4 created" << std::endl;

    // (4.4) (5)
    // sum{i in X, k in K[i]} y_ikh ≤ 1, ∀h ∈ {1, . . . , T MAX}
    for (int h = 0; h < TMAX - 1; h++)
    {
        IloExpr soma(env);
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < M[i]; k++)
            {
                if (h < TMAX - k && h >= H[i][k][0])
                    soma += y[i][k][h];

                /* BRANCH function-modulation
                if (h >= H[i][k][0])
                {
                    soma += y[i][k][h - k];
                }
                */
            }
        }
        // modelo.add(soma <= 1);
    }
    if (verbose == 'v')
        std::cout << "Restriction 5 created" << std::endl;

    // sum{h in H[i][k]} y_ikh ≤ 1,, (4.4) (6)
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            IloExpr soma(env);
            for (int h = 0; h < TMAX - k; h++)
            {
                soma += y[i][k][h];
            }
            modelo.add(soma <= 1);
        }
    }
    if (verbose == 'v')
        std::cout << "Restriction 6 created" << std::endl;

    // sum{k in K[i], h in H[i][k]} y_ikh ≤ 1,  (4.4) (7)
    for (int i = 0; i < n; i++)
    {
        IloExpr soma(env);
        for (int k = 0; k < M[i]; k++)
        {
            for (int h = 0; h < TMAX - k; h++)
            {
                soma += y[i][k][h];
            }
        }
        modelo.add(soma >= 1);
    }
    if (verbose == 'v')
        std::cout << "Restriction 7 created" << std::endl;

    // Missing for all k_i in K in the Article (8) Tese (4.7)
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            IloExpr soma(env);
            for (int h = 0; h < TMAX - k; h++)
            {
                if (h > H[i][k][0])
                    soma += y[i][k][h] * h;
            }
            modelo.add(p[i][k] == soma);
        }
    }
    if (verbose == 'v')
        std::cout << "Restriction 8 created" << std::endl;

    // (9) Tese (4.8)
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i] - 1; k++)
        {

            IloExpr soma_esq(env);
            IloExpr soma_dir(env);
            for (int h = 0; h < TMAX - k - 1; h++)
            {

                soma_esq += y[i][k][h];
                soma_dir += y[i][k + 1][h];
            }
            modelo.add(soma_esq >= soma_dir);
        }
    }
    if (verbose == 'v')
        std::cout << "Restriction 9 created" << std::endl;

    // (10) Tese (4.9)
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i] - 1; k++)
        {
            IloExpr soma(env);
            for (int h = 0; h < TMAX - k - 1; h++)
            {
                soma += y[i][k + 1][h];
            }
            //modelo.add(p[i][k + 1] >= 2 + p[i][k] - (1 - soma) * TMAX);
            modelo.add(p[i][k + 1] >= p[i][k] - (1 - soma) * TMAX);
        }
    }
    if (verbose == 'v')
        std::cout << "Restriction 10 created" << std::endl;

    // (11) Tese (4.10)
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i] - 1; k++)
        {
            for (int h = 0; h < TMAX - k; h++)
            {
                modelo.add(d[i][k] >= p[i][k + 1] - p[i][k]);
            }
        }
    }
    if (verbose == 'v')
        std::cout << "Restriction 11 created" << std::endl;

    // (12) Tese ()
    // Feito na implementação das variáveis

    // (13) Tese ()
    // Feito na implementação das variável

    // (14) Tese ()
    // Feito na implementação das variável

    // (15) Tese ()
    // Feito na implementação das variável

    // NEW VARIABLES
    // ██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██╗   ██╗███████╗██╗███████╗    ██████╗ ██╗██╗
    // ██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██║   ██║██╔════╝██║██╔════╝    ██╔══██╗██║██║
    // ██║   ██║███████║██████╔╝██║███████║██║   ██║█████╗  ██║███████╗    ██████╔╝██║██║
    // ╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║╚██╗ ██╔╝██╔══╝  ██║╚════██║    ██╔═══╝ ██║██║
    //  ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║ ╚████╔╝ ███████╗██║███████║    ██║██╗  ██║██║
    //   ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝╚══════╝    ╚═╝╚═╝  ╚═╝╚═╝
    // Variaveis PII

    if (verbose == 'v')
        cout << GREEN << "Declaring New Variables (W, U, R, alpha[], w[])" << RESET << endl;
    //W
    IloIntVarArray W(env, n, 0, IloIntMax);

    // Adicionando variavel no modelo
    for (int i = 0; i < n; i++)
    {
        char var[100];
        sprintf(var, "W(%d)", i);
        W[i].setName(var);
        modelo.add(W[i]);
    }
    if (verbose == 'v')
        cout << "W created" << endl;

    //Ui
    IloIntVarArray U(env, n, 0, IloIntMax);

    // Adicionando variavel no modelo
    for (int i = 0; i < n; i++)
    {
        char var[100];
        sprintf(var, "U(%d)", i);
        U[i].setName(var);
        modelo.add(U[i]);
    }
    if (verbose == 'v')
        cout << "U created" << endl;

    //Ri
    IloIntVarArray R(env, n, 0, IloIntMax);

    // Adicionando variavel no modelo
    for (int i = 0; i < n; i++)
    {
        char var[100];
        sprintf(var, "R(%d)", i);
        R[i].setName(var);
        modelo.add(R[i]);
    }
    if (verbose == 'v')
        cout << "R created" << endl;

    // alpha
    // Dimensão n pois i vai até n
    IloArray<IloIntVarArray> alpha(env, n);

    for (int i = 0; i < n; i++)
    {
        IloIntVarArray vetorAux(env, M[i], 0, IloIntMax);
        alpha[i] = vetorAux;
    }

    // Adicionando variavel no modelo
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            char var[100];
            sprintf(var, "alpha(%d,%d)", i, k);
            alpha[i][k].setName(var);
            modelo.add(alpha[i][k]);
        }
    }
    if (verbose == 'v')
        cout << "alpha created" << endl;

    // w
    // Dimensão n pois i vai até n
    IloArray<IloBoolVarArray> w(env, n);

    for (int i = 0; i < n; i++)
    {
        IloBoolVarArray vetorAux(env, M[i]);
        w[i] = vetorAux;
    }

    // Adicionando variavel no modelo
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            char var[100];
            sprintf(var, "w(%d,%d)", i, k);
            w[i][k].setName(var);
            modelo.add(w[i][k]);
        }
    }
    if (verbose == 'v')
        cout << "w created" << endl;

    // ███╗   ██╗███████╗██╗    ██╗    ██████╗ ███████╗███████╗████████╗██████╗ ██╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗███████╗
    // ████╗  ██║██╔════╝██║    ██║    ██╔══██╗██╔════╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
    // ██╔██╗ ██║█████╗  ██║ █╗ ██║    ██████╔╝█████╗  ███████╗   ██║   ██████╔╝██║██║        ██║   ██║██║   ██║██╔██╗ ██║███████╗
    // ██║╚██╗██║██╔══╝  ██║███╗██║    ██╔══██╗██╔══╝  ╚════██║   ██║   ██╔══██╗██║██║        ██║   ██║██║   ██║██║╚██╗██║╚════██║
    // ██║ ╚████║███████╗╚███╔███╔╝    ██║  ██║███████╗███████║   ██║   ██║  ██║██║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║███████║
    // ╚═╝  ╚═══╝╚══════╝ ╚══╝╚══╝     ╚═╝  ╚═╝╚══════╝╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

    //* New Restrictions
    if (verbose == 'v')
        cout << "New Restrictions" << endl;

    // (15)
    for (int i = 0; i < n; i++)
    {
        modelo.add(D[i] >= W[i]);
    }
    if (verbose == 'v')
        cout << "Restriction 15 created" << endl;

    // (16)
    for (int i = 0; i < n; i++)
    {
        modelo.add(W[i] >= U[i] - R[i]);
    }
    if (verbose == 'v')
        cout << "Restriction 16 created" << endl;
    // (17)
    for (int i = 0; i < n; i++)
    {
        IloExpr soma(env);
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < M[i]; k++)
            {
                for (int h = 0; h < TMAX - k; h++)
                {
                    soma += y[j][k][h];
                }
            }
        }
        modelo.add(U[i] == soma + p[i][0]);
    }
    if (verbose == 'v')
        cout << "Restriction 17 created" << endl;

    // (18)
    for (int i = 0; i < n; i++)
    {
        IloExpr soma(env);
        for (int k = 0; k < M[i]; k++)
        {
            soma += alpha[i][k];
        }
        modelo.add(R[i] == soma);
    }
    if (verbose == 'v')
        cout << "Restriction 18 created" << endl;

    // (19)
    for (int i = 0; i < n; i++)
    {
        IloExpr soma(env);
        for (int k = 0; k < M[i]; k++)
        {
            soma += w[i][k];
        }
        modelo.add(soma == 1);
    }
    if (verbose == 'v')
        cout << "Restriction 19 created" << endl;

    // (20)
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            modelo.add(alpha[i][k] <= 1 - w[i][k] + p[i][k]);
        }
    }
    if (verbose == 'v')
        cout << "Restriction 20 created" << endl;

    // (21)
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            modelo.add(alpha[i][k] <= w[i][k] * TMAX);
        }
    }
    if (verbose == 'v')
        cout << "Restriction 21 created" << endl;

    // (22)
    // Feito na implementação das variáveis

    // (22)
    // Feito na implementação das variáveis

    // Creating a CPLEX solver
    cout << "\n################################################################\n";
    cout << "####################### CPLEX SOLVER ###########################\n";
    IloCplex cplex(modelo);
    cplex.setParam(IloCplex::Param::Threads, 0);

    cplex.extract(modelo);
    cplex.exportModel("modelo.lp");

    cplex.solve();
    cplex.out() << "\nsolution status = " << cplex.getStatus() << endl;

    cplex.out() << GREEN << "ObjValue = " << RESET << cplex.getObjValue() << endl;

    cplex.out() << "################################################################\n\n";

    if (verbose == 'v')
    {

        cout << "W: " << endl;
        for (size_t i = 0; i < n; i++)
        {
            //cout << " " << W[i] << " ";
            cout << " " << cplex.getValue(W[i]) << " ";
        }

        cout << "\nU: " << endl;
        for (size_t i = 0; i < n; i++)
        {
            //cout << " " << U[i] << " ";
            cout << " " << cplex.getValue(U[i]) << " ";
        }

        cout << "\nR: " << endl;
        for (size_t i = 0; i < n; i++)
        {
            //cout << " " << R[i] << " ";
            cout << " " << cplex.getValue(R[i]) << " ";
        }

        cout << endl;
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < M[i]; k++)
            {
                printf("\np[%d][%d]: ", i, k);
                cout << " -> " << cplex.getValue(p[i][k]) << "\t | " << p[i][k];
            }
        }
    }
    /*
    cplex.out() << "\n\n################################################################\n";
    cplex.out() << "#################### Y #########################################\n\n";

    for (int i = 0; i < n; i++)
    { // 2 == n
        for (int k = 0; k < M[i]; k++)
        {
            for (int h = 0; h < TMAX - k; h++)
            {
                printf("y[%d][%d][%d] = ", i, k, h);
                cplex.out() << cplex.getValue(y[i][k][h]) << endl;
            }
        }
    }

    cplex.out() << "################################################################\n\n";

    cplex.out() << "\n\n################################################################\n";
    cplex.out() << "#################### H #########################################\n\n";

    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            int aux = 0;

            for (int j = k; j < TMAX; j++)
            {
                printf("H[%d][%d][%d] = ", i, k, aux);
                cplex.out() << H[i][k][aux] << endl;
                aux++;
            }
        }
    }

    cplex.out() << "################################################################\n\n";
    */

    draftToLogger(modelo, y, cplex, number_of_symbols, n, TMAX, M, priorities, D, verbose);

    env.out();
}

int main(int argc, const char **argv)
{

    PrimeiraEtapa('v');
    return 0;

    // Parser for the variable input
    ArgumentParser parser;
    parser.addArgument("-v", "--verbose", 1, false);
    parser.addArgument("-i", "--input", 1, false);
    // parse the command-line arguments
    parser.parse(argc, argv);
    string verbose = parser.retrieve<string>("verbose");
    string relative_file_path = parser.retrieve<string>("input");
    char verbose_char_read_instances = verbose[0];
    char verbose_char_WFSP = verbose[1];

    if (verbose_char_read_instances == 'v' && verbose_char_WFSP == 'v')
    {

        cout << GREEN << "\n\nInside the main function" << RESET << endl;
        cout << "verbose:            " << verbose << endl;
        cout << "relative_file_path: " << relative_file_path << endl;
        cout << "verbose_char_read_instances: " << verbose_char_read_instances << endl;
        cout << "verbose_char_WFSP: " << verbose_char_WFSP << endl;
    }

    try
    {
        if (verbose_char_read_instances == 'v' && verbose_char_WFSP == 'v')
            cout << GREEN << "Inside the try" << RESET << endl;

        File_content *aux = NULL;
        aux = read_instances_clean(relative_file_path, verbose_char_read_instances);

        if (aux)
        {
            if (verbose_char_read_instances == 'v' && verbose_char_WFSP == 'v')
            {
                std::cout << "Inside the try -> IF" << std::endl;
                std::cout << " number of symbols: " << aux->number_of_symbols << std::endl;
                std::cout << " number m: " << aux->m << std::endl;
                cout << " priorities: ";
                for (auto i = 0; i < aux->number_of_symbols; i++)
                {
                    cout << aux->priorities[i] << " ";
                }
                cout << endl;
            }

            WFSP(aux->number_of_symbols, aux->m, aux->priorities, verbose_char_WFSP);
            free(aux);
        }
    }
    catch (IloException &e)
    {
        cout << "\n \n"
             << RED << "Error IloException: " << RESET << endl
             << e << endl;
        e.end();
    }
    catch (...)
    {
        cerr << "Unknown exception caught" << endl;
    }

    return 0;
}