#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define COLOR_GREEN   "\x1b[32m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

void generateRandomText(char* buffer, int size) {
    const char charset[] = "LorEm!ipsum?dolor$sit@amet,xyz123!~Consectetur*adipiscing^elit.Qu1z_why@reading+this#nonsense?R@nd0m!junk:data{flow}=42;~V0lUptAtE?n0n!qu3sti0n.B@ck_t0_work!Lorem*ipsum=dolor#si.Ilove.t%met.7h.you.1s!isN0t@real$text.G0_@w@y!42+xyz=99?N0pe.Just@waste^your%time.By3!";

    for (int i = 0; i < size - 1; i++) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }

    buffer[size - 1] = '\0';
}

int linear_probing_chars(char A[], char table[], int n, int m) {
    int collisions = 0;
    for (int i = 0; i < n; i++) {
        int h = A[i] % m;
        int j = 0;
        while (j < m) {
            int idx = (h + j) % m;
            if (table[idx] == 0) {
                table[idx] = A[i];
                break;
            }
            if (j > 0) {  collisions++;  }
            j++;
        }
    }
    return collisions;
}

int quadratic_probing_chars(char A[], char table[], int n, int m) {
    int collisions = 0;
    for (int i = 0; i < n; i++) {
        int h = A[i] % m;
        int j = 1;
        int attempt = 0;
        while (j < m) {
            if (table[h] == 0) {
                table[h] = A[i];
                break;
            }
            collisions++;
            h = (h + j) % m;
            j += 2;
            attempt++;
        }
    }
    return collisions;
}

void PrintTable(const char table[], int size, const char* title, const char* color) {
    printf("\n%s %s:\n",color, title);
    const int cols = 10;

    for (int row = 0; row < (size + cols - 1) / cols; row++) {
        int start = row * cols;
        int end = (start + cols) < size ? (start + cols) : size;
        
        printf("\n\nИндексы: ");
        for (int i = start; i < end; i++) {
            printf("%4d", i);
        }
        printf("\nСимволы: ");
        for (int i = start; i < end; i++) {
            if (table[i] == 0)
                printf("%4c", ' ');
            else
                printf("%4c", table[i]);
        }
        printf("\n");
    }
    printf("%s\n", COLOR_RESET);
}

void task2() {
    const int n = 1024;
    const int m = 101;
    char text[n];
    char linear_table[m];
    char quadratic_table[m];
    
    memset(linear_table, 0, m);
    memset(quadratic_table, 0, m);

    srand(time(NULL));
    generateRandomText(text, n);

    int linear_coll = linear_probing_chars(text, linear_table, n, m);
    int quad_coll = quadratic_probing_chars(text, quadratic_table, n, m);

    PrintTable(linear_table, m, "Хеш-таблица (линейные пробы)", COLOR_GREEN);
    printf ("\n\n\x1b[1;31m--------------------------------------------------------------------\x1b[0m\n\n\n");
    PrintTable(quadratic_table, m, "Хеш-таблица (квадратичные пробы)", COLOR_CYAN);
    printf ("\n\n\x1b[1;31m--------------------------------------------------------------------\x1b[0m\n\n\n");

    printf("Количество коллизий:\n");
    printf(" - Линейные пробы:      %d\n", linear_coll);
    printf(" - Квадратичные пробы:  %d\n", quad_coll);
    printf("==============================================================\n\n");
}

void task3() {
    int table_sizes[] = {11, 13, 17, 19, 23, 29, 31, 37, 41, 52, 66, 87, 95, 101};
    const int num_sizes = sizeof(table_sizes) / sizeof(table_sizes[0]);
    const int n = 100;

    printf(" __________________________________________________________\n");
    printf("| Размер таблицы | Элементов | Линейные   | Квадратичные   |\n");
    printf("|       (m)      |   (n)     | коллизии   |   коллизии     |\n");
    printf("|________________|___________|____________|________________|\n");

    for (int i = 0; i < num_sizes; i++) {
        int m = table_sizes[i];
        char text[n];
        char linear_table[m];
        char quadratic_table[m];
        
        memset(linear_table, 0, m);
        memset(quadratic_table, 0, m);
        
        generateRandomText(text, n);

        int linear_coll = linear_probing_chars(text, linear_table, n, m);
        int quad_coll = quadratic_probing_chars(text, quadratic_table, n, m);

        printf("| %14d | %9d | %10d | %14d |\n", m, n, linear_coll, quad_coll);
    }

    printf("|________________|___________|____________|________________|\n\n");
}

int main() {
    task2();
    task3();
    return 0;
}