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
        A[i] = i;
    }
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

void PrintPhoneBook(int n, PhoneBook phoneBook[], int indexArr[]) {
    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("|        Имя        |        Фамилия        |        Номер        |                Адрес                |\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("| %-17s | %-21s | %-19s | %-35s |\n", 
            phoneBook[indexArr[i]].firstname, 
            phoneBook[indexArr[i]].lastname, 
            phoneBook[indexArr[i]].phone, 
            phoneBook[indexArr[i]].address
        );
    }

    printf("---------------------------------------------------------------------------------------------------------\n");
}

int selectSort(int n, PhoneBook arr[], int ind[], int (*predicate)(PhoneBook *a, PhoneBook *b)) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 0; i < n - 1; i++) {
        int minInd = i;
        for (int j = i + 1; j < n; j++) {
            C++;
            if (predicate(&arr[ind[j]], &arr[ind[minInd]]) < 0) {
                minInd = j;
            }
        }
        
        int temp = ind[i];
        ind[i] = ind[minInd];
        ind[minInd] = temp;
        M += 3;
    }

    return (M + C);
}

/*
Compares two PhoneBooks by last and first name
@return 0 if a and b are equal, >0 if a is greater than b, <0 if a is less than b
*/
int compareByLastAndFirstName(PhoneBook *a, PhoneBook *b) {
    int lastnameComparison = strcmp(a->lastname, b->lastname);

    if (lastnameComparison != 0) {
        return lastnameComparison;
    } else {
        int firstnameComparison = strcmp(a->firstname, b->firstname);
        return firstnameComparison;
    }
}

int compareByLastAndFirstNameDescending(PhoneBook *a, PhoneBook *b) {
    int lastnameComparison = strcmp(b->lastname, a->lastname);

    if (lastnameComparison != 0) {
        return lastnameComparison;
    } else {
        int firstnameComparison = strcmp(b->firstname, a->firstname);
        return firstnameComparison;
    }
}

int compareByFirstAndLastName(PhoneBook *a, PhoneBook *b) {
    int firstnameComparison = strcmp(a->firstname, b->firstname);
    
    if (firstnameComparison != 0) {
        return firstnameComparison;
    } else {
        int lastnameComparison = strcmp(a->lastname, b->lastname);
        return lastnameComparison;
    }
}

int compareByFirstAndLastNameDescending(PhoneBook *a, PhoneBook *b) {
    int firstnameComparison = strcmp(b->firstname, a->firstname);
    
    if (firstnameComparison != 0) {
        return firstnameComparison;
    } else {
        int lastnameComparison = strcmp(b->lastname, a->lastname);
        return lastnameComparison;
    }
}

int binarySearchImproved(int n, PhoneBook arr[], int indexArr[], PhoneBook *x, int (*predicate)(PhoneBook *a, PhoneBook *b)) {
    int L = 0, R = n - 1;
    int M;

    while (L < R) {
        M = (L + R) / 2;
        
        if (predicate(&arr[indexArr[M]], x) < 0) {
            L = M + 1;
        } else {
            R = M;
        }
    }
    
    M = (L + R) / 2;
    if (predicate(&arr[indexArr[M]], x) == 0) {
        return M;
    } else {
        return -1;
    }
}

void main() {
    
    PhoneBook phoneBook[MAX_RECORDS] = {
        {"Marvin", "Rollins", "1-520-299-3398", "16458 Ferry Road Suite 272"},
        {"Alexia", "Tucker", "1-611-725-3659", "736 Christiansen Highway Apt. 199"},
        {"David", "Wallace", "1-220-203-1354", "720 Blanca Forges Suite 248"},
        {"Charlie", "Briggs", "1-704-751-2986", "951 Gilbert Forks Apt. 154"},
        {"Alice", "Murphy", "1-397-364-2439", "2662 Nicholaus Unions Apt. 615"},
    };

    int a[MAX_RECORDS], b[MAX_RECORDS];
    FillInc(MAX_RECORDS, a);
    FillInc(MAX_RECORDS, b);

    printf("Оригинальная книга: \n\n");
    PrintPhoneBook(MAX_RECORDS, phoneBook, a);

    selectSort(MAX_RECORDS, phoneBook, a, compareByLastAndFirstName);
    selectSort(MAX_RECORDS, phoneBook, b, compareByFirstAndLastName);
    printf("\nОтсортированная по фамилии и имени книга: \n\n");
    PrintPhoneBook(MAX_RECORDS, phoneBook, a);
    printf("\nОтсортированная по имени и фамилии книга: \n\n");
    PrintPhoneBook(MAX_RECORDS, phoneBook, b);
    
    PhoneBook *x = &phoneBook[0];
    int index;

    printf("\nБинарный поиск по фамилии %s\n", x->lastname);
    index = binarySearchImproved(MAX_RECORDS, phoneBook, a, x, compareByLastAndFirstName);
    printf("Найден %s по индексу ind[%d] = %d\n\n", phoneBook[a[index]].lastname, index, a[index]);
    
    printf("\nБинарный поиск по имени %s\n", x->firstname);
    index = binarySearchImproved(MAX_RECORDS, phoneBook, b, x, compareByFirstAndLastName);
    printf("Найден %s по индексу ind[%d] = %d\n", phoneBook[b[index]].firstname, index, b[index]);
}