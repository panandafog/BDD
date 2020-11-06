//
//  main.cpp
//  BuDDy Demo
//
//  Created by panandafog on 27.10.2020.
//

#include <fstream>
#include <iostream>

#include "bdd.h"

using namespace std;

ofstream out;

string dotDir = "/Users/panandafog/Soft/С++/matlogic/Dot/";

void fun(char* varset, int size); // см. далее

// MARK: - main
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
    
    
    // Число вершин BDD
    
    cout << "\na1 < a2 < a3 < b1 < b2 < b3: " << bdd_nodecount(z) << " nodes\n" << endl;
    
    out.open(dotDir + "bdd1.dot");
    out << bdddot << z << endl; //вывод исходника dot
    out.close();
    
    
    // Меняем порядок переменных
    
    bdd_swapvar(1, 3);
    bdd_swapvar(2, 1);
    bdd_swapvar(2, 4);
    
    cout << "\na1 < b1 < a2 < b2 < a3 < b3: " << bdd_nodecount(z) << " nodes" << endl;
    out.open(dotDir + "bdd2.dot"); // снова выводим dot
    out << bdddot << z << endl;
    out.close();
    
    // вывод множеств, на которых z = 1 (функция истинна)
    
    out.open(dotDir + "set.txt");
    out << bdd_satcount(z) << " sats:\n" << endl;
    out << bddset << z << endl;
    out.close();
    
    out.open(dotDir + "allsat.txt");
    out << bdd_satcount(z) << " sats:\n" << endl;
    bdd_allsat(z, fun); // для каждого набора значений z выполняет функцию fun
    out.close();
    
    
    // exist-квантификация по b1, b2 и b3: z = a1 | a2 | a3
    
    z = bdd_exist(z, b1&b2&b3);
    out.open(dotDir + "bdd3.txt");
    out << bdddot << z << endl;
    out.close();
    // получится z = a1 ∨ a2 ∨ a3
    
    
    // Заменим (переименуем) переменные a1, a2 и a3 на b1, b2 и b3
    
    int A[] = {0, 1, 2}; // Будем менять переменные с номерами 0, 1, 2
    int B[] = {3, 4, 5}; // на перемнные с номерами 3, 4, 5
    bddPair* pair = bdd_newpair(); // инициализируем множество пар
    bdd_setpairs(pair, A, B, 3); // устанавливаем 3 пары: A[i] <-> B[i]
    z = bdd_replace(z, pair); // заменяем переменные: z = b1 | b2 | b3
    bdd_freepair(pair); // уничтожаем пары
    out.open(dotDir + "bdd4.txt");
    out << bdddot << z << endl;
    out.close();
    
    
    // Выведем таблицу, представляющую итоговую функцию z = b1 ∨ b2 ∨ b3
    
    out.open(dotDir + "Table.txt");
    out<<bddtable<<z<<endl;
    out.close();
    bdd_done(); // завершаем работу с библиотекой
    
    return 0;
}

// MARK: - fun
void fun(char* varset, int size)
{
    for (int i = 0; i < size; i++) {
        out << (varset[i] < 0 ? 'X' : (char)('0' + varset[i]));
    }
    out << endl;
}
