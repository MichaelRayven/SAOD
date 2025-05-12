#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Node {
    struct Node* next;
    int key;
} Node;

int hash(int size, int key) {
    return key % size;
}

int insert(Node** table, int size, int key) {
    int index = hash(size, key);

    Node* head = table[index];
    int collision = (head != NULL);

    Node* newNode = (Node*) malloc(sizeof(Node));
    if (newNode == NULL) exit(EXIT_FAILURE);

    newNode->key = key;
    newNode->next = head;
    table[index] = newNode;

    return collision;
}

void freeTable(Node** table, int size) {
    for (int i = 0; i < size; i++) {
        Node* current = table[i];
        while (current) {
            Node* tmp = current;
            current = current->next;
            free(tmp);
        }
    }
    free(table);
}

void search(Node **table, int size, int key) {
    int index = hash(size, key);

    Node* head = table[index];
    if (head == NULL) {
        printf("Not found.\n");
        return;
    }

    Node* p = head;
    int iter = 0;
    while (p != NULL) {
        if (p->key == key) {
            printf("Found %c(%d) in list %d at index %d.\n", key, key, index, iter);
            return;
        }
        iter++;
        p = p->next;
    }
    printf("Not found.\n");
}

void generateRandomText(char* buffer, int size) {
    const char charset[] = " LorEm!ipsum?dolor$sit@amet, xyz123! ~Consectetur*adipiscing^elit. Qu1z_why@reading+this#nonsense? R@nd0m!junk: data{flow}=42; ~V0lUptAtE?n0n!qu3sti0n. B@ck_t0_work! Lorem*ipsum=dolor#si.I love.t%met. 7h.you.1s!isN0t@real$text. G0_@w@y! 42+xyz=99? N0pe. Just@waste^your%time. By3! ";

    for (int i = 0; i < size - 1; i++) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }

    buffer[size - 1] = '\0';
}

int main() {
    srand(time(NULL));
    
    int primes[] = {11, 13, 17, 19, 23, 29, 31, 37, 41, 47,101};
    int primesCount = sizeof(primes) / sizeof(primes[0]);

    const int textSize = 100;
    char* text = (char*)malloc(textSize + 1);
    generateRandomText(text, textSize + 1);

    printf("\n Хеширование текста методом прямого связывания \n");
    printf("| Размер | Кол-во символов | Кол-во коллизий |\n");

    for (int p = 0; p < primesCount; p++) {
        int tableSize = primes[p];
        Node** table = (Node**)calloc(tableSize, sizeof(Node*));

        int collisionCount = 0;
        for (int i = 0; i < textSize && text[i] != '\0'; i++) {
            collisionCount += insert(table, tableSize, (int) text[i]);
        }

        printf("| %4d   | %10zu      | %10d      |\n", tableSize, strlen(text), collisionCount);
        freeTable(table, tableSize);
    }

    int size = primes[0];
    Node** table = (Node**)calloc(size, sizeof(Node*));
    for (int i = 0; i < textSize && text[i] != '\0'; i++) {
        insert(table, size, (int) text[i]);
    }

    char character;
    while (character != '\n') {
        printf("Search for: ");
        scanf("%c", &character);
        search(table, size, (int) character);

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {};
    }


    freeTable(table, size);
    free(text);
    return 0;
}