//
// Created by varsem on 20.11.23.
//
#include "functions.h"

int main(int ac, char *av[])
{
    try
    {
        int p, n1, n2, i;
        char *filename;

        //Проверка параметров командной строки
        if(ac != 5)
        {
            cout << "Wrong parameters" << endl;
            return -1;
        }
        if(toInt(av[1], &p) == -1)
        {
            cout << "Can't read parameter p." << endl;
            return -1;
        }
        if(toInt(av[2], &n1) == -1)
        {
            cout << "Can't read parameter n1." << endl;
            return -1;
        }
        if(toInt(av[3], &n2) == -1)
        {
            cout << "Can't read parameter n2." << endl;
            return -1;
        }
        filename = av[4];
        if(n1 < 0 or p <= 0 or n2 < 0)
        {
            cout << "Wrong parameters n or p" << endl;
            return -1;
        }

        //Считываем массив а из файла
        int n = n1 * n2;
        double *a = new double[n];
        if(read_a(filename, a, n) == -1)
            return -1;

       cout << "          ";
       for(i = 0; i < n; i++)
           printf("%10.3e ", a[i]);
       cout << endl;

        //Заполним аргументы
        ARGS *args = new ARGS[p];
        for(i = 0; i < p; i++)
        {
            args[i].p = p;
            args[i].m = i;
            args[i].a = a;
            args[i].n1 = n1;
            args[i].n2 = n2;
            args[i].a_help = new double[n];
        }

        // Создаем потоки
        pthread_t *threads = new pthread_t[p];
        for(i = 0; i < p; i++)
        {
            if(pthread_create(threads + i, 0, process_main, args + i))
            {
                cout << "Cannot create thread " << i << endl;
                delete[] threads;
                delete[] args;
                return -4;
            }
        }

        //Ждем потоки
        for(int i = 0; i < p; i++)
        {
            if(pthread_join(threads[i], 0))
                cout << "Cannot wait thread " << i << endl;
        }

        cout << "RESULT " << p << ": ";
        for(i = 0; i < n; i++)
            printf("%10.3e ", a[i]);
        cout << endl;

        cout << "CPU TIME" << endl;
        for(i = 0; i < p; i++)
            printf("     %d     | ", i);
        cout << endl;
        for(i = 0; i < p; i++)
            printf("%10.3e | ", args[i].cpu_time);
        cout << endl;

        cout << "FULL TIME: ";
        printf("%10.3e", args[0].full_time);
        cout << endl;


        delete[] threads;
        delete[] args;
        delete[] a;

    } catch (const bad_alloc& e)
    {
        cout << "Bad alloc" << endl;
        return -1;
    }

    return 0;
}