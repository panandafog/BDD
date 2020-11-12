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

#define LOG true

//#define N_VAR 18 // число булевых переменных
#define N 9 // число объектов
#define M 4 // число свойств
#define LOG_N 4

#define K 26 // максимальная сумма значений свойств соседей (для 7-го ограничения)

const int N_VAR = N * M * LOG_N;

ofstream out;

string dotDir = "/Users/panandafog/Soft/С++/matlogic/Dot/";

void fun(char* varset, int size);

int main(int argc, const char * argv[]) {
    
    // инициализация
    bdd_init(300000000, 10000000);
    
    bdd_setvarnum(N_VAR);
    
    // ->--- вводим функцию p(k, i, j) следующим образом ( кодируем pk[i][j] ):
    
    // k – номер свойства
    // i – номер объекта
    // j – код свойства
    
    bdd p1[N][N];
    bdd p2[N][N];
    bdd p3[N][N];
    bdd p4[N][N];
    
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
            p4[i][j] = bddtrue;
            for (unsigned k = 0; k < LOG_N; k++) p4[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N*3 + k) : bdd_nithvar(I + LOG_N*3 + k) ;
        }
        I += LOG_N*M;
    }
    
    bdd task = bdd_true();
    cout << "satcount изначально: " << bdd_satcount(task) << endl;
    
    // ограничения
    // 3 1-го типа, 4 2-го, 5 3-го, 4 4-го
    
    if (LOG) { cout << endl << "начал вводить ограничение 1" << endl; }
    
    // ограничения типа 1
    
    task &= p1[1][4];
    task &= p1[2][3];
    task &= p2[0][8];
    
    task &= p1[7][7];
    task &= p4[8][3];
    task &= p4[0][6];
    task &= p4[6][5];
    task &= p2[3][0];
    
    if (LOG) {
        cout << "satcount с ограничением 1: " << bdd_satcount(task) << endl;
        cout << endl << "начал вводить ограничения 2" << endl;
    }
    
    // ограничения типа 2
    
    for (unsigned i = 0; i < N; i++)
    {
        task &= (p2[i][2] & p3[i][1]) | (!p2[i][2] & !p3[i][1]);
        task &= (p2[i][3] & p3[i][8]) | (!p2[i][3] & !p3[i][8]);
        task &= (p2[i][4] & p3[i][7]) | (!p2[i][4] & !p3[i][7]);
        task &= (p2[i][5] & p3[i][6]) | (!p2[i][5] & !p3[i][6]);
        
        task &= (p3[i][2] & p1[i][2]) | (!p3[i][2] & !p1[i][2]);
        task &= (p2[i][6] & p4[i][8]) | (!p2[i][6] & !p4[i][8]);
    }
    
    if (LOG) {
        cout << "satcount с ограничениями 2: " << bdd_satcount(task) << endl;
        cout << endl << "начал вводить ограничения 3, 4" << endl;
    }
    
    for (unsigned i = 0; i < N - 3; i++)
    {
        unsigned j = i + 3;
        
        // ограничения типа 3
        
        task &= !(p4[i][0] ^ p3[j][8]);
        task &= !p3[0][8] & !p3[1][8] & !p3[2][8] & !p4[N - 3][0] & !p4[N - 2][0] & !p4[N - 3][0];
        
        task &= !(p4[i][2] ^ p3[j][7]);
        task &= !p3[0][7] & !p3[1][7] & !p3[2][7] & !p4[N - 3][2] & !p4[N - 2][2] & !p4[N - 3][2];
        
        task &= !(p4[i][1] ^ p3[j][6]);
        task &= !p3[0][6] & !p3[1][6] & !p3[2][6] & !p4[N - 3][1] & !p4[N - 2][1] & !p4[N - 3][1];
        
        task &= !(p4[i][7] ^ p1[j][1]);
        task &= !p1[0][1] & !p1[1][1] & !p1[2][1] & !p4[N - 3][7] & !p4[N - 2][7] & !p4[N - 3][7];
        
        task &= !(p4[i][8] ^ p1[j][0]);
        task &= !p1[0][0] & !p1[1][0] & !p1[2][0] & !p4[N - 3][8] & !p4[N - 2][8] & !p4[N - 3][8];
        
        // ограничения типа 4
        
        task &= (!(p2[i][3] ^ p3[j][2]) & !p3[0][2] & !p3[1][2] & !p3[2][2] & !p2[N - 3][3] & !p2[N - 2][3] & !p2[N - 3][3]) |
        (!(p3[i][2] ^ p2[j][3]) & !p2[0][3] & !p2[1][3] & !p2[2][3] & !p3[N - 3][2] & !p3[N - 2][2] & !p3[N - 3][2]);
        
        task &= (!(p2[i][4] ^ p3[j][0]) & !p3[0][0] & !p3[1][0] & !p3[2][0] & !p2[N - 3][4] & !p2[N - 2][4] & !p2[N - 3][4]) |
        (!(p3[i][0] ^ p2[j][4]) & !p2[0][4] & !p2[1][4] & !p2[2][4] & !p3[N - 3][0] & !p3[N - 2][0] & !p3[N - 3][0]);
        
        task &= (!(p2[i][5] ^ p3[j][1]) & !p3[0][1] & !p3[1][1] & !p3[2][1] & !p2[N - 3][5] & !p2[N - 2][5] & !p2[N - 3][5]) |
        (!(p3[i][1] ^ p2[j][5]) & !p2[0][5] & !p2[1][5] & !p2[2][5] & !p3[N - 3][1] & !p3[N - 2][1] & !p3[N - 3][1]);

        task &= (!(p2[i][2] ^ p3[j][5])& !p3[0][5] & !p3[1][5] & !p3[2][5] & !p2[N - 3][2] & !p2[N - 2][2] & !p2[N - 3][2]) |
        (!(p3[i][5] ^ p2[j][2]) & !p2[0][2] & !p2[1][2] & !p2[2][2] & !p3[N - 3][5] & !p3[N - 2][5] & !p3[N - 3][5]);
    }
    
    if (LOG) {
        cout << "satcount с ограничениями 3, 4: " << bdd_satcount(task) << endl;
        cout << endl << "начал вводить ограничение 6" << endl;
    }
    
    // ограничение типа 6
    
    for (unsigned i = 0; i < N; i++)
    {
        bdd p1T = bdd_false();
        bdd p2T = bdd_false();
        bdd p3T = bdd_false();
        bdd p4T = bdd_false();
        
        for (unsigned j = 0; j < N; j++)
        {
            p1T |= p1[i][j];
            p2T |= p2[i][j];
            p3T |= p3[i][j];
            p4T |= p4[i][j];
        }
        
        task &= p1T & p2T & p3T & p4T;
    }
    
    if (LOG) {
        cout << "satcount с ограничением 6: " << bdd_satcount(task) << endl;
        cout << endl << "начал вводить ограничение 5" << endl;
    }

    // ограничение типа 5
    
    for (unsigned i1 = 0; i1 < N; i1++)
    {
        for (unsigned i2 = 0; i2 < N; i2++)
        {
            for (unsigned j = 0; j < N; j++)
            {
                if (i1 != i2) {
                    task &= !p1[i1][j] | !p1[i2][j];
                    task &= !p2[i1][j] | !p2[i2][j];
                    task &= !p3[i1][j] | !p3[i2][j];
                    task &= !p4[i1][j] | !p4[i2][j];
                }
            }
        }
    }
    
    if (LOG) {
        cout << "satcount с ограничением 5: " << bdd_satcount(task) << endl;
        cout << endl << "начал вводить ограничение 7" << endl;
    }
    
    if (LOG) {
        out.open(dotDir + "out5.txt");
        out << bdd_satcount(task) << " solutions:\n" << endl;
        bdd_allsat(task, fun);
        out.close();
    }
    
    // ограничение типа 7
    
    for (unsigned i = 0; i < N - 3; i++)
    {
        unsigned j = i + 3;
        
        bdd temp = bdd_false();

        if (LOG) { cout << "   " << "i: " << i << ", j: " << j << endl; }

        for (unsigned rhsJ1 = 0; rhsJ1 < N; rhsJ1++)
        {
            for (unsigned rhsJ2 = 0; rhsJ2 < N; rhsJ2++)
            {
                for (unsigned rhsJ3 = 0; rhsJ3 < N; rhsJ3++)
                {
                    for (unsigned rhsJ4 = 0; rhsJ4 < N; rhsJ4++)
                    {
                        for (unsigned lhsJ1 = 0; lhsJ1 < N; lhsJ1++)
                        {
                            for (unsigned lhsJ2 = 0; lhsJ2 < N; lhsJ2++)
                            {
                                for (unsigned lhsJ3 = 0; lhsJ3 < N; lhsJ3++)
                                {
                                    for (unsigned lhsJ4 = 0; lhsJ4 < N; lhsJ4++)
                                    {
                                        unsigned sum = rhsJ1 + rhsJ2 + rhsJ3 + rhsJ4 + lhsJ1 + lhsJ2 + lhsJ3 + lhsJ4;

//                                        if (sum > K) {
//                                            task &= !(p1[i][lhsJ1] & p2[i][lhsJ2] & p3[i][lhsJ3] & p4[i][lhsJ4] &
//                                                      p1[j][rhsJ1] & p2[j][rhsJ2] & p3[j][rhsJ3] & p4[j][rhsJ4]);
//                                        }
                                        if (sum <= K) {
                                            temp |= (p1[i][lhsJ1] & p2[i][lhsJ2] & p3[i][lhsJ3] & p4[i][lhsJ4] &
                                                     p1[j][rhsJ1] & p2[j][rhsJ2] & p3[j][rhsJ3] & p4[j][rhsJ4]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        task &= temp;
    }
    
    // вывод результатов
    out.open(dotDir + "out.txt");
    cout << endl << "satcount в итоге: " << bdd_satcount(task) << endl;
    out << bdd_satcount(task) << " solutions:\n" << endl;
    bdd_allsat(task, fun);
    out.close();
    
    bdd_done(); // завершение работы библиотеки
    
    return 0;
}

char var[N_VAR];

void print(void)
{
    for (unsigned i = 0; i < N; i++)
    {
        out << i << ": ";
        for (unsigned j = 0; j < M; j++)
        {
            unsigned J = i * M * LOG_N + j * LOG_N;
            unsigned num = 0;
            for (unsigned k = 0; k < LOG_N; k++) num += (unsigned)(var[J + k] << k);
            out << num << ' ';
        }
        out << endl;
    }
    out << endl;
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
