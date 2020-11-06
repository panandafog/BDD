//
//  main.cpp
//  Einstein's problem
//
//  Created by panandafog on 04.11.2020.
//

#include <fstream>
#include <iostream>

#include "bdd.h"

using namespace std;

#define N_VAR 18 // число булевых переменных
#define N 3 // число объектов
#define M 3 // число свойств
#define LOG_N 2

ofstream out;

string dotDir = "/Users/panandafog/Soft/С++/matlogic/Dot/";

void fun(char* varset, int size);

int main(int argc, const char * argv[]) {
    
    // инициализация
    bdd_init(10000, 1000);
    bdd_setvarnum(N_VAR);
    // ->--- вводим функцию p(k, i, j) следующим образом ( кодируем pk[i][j] ):
    bdd p1[N][N];
    bdd p2[N][N];
    bdd p3[N][N];
    
    unsigned I = 0;
    for (unsigned i = 0; i < N; i++)
    {
        for (unsigned j = 0; j < N; j++)
        {
            p1[i][j] = bddtrue;
            for (unsigned k = 0; k < LOG_N; k++) p1[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + k) : bdd_nithvar(I + k) ;
            p2[i][j] = bddtrue;
            for (unsigned k = 0; k < LOG_N; k++) p2[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N + k) : bdd_nithvar(I + LOG_N + k) ;
            p3[i][j] = bddtrue;
            for (unsigned k = 0; k < LOG_N; k++) p3[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N*2 + k) : bdd_nithvar(I + LOG_N*2 + k) ;
        }
        I += LOG_N*M;
    }
    
    // здесь должны быть ограничения
    bdd task = bdd_true();
//    task &= p1[0][0]; // ограничение типа 1
//    out.open(dotDir + "bdd1.txt");
    
//    out.close()
    
    // вывод результатов
    out.open(dotDir + "out.txt");
    unsigned satcount = (unsigned)bdd_satcount(task);
    out << satcount << " solutions:\n" << endl;
    if (satcount) bdd_allsat(task, fun);
    out.close();

    bdd_done(); // завершение работы библиотеки
    
    return 0;
}

char var[N_VAR];

void print(void)
{
    for (unsigned i = 0; i < N; i++)
    {
        out<<i<<": ";
        for (unsigned j = 0; j < N; j++)
        {
            unsigned J = i*N*LOG_N + j*LOG_N;
            unsigned num = 0;
            for (unsigned k = 0; k < LOG_N; k++) num += (unsigned)(var[J + k] << k);
            out<<num<<' ';
        }
        out<<endl;
    }
    out<<endl;
}

void build(char* varset, unsigned n, unsigned I)
{
    if (I == n - 1)
    {
        if (varset[I] >= 0)
        {
            var[I] = varset[I];
            print();
            return;
        }
        var[I] = 0;
        print();
        var[I] = 1;
        print();
        return;
    }
    if (varset[I] >= 0)
    {
        var[I] = varset[I];
        build(varset, n, I + 1);
        return;
    }
    var[I] = 0;
    build(varset, n, I + 1);
    var[I] = 1;
    build(varset, n, I + 1);
}

void fun(char* varset, int size)
{
    build(varset, size, 0);
}
