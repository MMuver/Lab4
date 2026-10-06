#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* CreateTree(struct Node* root, int data)
{
    if (root == NULL)
    {
        struct Node* r = (struct Node*)malloc(sizeof(struct Node));
        if (r == NULL) exit(1);
        r->data = data;
        r->left = NULL;
        r->right = NULL;
        return r;
    }
    if (data < root->data)
        root->left = CreateTree(root->left, data);
    else
        root->right = CreateTree(root->right, data);
    return root;
}

struct Node* CreateTreeUnique(struct Node* root, int data, int* added)
{
    if (root == NULL)
    {
        struct Node* r = (struct Node*)malloc(sizeof(struct Node));
        if (r == NULL) exit(1);
        r->data = data;
        r->left = NULL;
        r->right = NULL;
        *added = 1;
        return r;
    }
    if (data == root->data)
    {
        *added = 0;
        return root;
    }
    else if (data < root->data)
    {
        root->left = CreateTreeUnique(root->left, data, added);
    }
    else
    {
        root->right = CreateTreeUnique(root->right, data, added);
    }
    return root;
}

void print_tree_helper(struct Node* r, const char* prefix, int is_left)
{
    if (r == NULL) return;

    char next_prefix[256];
    sprintf(next_prefix, "%s%s", prefix, (is_left == 1) ? "|   " : "    ");
    print_tree_helper(r->right, next_prefix, 0);

    printf("%s", prefix);
    if (is_left == 1)
        printf("\\-- ");
    else if (is_left == 0)
        printf("/-- ");
    printf("%d\n", r->data);

    sprintf(next_prefix, "%s%s", prefix, (is_left == 0) ? "|   " : "    ");
    print_tree_helper(r->left, next_prefix, 1);
}

void print_tree(struct Node* r)
{
    if (r == NULL)
    {
        printf("(дерево пусто)\n");
        return;
    }
    print_tree_helper(r, "", -1);
}

struct Node* search_tree(struct Node* root, int key, int* depth)
{
    if (root == NULL) return NULL;
    (*depth)++;
    if (root->data == key) return root;
    if (key < root->data)
        return search_tree(root->left, key, depth);
    else
        return search_tree(root->right, key, depth);
}

int count_occurrences(struct Node* root, int key)
{
    if (root == NULL) return 0;
    int count = (root->data == key) ? 1 : 0;
    if (key < root->data)
        count += count_occurrences(root->left, key);
    else
        count += count_occurrences(root->right, key);
    return count;
}

void free_tree(struct Node* root)
{
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand((unsigned int)time(NULL));

    while (1)
    {
        int tree_mode = 0;
        printf("\nВыберите тип дерева:\n");
        printf("1 - Дерево с повторами\n");
        printf("2 - Дерево без повторов\n");
        printf("0 - Выход из программы\n");
        printf("Ваш выбор: ");
        if (scanf("%d", &tree_mode) != 1 || tree_mode == 0)
            break;

        if (tree_mode != 1 && tree_mode != 2)
        {
            printf("Неверный ввод!\n");
            continue;
        }

        int n = 0;
        printf("Введите количество элементов: ");
        if (scanf("%d", &n) != 1 || n <= 0)
        {
            printf("Неверное количество элементов!\n");
            continue;
        }

        struct Node* root = NULL;
        printf("Сгенерированные элементы: ");
        for (int i = 0; i < n; i++)
        {
            int val = rand() % 100;
            printf("%d ", val);
            if (tree_mode == 1)
            {
                root = CreateTree(root, val);
            }
            else
            {
                int added = 0;
                root = CreateTreeUnique(root, val, &added);
            }
        }
        printf("\n\nПостроенное дерево:\n");
        print_tree(root);

        while (1)
        {
            int op = 0;
            printf("\nМеню операций:\n");
            printf("1 - Поиск элемента\n");
            printf("2 - Добавление 1 элемента\n");
            printf("3 - Выход к выбору дерева\n");
            printf("Ваш выбор: ");
            if (scanf("%d", &op) != 1)
                break;

            if (op == 1)
            {
                int key;
                printf("Введите значение для поиска: ");
                if (scanf("%d", &key) != 1)
                    continue;
                int depth = 0;
                struct Node* found = search_tree(root, key, &depth);
                if (found != NULL)
                {
                    int occurrences = count_occurrences(root, key);
                    printf("Элемент %d найден на глубине %d. Количество вхождений: %d\n", key, depth, occurrences);
                }
                else
                {
                    printf("Элемент %d не найден в дереве.\n", key);
                }
            }
            else if (op == 2)
            {
                int val;
                printf("Введите число для добавления: ");
                if (scanf("%d", &val) != 1)
                    continue;
                if (tree_mode == 1)
                {
                    root = CreateTree(root, val);
                    printf("Элемент %d добавлен.\n", val);
                }
                else
                {
                    int added = 0;
                    root = CreateTreeUnique(root, val, &added);
                    if (added)
                        printf("Элемент %d успешно добавлен.\n", val);
                    else
                        printf("Элемент %d уже существует (повтор исключен).\n", val);
                }
                printf("\nОбновленное дерево:\n");
                print_tree(root);
            }
            else if (op == 3)
            {
                break;
            }
            else
            {
                printf("Неверная команда!\n");
            }
        }

        free_tree(root);
    }

    return 0;
}
