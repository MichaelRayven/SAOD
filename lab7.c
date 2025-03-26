#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#define MAX_RECORDS 5

typedef struct {
    char firstname[32];
    char lastname[32];
    char phone[16];
    char address[128];
} PhoneBook;


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

void PrintPhoneBook(int n, PhoneBook *phoneBook) {
    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("|        Имя        |        Фамилия        |        Номер        |                Адрес                |\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("| %-17s | %-21s | %-19s | %-35s |\n", 
            phoneBook[i].firstname, 
            phoneBook[i].lastname, 
            phoneBook[i].phone, 
            phoneBook[i].address
        );
    }

    printf("---------------------------------------------------------------------------------------------------------\n");
}

int selectSort(int n, PhoneBook arr[], int (*predicate)(PhoneBook *a, PhoneBook *b)) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 0; i < n - 1; i++) {
        int minInd = i;
        for (int j = i + 1; j < n; j++) {
            C++;
            if (predicate(&arr[j], &arr[minInd]) > 0) {
                minInd = j;
            }
        }
        
        PhoneBook temp = arr[i];
        arr[i] = arr[minInd];
        arr[minInd] = temp;
        M += 3;
    }

    return (M + C);
}

int compareByLastAndFirstName(PhoneBook *a, PhoneBook *b) {
    int lastnameComparison = strcmp(a->lastname, b->lastname);

    if (lastnameComparison > 0) {
        return -1;
    } else if (lastnameComparison < 0) {
        return 1;
    } else {
        int firstnameComparison = strcmp(a->firstname, b->firstname);
        if (firstnameComparison > 0) {
            return -1;
        } else if (firstnameComparison < 0) {
            return 1;
        } else {
            return 0;
        }
    }
}

int compareByLastAndFirstNameDescending(PhoneBook *a, PhoneBook *b) {
    int lastnameComparison = strcmp(a->lastname, b->lastname);

    if (lastnameComparison > 0) {
        return 1;
    } else if (lastnameComparison < 0) {
        return -1;
    } else {
        int firstnameComparison = strcmp(a->firstname, b->firstname);
        if (firstnameComparison > 0) {
            return 1;
        } else if (firstnameComparison < 0) {
            return -1;
        } else {
            return 0;
        }
    }
}

int compareByFirstAndLastName(PhoneBook *a, PhoneBook *b) {
    int firstnameComparison = strcmp(a->firstname, b->firstname);
    
    if (firstnameComparison > 0) {
        return -1;
    } else if (firstnameComparison < 0) {
        return 1;
    } else {
        int lastnameComparison = strcmp(a->lastname, b->lastname);

        if (lastnameComparison > 0) {
            return -1;
        } else if (lastnameComparison < 0) {
            return 1;
        } else {
            return 0;
        }
    }
}

int compareByFirstAndLastNameDescending(PhoneBook *a, PhoneBook *b) {
    int firstnameComparison = strcmp(a->firstname, b->firstname);
    
    if (firstnameComparison > 0) {
        return 1;
    } else if (firstnameComparison < 0) {
        return -1;
    } else {
        int lastnameComparison = strcmp(a->lastname, b->lastname);

        if (lastnameComparison > 0) {
            return 1;
        } else if (lastnameComparison < 0) {
            return -1;
        } else {
            return 0;
        }
    }
}

int binarySearchImproved(int n, PhoneBook arr[], char lastname[32], int *ind) {
    int L = 0, R = n - 1;
    int C = 0, M;
    

    while (L < R) {
        M = (L + R) / 2;
        
        C++;
        if (strcmp(arr[M].lastname, lastname) < 0) {
            L = M + 1;
        } else {
            R = M;
        }
    }
    
    C++;
    M = (L + R) / 2;
    if (strcmp(arr[M].lastname, lastname) == 0) {
        (*ind) = M;
    } else {
        (*ind) = -1;
    }

    return C;
}

void main() {
    
    PhoneBook phoneBook[MAX_RECORDS] = {
        {"Marvin", "Rollins", "1-520-299-3398", "16458 Ferry Road Suite 272"},
        {"Alexia", "Tucker", "1-611-725-3659", "736 Christiansen Highway Apt. 199"},
        {"David", "Wallace", "1-220-203-1354", "720 Blanca Forges Suite 248"},
        {"Charlie", "Briggs", "1-704-751-2986", "951 Gilbert Forks Apt. 154"},
        {"Alice", "Murphy", "1-397-364-2439", "2662 Nicholaus Unions Apt. 615"},
    };

    printf("Оригинальная книга: \n\n");
    PrintPhoneBook(MAX_RECORDS, phoneBook);
    selectSort(MAX_RECORDS, phoneBook, compareByLastAndFirstNameDescending);
    printf("\nОтсортированная книга: \n\n");
    PrintPhoneBook(MAX_RECORDS, phoneBook);
    int ind = 0;
    char lastname[32] = "Wallace";
    binarySearchImproved(MAX_RECORDS, phoneBook, lastname, &ind);
    printf("Searching for %s in the phone book\n", lastname);
    printf("Found %s at %d\n", lastname, ind);
}