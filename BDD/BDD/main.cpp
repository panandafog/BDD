//
//  main.cpp
//  BDD
//
//  Created by panandafog on 27.10.2020.
//

#include <fstream>
#include <iostream>

#include "bdd.h"

using namespace std;

ofstream out;

string dotDir = "/Users/panandafog/Soft/С++/Dot/";

//void fun(char* varset, int size); // см. далее

int main(int argc, const char * argv[]) {
    
    // Инициализация: начальное число вершин и статический кэш для операций
    bdd_init(1000, 100);
    bdd_setvarnum(6); // 6 булевых переменных
    
    bdd a1 = bdd_ithvar(0);
    bdd a2 = bdd_ithvar(1);
    bdd a3 = bdd_ithvar(2);
    bdd b1 = bdd_ithvar(3);
    bdd b2 = bdd_ithvar(4);
    bdd b3 = bdd_ithvar(5);

    bdd z = a1&b1 | a2&b2 | a3&b3; // функция z(a1,a2,a3,b1,b2,b3)
    
    // число вершин BDD
    cout << "\na1 < a2 < a3 < b1 < b2 < b3: " << bdd_nodecount(z) << " nodes\n" << endl;
    
    out.open(dotDir + "bdd1.dot");
    out << bdddot << z << endl; //вывод исходника dot
    out.close();
    
    //меняем порядок переменных
    bdd_swapvar(1, 3);
    bdd_swapvar(2, 1);
    bdd_swapvar(2, 4);
    
    cout<<"\na1 < b1 < a2 < b2 < a3 < b3: " << bdd_nodecount(z) << " nodes" << endl;
    out.open(dotDir + "bdd2.dot");
    out << bdddot << z << endl;
    out.close();
    
    //вывод множеств, на которых z = 1
    out.open(dotDir + "set.txt");
    out << bdd_satcount(z) << " sats:\n" << endl;
    out << bddset << z << endl;
    
    return 0;
}
