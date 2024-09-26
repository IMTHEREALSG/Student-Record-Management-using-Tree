#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int PRN;
    char name[50], surname[50], dept[20];
    float cgpa;
    long long int number;
} student;

struct Node
{
    struct Node *left, *right;
    int height;
    student stu;
};

struct Node *root = NULL;

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int height(struct Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

struct Node *newNode(student s)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->stu = s;
    return node;
}

struct Node *rightrotate(struct Node *y)
{
    struct Node *x = y->left;
    struct Node *z = x->right;

    x->right = y;
    y->left = z;

    x->height = max(height(x->right), height(x->left)) + 1;
    y->height = max(height(y->right), height(y->left)) + 1;

    return x;
}

struct Node *leftrotate(struct Node *y)
{
    struct Node *x = y->right;
    struct Node *z = x->left;

    x->left = y;
    y->right = z;

    x->height = max(height(x->right), height(x->left)) + 1;
    y->height = max(height(y->right), height(y->left)) + 1;

    return x;
}

int getBal(struct Node *N)
{
    if (N == NULL)
        return 0;
    return (height(N->left) - height(N->right));
}

void search(struct Node *root, int key)
{
    if (root != NULL)
    {
        if (key < root->stu.PRN)
            search(root->left, key);
        else if (key > root->stu.PRN)
            search(root->right, key);
        else if (key == root->stu.PRN)
            printf("+------------+----------------------+----------------------+------------+--------+----------------------+\n");
        printf("| %-10d | %-20s | %-20s | %-10s | %-6.2f | %-20lld |\n", root->stu.PRN, root->stu.name, root->stu.surname, root->stu.dept, root->stu.cgpa, root->stu.number);
        printf("+------------+----------------------+----------------------+------------+--------+----------------------+\n");
        exit(0);
    }
    else
        printf("Not found!\n");
    exit(0);
}

void inorder(struct Node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("| %-10d | %-20s | %-20s | %-10s | %-6.2f | %-20lld |\n", root->stu.PRN, root->stu.name, root->stu.surname, root->stu.dept, root->stu.cgpa, root->stu.number);
        printf("+------------+----------------------+----------------------+------------+--------+----------------------+\n");
        inorder(root->right);
    }
}

struct Node *insert(struct Node *node, student key)
{
    if (node == NULL)
        return (newNode(key));

    if (key.PRN < node->stu.PRN)
        node->left = insert(node->left, key);
    else if (key.PRN > node->stu.PRN)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBal(node);

    if (balance > 1 && key.PRN < node->left->stu.PRN)
        return rightrotate(node);

    if (balance < -1 && key.PRN > node->right->stu.PRN)
        return leftrotate(node);

    if (balance > 1 && key.PRN > node->left->stu.PRN)
    {
        node->left = leftrotate(node->left);
        return rightrotate(node);
    }

    if (balance < -1 && key.PRN < node->right->stu.PRN)
    {
        node->right = rightrotate(node->right);
        return leftrotate(node);
    }

    return node;
}

struct Node *minValueNode(struct Node *node)
{
    struct Node *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

struct Node *deleteNode(struct Node *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->stu.PRN)
        root->left = deleteNode(root->left, key);
    else if (key > root->stu.PRN)
        root->right = deleteNode(root->right, key);
    else
    {
        if ((root->left == NULL) || (root->right == NULL))
        {
            struct Node *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            free(temp);
        }
        else
        {
            struct Node *temp = minValueNode(root->right);
            root->stu = temp->stu;
            root->right = deleteNode(root->right, temp->stu.PRN);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBal(root);

    if (balance > 1 && getBal(root->left) >= 0)
        return rightrotate(root);

    if (balance > 1 && getBal(root->left) < 0)
    {
        root->left = leftrotate(root->left);
        return rightrotate(root);
    }

    if (balance < -1 && getBal(root->right) <= 0)
        return leftrotate(root);

    if (balance < -1 && getBal(root->right) > 0)
    {
        root->right = rightrotate(root->right);
        return leftrotate(root);
    }

    return root;
}

struct Node *sat(struct Node *root, int key)
{
    if (root != NULL)
    {
        if (key < root->stu.PRN)
            sat(root->left, key);
        else if (key > root->stu.PRN)
            sat(root->right, key);
        else if (key == root->stu.PRN)
            return root;
    }
    else
    {
        return NULL;
    }
}

void writeStudentToFile(FILE *file, struct Node *root)
{
    if (root != NULL)
    {
        writeStudentToFile(file, root->left);

        fprintf(file, "| %-10d | %-20s | %-20s | %-10s | %-6.2f | %-20lld |\n",
                root->stu.PRN, root->stu.name, root->stu.surname,
                root->stu.dept, root->stu.cgpa, root->stu.number);
        fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");
        writeStudentToFile(file, root->right);
    }
}

void writeStudentToFile_low_CGPA(FILE *file, struct Node *root)
{
    if (root != NULL)
    {
        writeStudentToFile_low_CGPA(file, root->left);

        if (root->stu.cgpa <= 7.0)
        {
            fprintf(file, "| %-10d | %-20s | %-20s | %-10s | %-6.2f | %-20lld |\n",
                    root->stu.PRN, root->stu.name, root->stu.surname,
                    root->stu.dept, root->stu.cgpa, root->stu.number);
            fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");
        }

        writeStudentToFile_low_CGPA(file, root->right);
    }
}

void writeStudent(FILE *file, struct Node *root)
{
    if (root != NULL)
    {
        writeStudent(file, root->left);

        fprintf(file, "%d %s %s %s %f %10lld\n",
                root->stu.PRN, root->stu.name, root->stu.surname,
                root->stu.dept, root->stu.cgpa, root->stu.number);

        writeStudent(file, root->right);
    }
}

void createStudentListFile(struct Node *root)
{

    FILE *file = fopen("list_of_students.txt", "w");
    FILE *loadfile = fopen("Load_File.txt", "w");
    int ch;
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");
    fprintf(file, "| %-10s | %-20s | %-20s | %-10s | %-6.2s | %-20s |\n", "PRN", "Name", "Surname", "Dept.", "CGPA", "Number");
    fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");
    writeStudentToFile(file, root);
    fclose(file);
    printf("Student data written to 'list_of_students.txt' successfully.\n");

    if (loadfile == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    writeStudent(loadfile, root);
    fclose(loadfile);

    return;
}
void AddStudentListFile(struct Node *root)
{

    FILE *file = fopen("list_of_students.txt", "a");
    FILE *loadfile = fopen("Load_File.txt", "a");
    int ch;
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    writeStudentToFile(file, root);
    writeStudent(loadfile, root);

    fclose(file);
    fclose(loadfile);

    printf("Student data Added to 'list_of_students.txt' successfully.\n");
    return;
}

void Low_CGPA_student(struct Node *root)
{
    FILE *file = fopen("Low CGPA student.txt", "w");

    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");
    fprintf(file, "| %-10s | %-20s | %-20s | %-10s | %-6.2s | %-20s |\n", "PRN", "Name", "Surname", "Dept.", "CGPA", "Number");
    fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");

    writeStudentToFile_low_CGPA(file, root);

    fclose(file);

    printf("Student data written to 'Low CGPA student.txt' successfully.\n");
    return;
}

void draftLetters(FILE *file, struct Node *root)
{
    if (root != NULL)
    {
        draftLetters(file, root->left);

        if (root->stu.cgpa < 7.0)
        {

            char fileName[50];
            sprintf(fileName, "letter_%d.txt", root->stu.PRN);
            FILE *studentFile = fopen(fileName, "w");

            if (studentFile == NULL)
            {
                printf("Error opening the file for PRN %d.\n", root->stu.PRN);
                return;
            }

            fprintf(studentFile, "Dear %s %s,\n\n", root->stu.name, root->stu.surname);
            fprintf(studentFile, "I hope this letter finds you in good health and high spirits. I am writing to address academic matters.\n");
            fprintf(studentFile, "We regret to inform you that your CGPA is below 7.0.\n");
            fprintf(studentFile, "Please take necessary steps to improve your academic performance.\n\n");
            fprintf(studentFile, "Sincerely,\nThe Academic Board\n");

            fclose(studentFile);

            printf("Letter drafted for PRN %d.\n", root->stu.PRN);
        }

        draftLetters(file, root->right);
    }
}

void draftLettersForStudents(struct Node *root)
{

    FILE *logFile = fopen("letters_log.txt", "w");

    if (logFile == NULL)
    {
        printf("Error opening the log file.\n");
        return;
    }

    fprintf(logFile, "Letters Drafted for Students with CGPA < 7.0\n\n");

    draftLetters(logFile, root);

    fclose(logFile);

    printf("Letters drafted successfully. Check 'letters_log.txt' for details.\n");
}
void scanfile()
{
    FILE *f = fopen("Load_File.txt", "r");
    if (f != NULL)
    {
        student s;
        while (!feof(f))
        {
            fscanf(f, "%d %s %s %s %f %10lld\n", &s.PRN, s.name, s.surname, s.dept, &s.cgpa, &s.number);
            root = insert(root, s);
        }
        fclose(f);
    }
    else
    {
        printf("Error opening the file for reading.\n");
    }
}

int main()
{
    student s1;
    int PRN, ch = 1;
    root = NULL;
    FILE *lfile;
    struct Node *temp = NULL;

    while (1)
    {
        printf("\n1.)Enter data\n2.)InOrder Traversal\n3.)Search\n4.)Delete\n5.)Create List of Students\n6.)Add Student to Existing File\n7.)List of lowCGPA\n8.)Draft letter\n9.)Update Mobile Number\n10.)Exit\nEnter your choice : ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            printf("Enter data for student\n");
            printf("Enter PRN : ");
            scanf("%d", &s1.PRN);
            scanfile();
            temp = sat(root, s1.PRN);
            if (temp == NULL)
            {
                root = NULL;
                temp = NULL;
                printf("Enter name : ");
                scanf("%s", s1.name);
                printf("Enter surname : ");
                scanf("%s", s1.surname);
                printf("Enter department : ");
                scanf("%s", s1.dept);
                printf("Enter CGPA : ");
                scanf("%f", &s1.cgpa);
                printf("Enter phone number: ");
                scanf("%10lld", &s1.number);
                root = insert(root, s1);
            }
            else
            {
                printf("\nPRN is Already Present\n");
                free(root);
            }

            break;
        case 2:
            scanfile();
            printf("Inorder traversal of AVL TREE : \n\n");
            printf("+------------+----------------------+----------------------+------------+--------+----------------------+\n");
            printf("| %-10s | %-20s | %-20s | %-10s | %-6.2s | %-20s |\n", "PRN", "Name", "Surname", "Dept.", "CGPA", "Number");
            printf("+------------+----------------------+----------------------+------------+--------+----------------------+\n");
            inorder(root);
            free(root);
            break;
        case 3:
            lfile = fopen("Load_File.txt", "r");
            if (lfile != NULL)
            {
                student s;
                while (!feof(lfile))
                {
                    fscanf(lfile, "%d %s %s %s %f %10lld\n", &s.PRN, s.name, s.surname, s.dept, &s.cgpa, &s.number);
                    root = insert(root, s);
                }
                fclose(lfile);
            }
            else
            {
                printf("Error opening the file for reading.\n");
            }
            printf("Enter PRN : ");
            scanf("%d", &PRN);
            search(root, PRN);
            free(root);
            break;
        case 4:
            scanfile();
            printf("Enter PRN to delete: ");
            scanf("%d", &PRN);
            temp = sat(root, PRN);
            if (temp == NULL)
            {
                printf("\nPRN:- %d is Not Present.\n", PRN);
            }
            else
            {
                root = deleteNode(root, PRN);
                FILE *file = fopen("list_of_students.txt", "w");
                FILE *loadfile = fopen("Load_File.txt", "w");

                if (file == NULL)
                {
                    printf("Error opening the file.\n");
                }

                fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");
                fprintf(file, "| %-10s | %-20s | %-20s | %-10s | %-6.2s | %-20s |\n", "PRN", "Name", "Surname", "Dept.", "CGPA", "Number");
                fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");
                writeStudentToFile(file, root);
                fclose(file);
                printf("Student Deleted successfully.\n");

                if (loadfile == NULL)
                {
                    printf("Error opening the file.\n");
                }

                writeStudent(loadfile, root);
                fclose(loadfile);
            }
            break;
        case 5:
            scanfile();
            createStudentListFile(root);
            break;
        case 6:

            AddStudentListFile(root);
            break;
        case 7:
            scanfile();
            Low_CGPA_student(root);
            free(root);
            break;
        case 8:
            draftLettersForStudents(root);
            break;
        case 9:
             lfile = fopen("Load_File.txt", "r");
            if (lfile != NULL)
            {
                student s;
                while (!feof(lfile))
                {
                    fscanf(lfile, "%d %s %s %s %f %11lld\n", &s.PRN, s.name, s.surname, s.dept, &s.cgpa, &s.number);
                    root = insert(root, s);
                }
                fclose(lfile);
            }
            else
            {
                printf("Error opening the file for reading.\n");
            }
            printf("Enter PRN of the student whose details you want to modify: ");
            scanf("%d", &PRN);
            struct Node *temp = sat(root, PRN);
            if (temp != NULL)
            {
                printf("Enter the new phone number: ");
                scanf("%10lld", &temp->stu.number);
                FILE *file = fopen("list_of_students.txt", "w");
                lfile = fopen("Load_File.txt", "w");
                if (file == NULL)
                {
                    printf("Error opening the file.\n");
                }
                fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");
                fprintf(file, "| %-10s | %-20s | %-20s | %-10s | %-6.2s | %-20s |\n", "PRN", "Name", "Surname", "Dept.", "CGPA", "Number");
                fprintf(file, "+------------+----------------------+----------------------+------------+--------+----------------------+\n");
                writeStudentToFile(file, root);
                fclose(file);
                writeStudent(lfile, root);
                fclose(lfile);
                printf("Student data updated successfully.\n");
            }
            else
            {
                printf("Student with PRN %d not found.\n", PRN);
            }
            free(root);
            break;
        case 11:
            
            exit(0);
        default:
            printf("Invalid choice\n");
        }
    }
    return 0;
}