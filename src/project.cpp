

#include "../include/project.h"

/*
Dados:

H   Conjunto de projetos onde i, j ∈ conjunto de projetos.
K   Conjunto de seção onde k = (dia,horário,sala).
    dia       Número de dias.
    horário   Horário definido para a seção (M1, M2, T1, T2)
             M1 = Corresponde ao primeiro horário da manhã
    sala      Número de salas.
Bij Conjunto de pesos que relacionam a temática dos projetos, colocando um peso maior naqueles com a mesma área temática. i, j ∈ conjuntos de projetos.
*/

int PrimeiraEtapa(char verbose)
{
    // Criando o Ambiente
    IloEnv env;
    // Criando o Modelo
    IloModel modelo(env);

    // ██████╗  █████╗ ██████╗  ██████╗ ███████╗
    // ██╔══██╗██╔══██╗██╔══██╗██╔═══██╗██╔════╝
    // ██║  ██║███████║██║  ██║██║   ██║███████╗
    // ██║  ██║██╔══██║██║  ██║██║   ██║╚════██║
    // ██████╔╝██║  ██║██████╔╝╚██████╔╝███████║
    // ╚═════╝ ╚═╝  ╚═╝╚═════╝  ╚═════╝ ╚══════╝

    // DADOS
    // Variavel que armazena o nº de projetos
    int nProjetos = 4;

    int nDias = 3;
    int nHorarios = 4;
    int nSalas = 6;

    // Conjunto de seção onde k = (dia,horário,sala).
    std::vector<std::vector<int>> K;

    // Criando dados de K de acordo com a quantidade de dias...
    for (auto dia = 0; dia < nDias; dia++)
    {
        // horarios...
        for (auto horario = 0; horario < nHorarios; horario++)
        {
            // e numero de salas.
            for (auto sala = 0; sala < nSalas; sala++)
            {
                std::vector<int> aux = {dia,
                                        horario,
                                        sala};

                K.push_back(aux);
            }
        }
    }

    std::cout << "nProjetos: " << nProjetos << std::endl;
    std::cout << "nDias: " << nDias << std::endl;
    std::cout << "nHorarios: " << nHorarios << std::endl;
    std::cout << "nSalas: " << nSalas << std::endl;

    std::cout << "K size: " << K.size() << std::endl;
    std::cout << "K size (math): " << nDias * nHorarios * nSalas << std::endl;

    /*
    //conferindo o resultado
    for (auto i : K)
    {
        std::cout << "Inst: " << std::endl;
        std::cout << " i[0]: " << i[0] << std::endl;
        std::cout << " i[1]: " << i[1] << std::endl;
        std::cout << " i[2]: " << i[2] << std::endl;
    }
    */

    // Conjunto de pesos que relacionam a temática dos projetos, colocando um peso maior naqueles com a mesma área temática.
    // i, j ∈ conjuntos de projetos.
    std::vector<std::vector<int>> B(nProjetos, std::vector<int>(nProjetos));

    for (auto i = 0; i < nProjetos; i++)
    {
        for (auto j = i; j < nProjetos; j++)
        {
            if (i == j)
            {
                continue;
            }
            std::random_device rd;
            std::uniform_int_distribution<int> dist(0, 8);
            //std::cout << dist(rd) << std::endl;
            B[i][j] = dist(rd);

            B[j][i] = B[i][j];
        }
    }

    if (verbose == 'd')
    {
        for (auto i = 0; i < nProjetos; i++)
        {
            for (auto j = 0; j < nProjetos; j++)
            {
                printf("B[%d][%d]: %d \n", i, j, B[i][j]);
                printf("B[%d][%d]: %d \n", j, i, B[j][i]);
                printf("\n");
            }
        }
    }

    //
    // ██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██╗   ██╗███████╗██╗███████╗
    // ██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██║   ██║██╔════╝██║██╔════╝
    // ██║   ██║███████║██████╔╝██║███████║██║   ██║█████╗  ██║███████╗
    // ╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║╚██╗ ██╔╝██╔══╝  ██║╚════██║
    //  ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║ ╚████╔╝ ███████╗██║███████║
    //   ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝╚══════╝
    // VARIAVEIS

    // y_kj
    IloArray<IloBoolVarArray> y(env, K.size());

    for (int k = 0; k < K.size(); k++)
    {
        // Construindo a dimensão interna com vetores do tamanho de nProjetos
        IloBoolVarArray vetorAux(env, nProjetos);
        y[k] = vetorAux;
    }

    // Adicionando variavel no modelo
    for (int k = 0; k < K.size(); k++)
    {
        for (int j = 0; j < nProjetos; j++)
        {
            char var[100];
            sprintf(var, "y(%d,%d)", k, j);
            y[k][j].setName(var);
            modelo.add(y[k][j]);
        }
    }
    if (verbose == 'v')
        std::cout << "y created" << std::endl;

    //* Variaveis
    // x_kij , variavel binaria
    // Dimensão k pois k vai até K.size()
    IloArray<IloArray<IloBoolVarArray>> x(env, K.size());

    for (int k = 0; k < K.size(); k++)
    {
        IloArray<IloBoolVarArray> vetorAux(env, nProjetos);
        x[k] = vetorAux;

        for (int i = 0; i < nProjetos; i++)
        {
            IloBoolVarArray vetorAux2(env, nProjetos);
            x[k][i] = vetorAux2;
        }
    }

    // Adicionando variavel no modelo
    for (int k = 0; k < K.size(); k++)
    {
        for (int i = 0; i < nProjetos; i++)
        {
            for (int j = 0; j < nProjetos; j++)
            {
                char var[100];
                sprintf(var, "x(%d,%d,%d)", k, i, j);
                x[k][i][j].setName(var);
                modelo.add(x[k][i][j]);
            }
        }
    }
    if (verbose == 'v')
        std::cout << "x created" << std::endl;

    // ███████╗ ██████╗
    // ██╔════╝██╔═══██╗
    // █████╗  ██║   ██║
    // ██╔══╝  ██║   ██║
    // ██║     ╚██████╔╝
    // ╚═╝      ╚═════╝
    // Criando a Função Objetivo (FO)
    // max
    IloExpr soma(env);
    for (int k = 0; k < K.size(); k++)
    {
        for (int i = 0; i < nProjetos; i++)
        {
            for (int j = 0; j < nProjetos; j++)
            {
                soma += B[i][j] * x[k][i][j];
            }
        }
    }

    IloObjective obj(env, soma, IloObjective::Maximize);
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

    // (1)
    // , para i = 1, ... , nProjetos; para j = 1, nProjetos, para k pertence K
    for (int k = 0; k < K.size(); k++)
    {
        for (int i = 0; i < nProjetos; i++)
        {
            for (int j = 0; j < nProjetos; j++)
            {
                if (i < j)
                {
                    modelo.add(x[k][i][j] <= y[k][i]);
                }
            }
        }
    }
    if (verbose == 'v')
        std::cout << "Restriction 1 created" << std::endl;

    // (2)
    // , para i = 1, ... , nProjetos; para j = 1, nProjetos, para k pertence K
    for (int k = 0; k < K.size(); k++)
    {
        for (int i = 0; i < nProjetos; i++)
        {
            for (int j = 0; j < nProjetos; j++)
            {
                if (i < j)
                {
                    modelo.add(x[k][i][j] <= y[k][j]);
                }
            }
        }
    }
    if (verbose == 'v')
        std::cout << "Restriction 2 created" << std::endl;

    // (3)
    // , para i = 1, ... , nProjetos; para j = 1, nProjetos, para k pertence K
    for (int k = 0; k < K.size(); k++)
    {
        for (int i = 0; i < nProjetos; i++)
        {
            for (int j = 0; j < nProjetos; j++)
            {
                if (i < j)
                {
                    modelo.add(x[k][i][j] + 1 <= y[k][i] + y[k][j]);
                }
            }
        }
    }
    if (verbose == 'v')
        std::cout << "Restriction 3 created" << std::endl;

    ///*
    // (4)
    // , para i = 1, ... , nProjetos; para j = 1, nProjetos, para k pertence K
    for (int k = 0; k < K.size(); k++)
    {
        IloExpr soma(env);
        for (int i = 0; i < nProjetos; i++)
        {
            soma += y[k][i];
        }
        modelo.add(soma == 4);
    }
    if (verbose == 'v')
        std::cout << "Restriction 4 created" << std::endl;
    //*/

    ///*
    // (5)
    // para todos projetos, a soma em cada seção deve ser igual a 1
    // , para i = 1, ... , nProjetos; para j = 1, nProjetos, para k pertence K
    for (int i = 0; i < nProjetos; i++)
    {
        IloExpr soma(env);
        for (int k = 0; k < K.size(); k++)
        {
            soma += y[k][i];
        }
        modelo.add(soma == 1);
    }
    if (verbose == 'v')
        std::cout << "Restriction 5 created" << std::endl;
    //*/

    //CPLEX SOLVER
    if (verbose == 'v')
    {
        std::cout << "Initialize CPLEX solver" << std::endl;
    }

    std::cout << "\n################################################################\n";
    std::cout << "####################### CPLEX SOLVER ###########################\n";
    IloCplex cplex(modelo);
    cplex.setParam(IloCplex::Param::Threads, 0);

    cplex.extract(modelo);
    cplex.exportModel("modeloPrimeiraEtapa.lp");

    cplex.solve();
    cplex.out() << "\nsolution status = " << cplex.getStatus() << std::endl;

    cplex.out() << GREEN << "ObjValue = " << RESET << cplex.getObjValue() << std::endl;

    cplex.out() << "################################################################\n\n";

    std::cout << "Resposta" << std::endl;

    for (int k = 0; k < K.size(); k++)
    {
        for (int j = 0; j < nProjetos; j++)
        {
            cplex.out() << y[k][j] << " Value: " << cplex.getValue(y[k][j]) << std::endl;
            /*
            if (cplex.getValue(y[k][j]) == 1)
            {
                //cplex.out() << y[k][j] << " Value: " << cplex.getValue(y[k][j]) << std::endl;
            }
            */
        }
    }
}

int SegundaEtapa()
{

    // Variavel que armazena o nº professores
    int professores;
}