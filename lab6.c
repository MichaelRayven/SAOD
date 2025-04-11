#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <SDL3/SDL.h>
#include <graph.h>
#include <math.h>

int readInt(int *number) {
    char line[64];
    fgets(line, sizeof(line), stdin);

    if (sscanf(line, "%d", number) != 1) {
        return 0;
    } else {
        return 1;
    }
}

void FillInc(int n, int A[]) {
    for (int i = 0; i < n; i++) {
        A[i] = i + 1;
    }
}

void FillDec(int n, int A[]) {
    for (int i = 0; i < n; i++) {
        A[i] = n - i;
    }
}

void FillRand(int n, int A[]) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 100;
    }
}

int CheckSum(int n, int A[]) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += A[i];
    }
    return sum;
}

int RunNumber(int n, int A[]) {
    if (n == 0) return 0;
    int series = 1;
    for (int i = 1; i < n; i++) {
        if (A[i] < A[i - 1]) {
            series++;
        }
    }
    return series;
}

void PrintMas(int n, int A[]) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

void sPrintMas(char* buffer, int n, int A[]) {
    char num[16];
    for (int i = 0; i < n; i++) {
        sprintf(num, "%d ", A[i]);
        strcat(buffer, num);
    }
    strcat(buffer, "\n");
}

/*
Copies all elements from source array into the target array.
@param n length of array A and B.
@param A source array.
@param B target array.
*/
void CopyMas(int n, int A[], int B[]) {
    for (int i = 0; i < n; i++) {
        B[i] = A[i];
    }
}

void PrintTimeTable(int (*sortFunction)(int n, int arr[])) {
    srand(time(NULL));

    printf("|  N  |  M+C theor.  |       Mfact + Cfact       |\n");
    printf("|     |              |  Dec.  |  Rand.  |  Inc.  |\n");

    for (int n = 100; n <= 500; n += 100) {
        int *A = (int *) malloc(sizeof(int) * n);

        if (A == NULL) {
            perror("Memory allocation failed!");
            return;
        }

        int C = n * (n - 1) / 2;
        int M = C + n*2 - 2;
        int Ttheor = M + C;

        FillDec(n, A);
        int Tdec = sortFunction(n, A);

        FillRand(n, A);
        int Trand = sortFunction(n, A);

        FillInc(n, A);
        int Tinc = sortFunction(n, A);

        printf("| %3d |  %10d  | %6d | %7d | %6d |\n", n, Ttheor, Tdec, Trand, Tinc);

        free(A);
    }
}

void PrintComparisonTable(int fcnt, int (**sortFunctions)(int n, int arr[]), char **labels) {
    srand(time(NULL));

    char header[256] = "|  N  |";
    char subHeader[256] = "|     |";
    int spaceCnt = 4*2*fcnt + fcnt - strlen("Mfact + Cfact");
    int i, length;

    for (i = 0; i < fcnt; i++) 
        spaceCnt += strlen(labels[i]);

    spaceCnt /= 2;
    
    length = strlen(header);
    for (i = length; i < (length + spaceCnt); i++) 
        header[i] = ' ';
    
    strcat(header, "Mfact + Cfact");

    length = strlen(header);
    for (i = length; i < (length + spaceCnt); i++) 
        header[i] = ' ';

    header[i] = '|';
    header[i + 1] = 0;

    printf("%s\n", header);


    for (i = 0; i < fcnt; i++) {
        length = strlen(subHeader);
        for (int j = length; j < (length + 4); j++) {
            subHeader[j] = ' ';
        }
        subHeader[strlen(subHeader)] = 0;
        strcat(subHeader, labels[i]);

        length = strlen(subHeader);
        for (int j = length; j < (length + 4); j++) {
            subHeader[j] = ' ';
        }

        length = strlen(subHeader);
        subHeader[length] = '|';
        subHeader[length + 1] = 0;
    }

    printf("%s\n", subHeader);


    for (int n = 100; n <= 500; n += 100) {
        int *A = (int *) malloc(sizeof(int) * n);
        int *B = (int *) malloc(sizeof(int) * n);
        FillRand(n, A);

        if (A == NULL || B == NULL) {
            perror("Memory allocation failed!");
            return;
        }

        printf("| %3d |", n);
        for (i = 0; i < fcnt; i++) {
            CopyMas(n, A, B);
            int Trand = sortFunctions[i](n, B);
            char line[64] = "    %", number[16];
            sprintf(number, "%ld", strlen(labels[i]));
            strcat(line, number);
            strcat(line, "d    |");
            printf(line, Trand);
        }
        printf("\n");

        // Clear memory
        free(A);
        free(B);
    }
}

void RunTests(int (*sortFunction)(int n, int arr[])) {
    int arr[10];
    int n = 10;

    // Incremental array checks
    FillInc(n, arr);
    printf("\nIncremental array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));
    sortFunction(n, arr);
    printf("\nSorted incremental array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));

    // Decremental array checks
    FillDec(n, arr);
    printf("\nDecremental array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));
    sortFunction(n, arr);
    printf("\nSorted decremental array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));


    // Random array checks
    FillRand(n, arr);
    printf("\nRandom array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));
    sortFunction(n, arr);
    printf("\nSorted random array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n\n", RunNumber(n, arr), CheckSum(n, arr));
}

// @param fcnt the number of functions to graph
void MakeSearchGraph(int fcnt, int (**sortFunctions)(int n, int arr[], int x, int* ind), char **functionNames, int iterations) {
    int **timeArrays = (int **) malloc(sizeof(int *) * fcnt);
    int *numberArray = (int *) malloc(sizeof(int) * iterations);
    
    if (timeArrays == NULL || numberArray == NULL) {
        perror("Memory allocation failed!");
        return;
    }
    
    // Get test data by running all the sort functions and measuring time
    int max = INT_MIN, min = INT_MAX;
    for (int i = 0; i < fcnt; i++) {
        timeArrays[i] = (int *) malloc(sizeof(int) * iterations);
        
        if (timeArrays[i] == NULL) {
            perror("Memory allocation failed!");
            return;
        }
        
        int n = 2;
        int ind = -1;
        // Fill time array for i-th sort function
        for (int j = 0; j < iterations; j++) {
            int *arr = (int *) malloc(sizeof(int) * n);
            FillInc(n, arr);
            
            
            timeArrays[i][j] = (sortFunctions[i])(n, arr, arr[0], &ind);
            numberArray[j] = (int) (SDL_log(n)/SDL_log(2));

            if (arr[ind] != arr[0]) {
                perror("Search function error: Found value doesn't match the key.");
                return;
            }
            
            if (timeArrays[i][j] > max) {
                max = timeArrays[i][j];
            }
            if (timeArrays[i][j] < min) {
                min = timeArrays[i][j];
            }
            
            free(arr);
            n *= 2;
        }
    }
    
    // Generate graph y-labels
    int lstep = max / 8 + 1;
    int lcnt = 1;
    while (lcnt * lstep < max) {
        lcnt++;
    }

    int *labelsArray = (int *) malloc(sizeof(int) * lcnt);
    for (int i = 0; i < lcnt; i++) {
        labelsArray[i] = lstep * i;
    }


    GraphInit();
    bool quit = { false };

    //The event data
    SDL_Event e;
    while( quit == false ){
                //Get event data
                while( SDL_PollEvent( &e ) )
                {
                    //If event is quit type
                    if( e.type == SDL_EVENT_QUIT )
                    {
                        //End the main loop
                        GraphQuit();
                        quit = true;
                    }
                }

                /* Draw the text */
                GraphIterate(fcnt, iterations, lcnt, timeArrays, functionNames, numberArray, labelsArray);
    }

    // Free memory
    for (int i = 0; i < fcnt; i++) {
        free(timeArrays[i]);
    }
    free(timeArrays);
    free(numberArray);
    free(labelsArray);
}

int shakerSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения
    int L = 0, R = n - 1, k = n - 1;

    do {
        for (int i = R; i > L; i--) {
            C++;
            if (arr[i] < arr[i - 1]) {
                int temp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = temp;
                M += 3;
                k = i;
            }
        }
        L = k;

        for (int i = L; i < R; i++) {
            C++;
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                M += 3;
                k = i;
            }
        }
        R = k;

    } while(L < R);

    // printf("\n* Comparisons: %d, moves: %d *\n", C, M);

    return (M + C);
}

int bubbleSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 0; i < n - 1; i++) {
        for (int j = n - 1; j > i; j--) {
            C++;
            if (arr[j] < arr[j - 1]) { 
                int temp = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = temp;
                M += 3;
            }
        }
    }

    // printf("\n* Comparisons: %d, moves: %d *\n", C, M);

    return (M + C);
}

int selectSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 0; i < n - 1; i++) {
        int minInd = i;
        for (int j = i + 1; j < n; j++) {
            C++;
            if (arr[j] < arr[minInd]) {
                minInd = j;
            }
        }
        
        int temp = arr[i];
        arr[i] = arr[minInd];
        arr[minInd] = temp;
        M += 3;
    }

    return (M + C);
}

int insertSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 1; i < n; i++) {
        int temp = arr[i];
        int j = i - 1;
        M++;
        
        C++;
        for (; j >= 0 && temp < arr[j]; j--) {
            arr[j + 1] = arr[j];
            C++;
            M++;
        }
        
        arr[j + 1] = temp;
        M++;
    }

    // printf("\n* Comparisons: %d, moves: %d *\n", C, M);
    return M + C;
}

int* generateSteps(int n) {
    int m = ((int) log2(n))- 1;
    int h = 1, i = 0;
    int *arr = malloc(sizeof(int) * m);

    while (i < m) {
        arr[i] = h;
        h = 2*h + 1;
        i++;         
    }

    return arr;
}

int shellSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения
    int m = ((int) log2(n)) - 1; // Количество k-сортировок
    int *H = generateSteps(n);

    while (m > 0) {
        int k = H[--m]; // Шаг
        
        for (int i = k; i < n; i++) {
            int temp = arr[i];
            int j = i - k;
            M++;
            
            C++;
            for (; j >= 0 && temp < arr[j]; j -= k) {
                arr[j + k] = arr[j];
                C++;
                M++;
            }
            
            arr[j + k] = temp;
            M++;
        }
    }

    // printf("\n* Comparisons: %d, moves: %d *\n", C, M);
    free(H);
    return M + C;
}

int* myGenerateSteps(int n, int *m) {
    int h = 1, i = 0;
    int size = n/2;
    int *arr = malloc(sizeof(int) * size);
    double gamma = 2.243609061420001;
    
    
    while (h < n) {
        if (i >= size) {
            size += 10;
            arr = realloc(arr, sizeof(int) * size);
        }

        arr[i++] = h;
        h = (int) ((pow(gamma, (i + 1)) - 1)/(gamma - 1)) + 1;
    }
    
    *m = i;

    return arr;
}

int myShellSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения
    int m = 0; // Количество k-сортировок
    int *H = myGenerateSteps(n, &m);

    while (m > 0) {
        int k = H[--m]; // Шаг
        
        for (int i = k; i < n; i++) {
            int temp = arr[i];
            int j = i - k;
            M++;
            
            C++;
            for (; j >= 0 && temp < arr[j]; j -= k) {
                arr[j + k] = arr[j];
                C++;
                M++;
            }
            
            arr[j + k] = temp;
            M++;
        }
    }

    // printf("\n* Comparisons: %d, moves: %d *\n", C, M);
    free(H);
    return M + C;
}

int binarySearch(int n, int arr[], int x, int *ind) {
    int L = 0, R = n - 1;
    int C = 0;
    
    while (R >= L) {
        int M = (L + R) / 2;
        
        C++;
        if (arr[M] == x) {
            (*ind) = M;
            return C;
        } 
        
        C++;
        if (arr[M] > x) {
            R = M - 1;
        } else {
            L = M + 1;
        }
    }

    (*ind) = -1;
    return C;
}

int binarySearchImproved(int n, int arr[], int x, int *ind) {
    int L = 0, R = n - 1;
    int C = 0, M;
    

    while (L < R) {
        M = (L + R) / 2;
        
        C++;
        if (arr[M] < x) {
            L = M + 1;
        } else {
            R = M;
        }
    }
    
    C++;
    M = (L + R) / 2;
    if (arr[M] == x) {
        (*ind) = M;
    } else {
        (*ind) = -1;
    }

    return C;
}

int binarySearchAll(int n, int arr[], int x, int *start, int *stop) {
    int L = 0, R = n - 1;
    int C = 0;
    
    while (R >= L) {
        int M = (L + R) / 2;
        
        C++;
        if (arr[M] == x) {
            (*start) = M;
        } 
        
        C++;
        if (arr[M] < x) {
            L = M + 1;
        } else {
            R = M - 1;
        }
    }
    
    L = 0, R = n - 1;
    while (R >= L) {
        int M = (L + R) / 2;
        
        C++;
        if (arr[M] == x) {
            (*stop) = M;
        } 
        
        C++;
        if (arr[M] <= x) {
            L = M + 1;
        } else {
            R = M - 1;
        }
    }

    return C;
}

int binarySearchAllImproved(int n, int arr[], int x, int *start, int *stop) {
    int L = 0, R = n - 1;
    int C = 0, M;
    

    while (L < R) {
        M = (L + R) / 2;
        
        C++;
        if (arr[M] < x) {
            L = M + 1;
        } else {
            R = M;
        }
    }
    
    C++;
    M = (L + R) / 2;
    if (arr[M] == x) {
        (*start) = M;
    } else {
        (*start) = -1;
    }


    L = 0, R = n - 1;
    while (L <= R) {
        M = (L + R) / 2;
        
        C++;
        if (arr[M] <= x) {
            L = M + 1;
        } else {
            R = M - 1;
        }
    }
    
    C++;
    M = (L + R) / 2;
    if (arr[M] == x) {
        (*stop) = M;
    } else {
        (*stop) = -1;
    }

    return C;
}

void PrintSearchComparisonTable() {
    printf("|   N   |  Cfact version 1  |  Cfact version 2  |\n");
    for (int n = 100; n <= 1000; n += 100) {
        int *arr = malloc(sizeof(int) * n);
        FillInc(n, arr);
        int ind = 0;
        int C = binarySearch(n, arr, arr[n - 1], &ind);
        int Cimproved = binarySearchImproved(n, arr, arr[n - 1], &ind);
        printf("| %5d | %17d | %17d |\n", n, C, Cimproved);
        free(arr);
    }
    printf("\n");
}

void PrintSearchAllComparisonTable() {
    printf("|   N   |  Cfact All version 1  |  Cfact All version 2  |\n");
    for (int n = 100; n <= 1000; n += 100) {
        int start = -1, stop = -1;
        int *arr = malloc(sizeof(int) * n);
        FillInc(n, arr);
        int C = binarySearchAll(n, arr, arr[n - 1], &start, &stop);
        int Cimproved = binarySearchAllImproved(n, arr, arr[n - 1], &start, &stop);
        printf("| %5d | %21d | %21d |\n", n, C, Cimproved);
        free(arr);
    }
    printf("\n");
}

int main() {  
    srand(time(NULL));

    int n = 100000, ind = -1, start = -1, stop = -1, C = -1;
    int *arr = malloc(sizeof(int) * n);
    FillInc(n, arr);
    // PrintMas(n, arr);

    printf("Searching for first element - %d:\n", arr[0]);
    C = binarySearch(n, arr, arr[0], &ind);
    printf("Found element %d at %d in %d comparisons\n\n", arr[ind], ind, C);
    
    printf("Searching for last element - %d:\n", arr[n - 1]);
    C = binarySearch(n, arr, arr[n - 1], &ind);
    printf("Found element %d at %d in %d comparisons\n\n", arr[ind], ind, C);

    printf("Searching for non-existant element %d:\n", -999);
    C = binarySearch(n, arr, -999, &ind);
    printf("Found element %d at %d in %d comparisons\n\n", arr[ind], ind, C);


    printf("\nImproved binary search:\n");
    printf("Searching for first element - %d:\n", arr[0]);
    C = binarySearchImproved(n, arr, arr[0], &ind);
    printf("Found element %d at %d in %d comparisons\n\n", arr[ind], ind, C);
    
    printf("Searching for last element - %d:\n", arr[n - 1]);
    C = binarySearchImproved(n, arr, arr[n - 1], &ind);
    printf("Found element %d at %d in %d comparisons\n\n", arr[ind], ind, C);

    printf("Searching for non-existant element %d:\n", -999);
    C = binarySearchImproved(n, arr, -999, &ind);
    printf("Found element %d at %d in %d comparisons\n\n", arr[ind], ind, C);

    PrintSearchComparisonTable();
    PrintSearchAllComparisonTable();

    FillRand(n, arr);
    shellSort(n, arr);
    printf("Binary search all test:\n");

    C = binarySearchAll(n, arr, 1, &start, &stop);
    printf("Found element %d at %d-%d in %d comparisons\n", arr[start], start, stop, C);
    printf("arr[%d] = %d, arr[%d] = %d, arr[%d] = %d, arr[%d] = %d\n", start - 1, arr[start - 1], start, arr[start], stop, arr[stop], stop + 1, arr[stop + 1]);
    
    printf("\nBinary search all improved test:\n");
    C = binarySearchAllImproved(n, arr, 1, &start, &stop);
    printf("Found element %d at %d-%d in %d comparisons\n", arr[start], start, stop, C);
    printf("arr[%d] = %d, arr[%d] = %d, arr[%d] = %d, arr[%d] = %d\n", start - 1, arr[start - 1], start, arr[start], stop, arr[stop], stop + 1, arr[stop + 1]);

    int (*searchFunctions[])(int, int*, int, int*) = {binarySearch, binarySearchImproved};
    char* functionNames[] = {"BinarySearch v1", "BinarySearch v2"};
    int fcnt = (int) sizeof(searchFunctions) / sizeof(searchFunctions[0]);
    MakeSearchGraph(fcnt, searchFunctions, functionNames, 22);

    free(arr);

    return 0;
}