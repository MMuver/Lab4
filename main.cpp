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

void find_all_depths(struct Node* root, int key, int current_depth, int* depths, int* count)
{
    if (root == NULL) return;
    if (key == root->data)
    {
        depths[(*count)++] = current_depth;
        find_all_depths(root->right, key, current_depth + 1, depths, count);
    }
    else if (key < root->data)
    {
        find_all_depths(root->left, key, current_depth + 1, depths, count);
    }
    else
    {
        find_all_depths(root->right, key, current_depth + 1, depths, count);
    }
}

void collect_values(struct Node* r, int* arr, int* count)
{
    if (r == NULL) return;
    arr[(*count)++] = r->data;
    collect_values(r->left, arr, count);
    collect_values(r->right, arr, count);
}

void free_tree(struct Node* root)
{
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

struct Node* detach_node(struct Node* root, int key, struct Node** target)
{
    if (root == NULL) return NULL;
    if (root->data == key && *target == NULL)
    {
        *target = root;
        return NULL;
    }
    if (key < root->data)
        root->left = detach_node(root->left, key, target);
    else
        root->right = detach_node(root->right, key, target);
    return root;
}

struct Node* modify_node_value(struct Node* root, int old_val, int new_val, int* status)
{
    int dummy_depths[10];
    int count = 0;
    find_all_depths(root, old_val, 0, dummy_depths, &count);
    if (count == 0)
    {
        *status = 1;
        return root;
    }

    if (old_val == new_val)
    {
        *status = 2;
        return root;
    }

    count = 0;
    find_all_depths(root, new_val, 0, dummy_depths, &count);
    if (count > 0)
    {
        *status = 3;
        return root;
    }

    struct Node* target = NULL;
    root = detach_node(root, old_val, &target);
    if (target == NULL)
    {
        *status = 1;
        return root;
    }

    int* desc_vals = (int*)malloc(2048 * sizeof(int));
    if (desc_vals == NULL) exit(1);
    int desc_count = 0;
    collect_values(target->left, desc_vals, &desc_count);
    collect_values(target->right, desc_vals, &desc_count);

    free_tree(target);

    for (int i = 0; i < desc_count; i++)
    {
        int added = 0;
        root = CreateTreeUnique(root, desc_vals[i], &added);
    }
    free(desc_vals);

    int added = 0;
    root = CreateTreeUnique(root, new_val, &added);
    *status = 0;
    return root;
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
            if (tree_mode == 2)
            {
                printf("3 - Изменение значения узла\n");
                printf("4 - Выход к выбору дерева\n");
            }
            else
            {
                printf("3 - Выход к выбору дерева\n");
            }
            printf("Ваш выбор: ");
            if (scanf("%d", &op) != 1)
                break;

            if (op == 1)
            {
                int key;
                printf("Введите значение для поиска: ");
                if (scanf("%d", &key) != 1)
                    continue;

                int* depths = (int*)malloc((n + 100) * sizeof(int));
                if (depths == NULL) continue;
                int count = 0;
                find_all_depths(root, key, 0, depths, &count);

                if (count == 0)
                {
                    printf("Элемент %d не найден в дереве.\n", key);
                }
                else if (count == 1)
                {
                    printf("Элемент %d найден на глубине %d. Количество вхождений: 1\n", key, depths[0]);
                }
                else
                {
                    printf("Элемент %d найден (вхождений: %d). Глубины: ", key, count);
                    for (int i = 0; i < count; i++)
                    {
                        printf("%d%s", depths[i], (i == count - 1) ? "" : ", ");
                    }
                    printf("\n");
                }
                free(depths);
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
                    n++;
                    printf("Элемент %d добавлен.\n", val);
                }
                else
                {
                    int added = 0;
                    root = CreateTreeUnique(root, val, &added);
                    if (added)
                    {
                        n++;
                        printf("Элемент %d успешно добавлен.\n", val);
                    }
                    else
                    {
                        printf("Элемент %d уже существует (повтор исключен).\n", val);
                    }
                }
                printf("\nОбновленное дерево:\n");
                print_tree(root);
            }
            else if (tree_mode == 2 && op == 3)
            {
                int old_val, new_val;
                printf("Введите значение узла для изменения: ");
                if (scanf("%d", &old_val) != 1)
                    continue;
                printf("Введите новое значение: ");
                if (scanf("%d", &new_val) != 1)
                    continue;

                int status = 0;
                root = modify_node_value(root, old_val, new_val, &status);
                if (status == 0)
                {
                    printf("Значение узла %d изменено на %d.\n", old_val, new_val);
                    printf("\nОбновленное дерево:\n");
                    print_tree(root);
                }
                else if (status == 1)
                {
                    printf("Элемент %d не найден в дереве.\n", old_val);
                }
                else if (status == 2)
                {
                    printf("Новое значение совпадает со старым.\n");
                }
                else if (status == 3)
                {
                    printf("Элемент %d уже существует в дереве (повтор исключен).\n", new_val);
                }
            }
            else if ((tree_mode == 1 && op == 3) || (tree_mode == 2 && op == 4))
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
