
// TODO: Coisas A fazer
//// Coisas completamente erradoas
//? perguntas a serem solucionadas
//! possiveis erros de dar merda
// comentario normal
//* comentario mais claro

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <math.h>

#include "../include/project.h"
#include "../include/argparser.hpp"

using namespace std;

int main(int argc, const char **argv)
{

    // Parser for the variable input
    ArgumentParser parser;
    parser.addArgument("-v", "--verbose", 1, false);
    parser.addArgument("-p", "--projetos", 1, false);
    parser.addArgument("-d", "--dias", 1, false);
    parser.addArgument("-s", "--salas", 1, false);

    // parse the command-line arguments
    parser.parse(argc, argv);

    string verbose = parser.retrieve<string>("verbose");
    string projetos = parser.retrieve<string>("projetos");
    string dias = parser.retrieve<string>("dias");
    string salas = parser.retrieve<string>("salas");

    int verboseInt = stoi(verbose);
    int projetosInt = stoi(projetos);
    int diasInt = stoi(dias);
    int salasInt = stoi(salas);

    std::cout << "Input " << std::endl;
    std::cout << " verbose: " << verbose << std::endl;
    std::cout << " projetos: " << projetosInt << std::endl;
    std::cout << " dias: " << diasInt << std::endl;
    std::cout << " salas: " << salasInt << std::endl;

    try
    {
        PrimeiraEtapa(projetosInt, diasInt, salasInt, verboseInt);
    }
    catch (IloException &e)
    {
        cout << "\n \n"
             << RED << "Error IloException in Main Function:" << RESET << endl
             << e << endl;
        e.end();
    }
    catch (...)
    {
        cerr << "Unknown exception caught" << endl;
    }

    return 0;
}