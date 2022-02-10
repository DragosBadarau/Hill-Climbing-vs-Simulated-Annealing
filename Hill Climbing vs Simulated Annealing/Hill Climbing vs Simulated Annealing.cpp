#include <iostream>
#include <stdlib.h>
#include <random>
#include <bitset>
#include <climits>
#include <time.h>
#include <fstream>
#include <iomanip>
#include<cmath>
#include<math.h>
#define steps 1000
#define PI 3.14159
using namespace std;
ofstream MyFile("VALORI.txt");
ofstream MyTime("TIMP.txt");
void random1(int restrictie, int val[2]) {//o folosesc pt SA pt a afla bitul random
    random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(0, restrictie);
    val[0] = distr(eng);
    //cout << val[0]<<endl;
}
void random0(float random[2])// random[0,1)
{
    random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(0, 1);
    random[0] = distr(eng);
    //cout << random[0] << endl;
}
bool bitrand() {//generez bit random
    random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(-1, 1);
    bool a = ceil(distr(eng));
    //cout << a << " ";
    return a;
}
int numarbiti(double a, double b) {//formula nr de biti/valoare
    return ceil(log2((b - a) * pow(10, 5)));
}
void randbitstring(bool v[], int nrbiti, int n) {//generez dimensiunile/valorile random in biti
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < nrbiti; j++)
        {
            v[i * nrbiti + j] = bitrand();
        }

        // cout << endl;
    }
    //cout << endl;
   // for (int i = 0; i < n; i++){
        //for (int j = 0; j < nrbiti; j++)
            //cout << v[i * nrbiti + j]; cout << endl;}
}
void binarytoreal(bool v[], int n, double a, double b, double val[]) {
    //convertesc din bistring in nr reale cu formula de pe site
    int nrbiti = numarbiti(a, b), i, j, k = 0;
    double real, decimal;
    for (i = 0; i < n; i++)
    {
        real = 0; decimal = 0;
        for (j = 0; j < nrbiti; j++)
        {
            //cout<<v[k++];
            decimal = decimal + v[i * nrbiti + j] * pow(2, (nrbiti - j - 1));

        }
        real = a + decimal * (b - a) / (pow(2, nrbiti) - 1);
        val[i] = real;
        //cout << val[i] << endl;
    }
    // cout << endl;   
}
//la toate cele 5 functii de mai jos le dau ca parametru bool v[] care e bistringul de valori si n nr de valori
//in toate apelez convertirea din bitstring in valori reale pt a afla suma respectiva
double DeJong1(bool v[], int n) {
    int i;
    double rez = 0, val[31];
    binarytoreal(v, n, -5.12, 5.12, val);//retin in val valorile reale convertite din bitstring
    for (i = 0; i < n; i++)
        rez = rez + val[i] * val[i];
    //cout << "Suma DeJong este: " << rez << " " << endl;
    return rez;
}
double Schwefel(bool v[], int n) {
    int i, nrbiti = numarbiti(-500, 500);
    double rez = 0, val[31];
    binarytoreal(v, n, -500, 500, val);
    for (i = 0; i < n; i++)
        rez = rez - val[i] * sin(sqrt(abs(val[i])));
    // cout << "Schwefel: " << rez << " " << endl;
    return rez;
}
double Michalewicz(bool v[], int n)
{
    int nrbiti = numarbiti(0, PI);
    double rez = 0, val[31];
    binarytoreal(v, n, 0, PI, val);
    for (int i = 1; i <= n; i++)
    {
        rez += (sin(val[i - 1]) * pow(sin(i * (val[i - 1] * val[i - 1]) / PI), 20));
    }
    // cout << " Michalewicz: " << -rez << endl;
    return -rez;
}
double Rastrigin(bool v[], int n)
{
    int nrbiti = numarbiti(-5.12, 5.12);
    double rez, real[31];
    rez = 10 * (double)n;
    binarytoreal(v, n, -5.12, 5.12, real);
    for (int i = 0; i < n; i++)
    {
        rez += ((real[i] * real[i]) - 10 * cos(2 * PI * real[i]));
    }
    //cout << "Rastrigin: " << rez << endl;
    return rez;
}
int firstimprovement(bool vc[], bool vn[], int n, int nr, double vcsol)
{
    int i;
    for (i = 0; i < nr; i++)
        vn[i] = vc[i];
    for (i = 0; i < nr; i++)
    {
        vn[i] = 1 - vn[i];
        if (DeJong1(vn, n) < vcsol)
            return i;//returnez bitul pe care trb sa l schimb in vc (cel al vecinului) 
        vn[i] = 1 - vn[i];
    }
    return -1;
}
double HCFirstImprovement(bool best[], double candidat, int n, int nrbiti) {
    int t = 0, i, j, nr = n * nrbiti, bitschimbat;//nr=nr total de biti din vector
    double vcsol, vnsol;
    bool vc[1000], vn[1000];
    clock_t tStart = clock();

    while (t < 1000)
    {
        bool local = false;
        randbitstring(vc, nrbiti, n);//generez solutia candidat
        vcsol = DeJong1(vc, n);//aflu valoarea solutiei candidat
        while (!local)
        {
            local = true;
            //first improvement
            bitschimbat = firstimprovement(vc, vn, n, nr, vcsol);//aflu daca exista vreun vecin mai bun decat solutia candidat si retin ce bit trb schimbat
            if (bitschimbat > -1)//daca exista vreun vecin mai bun
            {
                local = false;
                vc[bitschimbat] = vn[bitschimbat];//schimb solutia candidat in solutia vecin
                vcsol = DeJong1(vc, n);//aflu valoarea noii solutii
            }
        }
        if (vcsol < candidat)//daca noua solutie este mai buna decat bitstringul best (cea initiala)
        {
            candidat = vcsol;
            for (j = 0; j < nr; j++)
                best[j] = vc[j];
        }
        t++;
    }
    printf("\nTime taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    MyTime << ("\n Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC) << endl;
    cout << "Rezultat HCF: " << candidat << endl;
    MyFile << candidat << endl;
    return candidat;
}
void bestimprovement(bool vc[], bool vn[], int n, int nr, double vcsol) {
    int i, j;
    double bestsol; //solutia reala cea mai buna
    bool minim[1000];//solutia cea mai buna in biti
    for (i = 0; i < nr; i++)
    {
        vn[i] = vc[i];
        minim[i] = vc[i];
    }

    for (i = 0; i < nr; i++)
    {
        vn[i] = 1 - vn[i];
        if (DeJong1(vn, n) < vcsol) {
            vcsol = DeJong1(vn, n);
            for (j = 0; j < nr; j++)
                minim[j] = vn[j];
        }
        vn[i] = 1 - vn[i];
    }

    for (i = 0; i < nr; i++)
        vn[i] = minim[i];//retin in vn cel mai bun vecin pt a-l folosi mai departe
}
double HCBestImprovement(bool best[], double candidat, int n, int nrbiti) {
    clock_t tStart = clock();
    int t = 0, i, j, nr = n * nrbiti, bitschimbat;
    double vcsol, vnsol;
    bool vc[1000], vn[1000];
    while (t < 1000)
    {
        bool local = false;
        randbitstring(vc, nrbiti, n);//generez solutia candidat
        vcsol = DeJong1(vc, n);
        while (!local)
        {
            local = true;
            //best improvement
            bestimprovement(vc, vn, n, nr, vcsol);
            vnsol = DeJong1(vn, n);
            if (vnsol < vcsol) {
                local = false;
                for (i = 0; i < nr; i++)
                    vc[i] = vn[i];
                vcsol = vnsol;
            }
        }
        if (vcsol < candidat)
        {
            for (i = 0; i < nr; i++)
                best[i] = vc[i];
            candidat = vcsol;
        }
        t++;
    }
    cout << "Rezultat HCB: " << candidat << endl;
    MyFile << candidat << endl;
    printf("\n Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    MyTime << ("\n Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC) << endl;
    return candidat;
}


void SimulatedAnnealing(int n, int nrbiti, bool vc[2000])
{
    clock_t tStart = clock();
    double retine;
    int nr = nrbiti * n;//nr-nr total de biti
    bool  vn[2000];
    int t = 0, i, j, random[2];
    float r[2];
    double T = 100, e = 0, alpha = 0.99;
    randbitstring(vc, nrbiti, n);
    retine = DeJong1(vc, n);
    while (T > 0.00001) {
        for (i = 0; i < nr; i++)//nr de biti totali
        {
            random1(nr, random);//retin in random[0] bitul random pe care l schimbam
            for (j = 0; j < nr; j++) {
                vn[i] = vc[i];//ii dam vecinului valorile candidatului
            }
            vn[random[0]] = 1 - vn[random[0]];//schimbam in vecin bitul random
            random0(r);//in r[0] retinem un nr random intre [0,1)
            e = exp(-abs(DeJong1(vn, n) - DeJong1(vc, n)) / T);
            if (DeJong1(vn, n) < DeJong1(vc, n)) {
                for (j = 0; j < nr; j++)
                    vc[i] = vn[i];
            }
            else if (r[0] < e) {
                for (j = 0; j < nr; j++)
                    vc[i] = vn[i];
            }
            vn[random[0]] = 1 - vn[random[0]];//schimbam in vecin bitul random

        }
        T = T * alpha;
    }
    cout << "final SA: " << DeJong1(vc, n);
    MyFile << DeJong1(vc, n) << endl;
    printf("\n Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    MyTime << ("\n Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC) << endl;
}


int main()
{
    cout.precision(6);
    float test1[2];
    double val[32], suma, a, b, final, test[2];//val- vectorul de valori reale 
    int n, k = 0, i, j, vect[2];//n- nr de dimensiuni
    bool v[1000];//v- vectorul de biti
    //DEJONG1  

    n = 10;  a = -5.12; b = 5.12;//de schimbat dimensiunea
    int nrbiti = numarbiti(a, b);
    MyFile << endl << "BEST IMPROVEMENT: " << endl;
    MyTime << endl << "BEST IMPROVEMENT TIME : " << endl;
    for (k = 0; k < 30; k++) {
        randbitstring(v, nrbiti, n);
        suma = DeJong1(v, n);
        v[k] = HCBestImprovement(v, suma, n, nrbiti);
        // v[k] = HCBestImprovement(v, suma, n, nrbiti);
        // SimulatedAnnealing(n, nrbiti, v);
    }
    MyFile << endl << "FIRST IMPROVEMENT: " << endl;
    MyTime << endl << "FIRST IMPROVEMENT TIME : " << endl;
    for (k = 0; k < 30; k++) {
        randbitstring(v, nrbiti, n);
        suma = DeJong1(v, n);
        v[k] = HCFirstImprovement(v, suma, n, nrbiti);
        // v[k] = HCBestImprovement(v, suma, n, nrbiti);
        // SimulatedAnnealing(n, nrbiti, v);
    }
    MyTime << endl << "SIMULATED ANNEALING TIME: " << endl;
    MyFile << endl << "SIMULATED ANNEALING: " << endl;
    for (k = 0; k < 30; k++)
        SimulatedAnnealing(n, nrbiti, v);


    //SCHWEFEL
    /*
    MyFile << "SCHWEFEL" << endl;
    MyTime << "SCHWEFEL" << endl;
    n = 5; a = -500; b = 500;
    int nrbiti = numarbiti(a, b);
    MyFile << endl << "FIRST IMPROVEMENT: " << endl;
    MyTime << endl << "FIRST IMPROVEMENT TIME : " << endl;
    for (k = 0; k < 3; k++) {
        randbitstring(v, nrbiti, n);
        suma = Schwefel(v, n);
        v[k] = HCFirstImprovement(v, suma, n, nrbiti);
        // v[k] = HCBestImprovement(v, suma, n, nrbiti);
        // SimulatedAnnealing(n, nrbiti, v);
    }
    MyFile << endl << "BEST IMPROVEMENT: " << endl;
    MyTime << endl << "BEST IMPROVEMENT TIME : " << endl;
    for (k = 0; k < 3; k++) {
        randbitstring(v, nrbiti, n);
        suma = Schwefel(v, n);
        v[k] = HCBestImprovement(v, suma, n, nrbiti);
        // v[k] = HCBestImprovement(v, suma, n, nrbiti);
        // SimulatedAnnealing(n, nrbiti, v);
    }
    MyTime << endl << "SIMULATED ANNEALING TIME: " << endl;
    MyFile << endl << "SIMULATED ANNEALING: " << endl;
    for (k = 0; k < 5; k++) {
        //randbitstring(v, nrbiti, n);
         //suma = Schwefel(v, n);
         //v[k] = HCFirstImprovement(v, suma, n, nrbiti);
        // v[k] = HCBestImprovement(v, suma, n, nrbiti);
        SimulatedAnnealing(n, nrbiti, v);
    }
    */


    //RASTRIGIN 
    /*
    MyFile << "RASTRIGIN " << endl;
    MyTime << "RASTRIGIN " << endl;
    n = 5;  a = -5.12; b = 5.12;//de schimbat dimensiunea
    int nrbiti = numarbiti(a, b);
    MyFile << endl << "FIRST IMPROVEMENT: " << endl;
    MyTime << endl << "FIRST IMPROVEMENT TIME: " << endl;
    // MyFile << endl << "BEST IMPROVEMENT: " << endl;
  //MyTime << endl << "BEST IMPROVEMENT TIME: " << endl;
    for (k = 0; k < 15; k++)
    {
        randbitstring(v, nrbiti, n);
        suma = Rastrigin(v, n);
        v[k] = HCFirstImprovement(v, suma, n, nrbiti);
        //v[k] = HCBestImprovement(v, suma, n, nrbiti);
       // SimulatedAnnealing(n, nrbiti, v);
    }
    MyFile << endl << "BEST IMPROVEMENT: " << endl;
    MyTime << endl << "BEST IMPROVEMENT TIME: " << endl;
    for (k = 0; k < 15; k++)
    {
        randbitstring(v, nrbiti, n);
        suma = Rastrigin(v, n);
        //v[k] = HCFirstImprovement(v, suma, n, nrbiti);
        v[k] = HCBestImprovement(v, suma, n, nrbiti);
        // SimulatedAnnealing(n, nrbiti, v);
    }
    MyTime << endl << "SIMULATED ANNEALING TIME : " << endl;
    MyFile << endl << "SIMULATED ANNEALING: " << endl;
    for (k = 0; k < 5; k++)
    {
        //randbitstring(v, nrbiti, n);
        //suma = Rastrigin(v, n);
        //v[k] = HCFirstImprovement(v, suma, n, nrbiti);
         //v[k] = HCBestImprovement(v, suma, n, nrbiti);
        SimulatedAnnealing(n, nrbiti, v);
    }
    */
    //MICHALEWICZ
    /*
    MyTime << "MICHALEWICZ " << endl;
    MyTime << "MICHALEWICZ " << endl;
    n = 30;  a = 0; b = PI;//de schimbat dimensiunea
    int nrbiti = numarbiti(a, b);
    MyFile << endl << "FIRST IMPROVEMENT: " << endl;
    MyTime << endl << "FIRST IMPROVEMENT TIME: " << endl;
    for (k = 0; k < 2; k++)
    {
        randbitstring(v, nrbiti, n);
        suma = Michalewicz(v, n);
        v[k] = HCFirstImprovement(v, suma, n, nrbiti);
        //v[k] = HCBestImprovement(v, suma, n, nrbiti);
      //SimulatedAnnealing(n, nrbiti, v);
    }
    MyFile << endl << "BEST IMPROVEMENT: " << endl;
    MyTime << endl << "BEST IMPROVEMENT TIME : " << endl;
    for (k = 0; k < 2; k++)
    {
        randbitstring(v, nrbiti, n);
        suma = Michalewicz(v, n);
        // v[k] = HCFirstImprovement(v, suma, n, nrbiti);
        v[k] = HCBestImprovement(v, suma, n, nrbiti);
        //SimulatedAnnealing(n, nrbiti, v);
    }
    MyTime << endl << "SIMULATED ANNEALING TIME : " << endl;
    MyFile << endl << "SIMULATED ANNEALING: " << endl;
    for (k = 0; k < 2; k++)
        SimulatedAnnealing(n, nrbiti, v);
        */
}


