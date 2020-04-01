#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//identifica se esta rodando em windows ou outro S.O.
#if defined(_WIN32) || defined(WIN32)
#define clearBuffer() fflush(stdin)
#define clearScreen() system("cls")
#else
#include <stdio_ext.h>
#define clearBuffer() __fpurge(stdin)
#define clearScreen() system("clear")
#endif

struct node
{
    char texto[30];
    struct node *previous;
    struct node *next;
};

typedef struct node Node;
typedef struct node* List;

List* createList();
void insertBefore(List *txtList, char newstring[28], List *cursor2);
void insertAfter(List *txtList, char newstring[28], List *cursor2);
void changeText(List *txtList, char newstring[28], List *cursor2);
int searchInTxt(List *txtList, char newstring[28], List *cursor2);
void readFile(List *txtList, char nfile[5], List *cursor2);
void printList(List *txtList);
int seekSpecialChar(char fileChar);
void nameFile(char number, char *nfile);
void removeEnter(char *pChar);
void command(char str[30], char newstr[28]);
void insertWordinNode(List *txtList, char string[20], List *cursor2);
void insertSpecialinNode(List *txtList, char string2, List *cursor2);
void initialCursor(List *txtList, List *cursor2);
void removeWord(List *txtList, List *cursor2);
void forwardCursor(List *txtList, int number, List *cursor2);
void backwardCursor(List *txtList, int number, List *cursor2);
void endingCursor(List *txtList, List *cursor2);
void freeList(List *txtList);

int main()
{
    int *pos, count, numPos = 1, vrf;
    char num, option[30], option2[28];
    memset(option, 0, 30);
    memset(option2, 0, 28);
    char enter[] = "<ENTER>", enter2[1];
    enter2[0] = '\n';
    List *textList, *cursor;
    char* filen = (char *) malloc(5*sizeof(char)); //file name

    textList = createList();
    cursor = createList();
    if (!textList)
    {
        exit(1);
    }
    if (!cursor)
    {
        exit(1);
    }

    pos = (int *) calloc(numPos, sizeof(int));
    if (!pos)
    {
        return 0;
    }

    /*Estou usando scanf(" ") para limpar o buffer do teclado, pois o run.codes nao aceitou setbuf(stdin, NULL), 
    nem fflush(stdin), nem __fpurge(stdin)*/
    scanf(" ");
    scanf("%c", &num);
    nameFile(num, filen);
    readFile(textList, filen, cursor);

    while (option[0] != 's')
    {
        scanf(" ");
        fgets(option, sizeof(option), stdin);

        removeEnter(option);

        memset(option2, 0, 28);
        command(option, option2);
        if (strcmp(enter, option2) == 0)
        {
            strcpy(option2, enter2);
        }

        switch(option[0])
        {
        case 'a':
            insertAfter(textList, option2, cursor);
            break;
        case 'b':
            initialCursor(textList, cursor);
            break;
        case 'd':
            removeWord(textList, cursor);
            break;
        case 'e':
            endingCursor(textList, cursor);
            break;
        case 'f':
            vrf = searchInTxt(textList, option2, cursor);
            if (vrf != -1)
            {
                numPos = numPos + 1;
                pos = (int *) realloc(pos, numPos);
                if (!pos)
                {
                    return 0;
                }
                pos[numPos-2] = searchInTxt(textList, option2, cursor);
            }
            
            break;
        case 'g':
            count = atoi(option2);
            if (count > 0)
            {
                forwardCursor(textList, count, cursor);
            }
            else
            {
                backwardCursor(textList, count, cursor);
            }
            break;
        case 'i':
            insertBefore(textList, option2, cursor);
            break;
        case 'n':
            forwardCursor(textList, 1, cursor);
            break;
        case 'p':
            backwardCursor(textList, -1, cursor);
            break;
        case 'r':
            changeText(textList, option2, cursor);
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < (numPos - 1); i++)
    {
        printf("%d\n", pos[i]);
    }

    printList(textList);
    freeList(textList);

    return 0;
}

List* createList()
{
    List *lista = (List *) calloc(1, sizeof(List));
    if (lista != NULL)
    {
        (*lista) = NULL;
        return lista;
    }
    else
    {
        return NULL;
    }
}

void insertBefore(List *txtList, char newstring[28], List *cursor2) //i "string"
{
    Node *newnode;
    newnode = (Node *) calloc(1, sizeof(Node));

    if ((!txtList) || (!cursor2))
    {
        return;
    }

    if (newnode != NULL)
    {
        newnode->next = NULL;
        newnode->previous = NULL;
    }
    else
    {
        return;
    }

    strcpy((newnode->texto), newstring);
    if (((*cursor2)->previous) == NULL)
    {

    }
    else
    {
        newnode->previous = (*cursor2)->previous;
        (*cursor2)->previous->next = newnode;
        newnode->next = (*cursor2);
        (*cursor2)->previous = newnode;
    }

}

void insertAfter(List *txtList, char newstring[28], List *cursor2) //a "string"
{
    Node *newnode;
    newnode = (Node *) calloc(1, sizeof(Node));

    if (!txtList || !cursor2)
    {
        return;
    }

    if (newnode != NULL)
    {
        newnode->next = NULL;
        newnode->previous = NULL;
    }
    else
    {
        return;
    }

    strcpy((newnode->texto), newstring);

    newnode->next = (*cursor2)->next;
    newnode->previous = (*cursor2);
    (*cursor2)->next = newnode;
}

void changeText(List *txtList, char newstring[28], List *cursor2) //r "string"
{
    Node *auxList;

    if (txtList != NULL)
    {
        auxList = (*txtList);
    }
    else
    {
        return;
    }

    if ((*cursor2) == NULL)
    {
        while (auxList->next != NULL)
        {
            auxList = auxList->next;
        }

        (*cursor2) = auxList;
        strcpy(((*cursor2)->texto), newstring);
    }
    else
    {
        strcpy(((*cursor2)->texto), newstring);
    }
}

//imprime a lista (s "string")
void printList(List *txtList)
{
    Node *auxList;
    int nextChar;

    if(!txtList)
    {
        return;
    }

    auxList = (*txtList);
    while((auxList != NULL) && ((auxList->texto[0]) != 0))
    {
        if (auxList->next != NULL)
        {
            nextChar = seekSpecialChar(auxList->next->texto[0]);
        }
        else
        {
            nextChar = seekSpecialChar(auxList->texto[0]);
        }

        if (nextChar == 0)
        {
            printf("%s ", (auxList->texto));
            auxList = auxList->next;
        }
        else if (nextChar == 1)
        {
            printf("%s", auxList->texto);
            auxList = auxList->next;
        }
    }
}

//retorna 0 se nao for um caracter especial
//retorna 1 se for um caracter especial
//retorna -1 se der algo errado
int seekSpecialChar(char fileChar)
{
    if ((fileChar >= 'A' && fileChar <= 'Z') || (fileChar >= 'a' && fileChar <= 'z') || (fileChar >= '0' && fileChar <= '9'))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//atribui a extensao ".ext" ao numero informado por teclado
void nameFile(char number, char *nfile)
{
    char format[4] = ".ext";

    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
        {
            nfile[i] = number;
        }
        else
        {
            nfile[i] = format[i-1];
        }
    }

}

//remove o '\n' que o fgets pega
void removeEnter(char *pChar)
{
    while (*pChar)
    {
        if ((*pChar) == '\n')
        {
            (*pChar) = '\0';
            break;
        }
        else
        {
            pChar++;
        }
    }
}

//identifica o que esta sendo passado apos um comando
void command(char str[30], char newstr[28])
{
    int size;
    size = strlen(str);
    if (size > 1)
    {
        for (int i = 2; i < size; i++)
        {
            newstr[i-2] = str[i];
        }
    }
    else
    {
        newstr[0] = str[0];
    }
}

void insertWordinNode(List *txtList, char string[20], List *cursor2)
{
    Node *newnode, *auxList; //auxList eh o auxiliar, para eu nao perder o ponteiro do inicio do txtList

    newnode = (Node *) calloc(1, sizeof(Node));

    if ((!newnode) || (!txtList))
    {
        return;
    }

    memset((newnode->texto), 0, 30);
    auxList = (*txtList);

    if (string[0] != 0) //se a string nao estiver vazia atribui ao newnode
    {
        strcpy((newnode->texto), string);
    }

    if ((*txtList) == NULL) //lista vazia
    {
        (*txtList) = newnode;
        (*txtList)->previous = NULL;
        (*txtList)->next = NULL;
        (*cursor2) = (*txtList);
    }
    else
    {
        while (auxList->next != NULL)
        {
            auxList = auxList->next;
        }
        newnode->next = auxList->next;
        auxList->next = newnode;
        newnode->previous = auxList;
    }
}

//funcao que le do arquivo
void readFile(List *txtList, char nFile[5], List *cursor2)
{
    FILE* fp;
    int specialChar, j = 0;
    int str;
    char nonspecial[30]; //nonspecial == string de caracteres nao especiais, str eh a string lida do arquivo
    memset(nonspecial, 0, 30);

    fp = fopen(nFile, "r");
    if (fp == NULL)
    {
        return;
    }

    while (!feof(fp))
    {
        str = fgetc(fp);

        specialChar = seekSpecialChar(str);
        if (1) //se a string nao estiver vazia
        {
            if (specialChar == 1) //se for um caracter especial
            {
                if (nonspecial[0] != 0)
                {
                    insertWordinNode(txtList, nonspecial, cursor2); //insere a palavra num no da lista
                    memset(nonspecial, 0, 30);
                    if ((str != ' ') && (str != 0xffffffff))
                    {
                        insertSpecialinNode(txtList, str, cursor2); //insere o caracter especial num no exclusivo
                    }
                    
                    j = 0;
                }
                else
                {
                    if ((str != ' ') && (str != 0xffffffff))
                    {
                        insertSpecialinNode(txtList, str, cursor2);
                    }
                    
                    memset(nonspecial, 0, 30);
                    j = 0;
                }
            }
            else if (specialChar == 0) //se for um caracter nao especial
            {
                nonspecial[j] = str; //adiciona o caracter à string
                j++;
            }
        }

    }

    fclose(fp);
}

//insere um caracter especial num no exclusivo
void insertSpecialinNode(List *txtList, char string2, List *cursor2)
{
    Node *newnode, *auxList;

    newnode = (Node *) calloc(1, sizeof(Node));

    if ((!newnode) || (!txtList))
    {
        return;
    }

    memset((newnode->texto), 0, 30);
    auxList = (*txtList);
    if (string2 != 0xffffffff) //string2 eh o caracter especial lido do arquivo
    {
        newnode->texto[0] = string2;
    }

    if ((*txtList) == NULL)
    {
        (*txtList) = newnode;
        (*txtList)->next = NULL;
        (*txtList)->previous = NULL;
        (*cursor2) = (*txtList);
    }
    else
    {
        while (auxList->next != NULL)
        {
            auxList = auxList->next;
        }
        newnode->next = auxList->next;
        auxList->next = newnode;
        newnode->previous = auxList;
    }
}

//retorna -1 se nao encontrar
//retorna o indice se encontrar
int searchInTxt(List *txtList, char newstring[28], List *cursor2)
{
    Node *auxList, *auxCursor;
    int cont = 0, cont1 = 0, cont2 = 0;
    //cont1 = contador do inicio da lista ate a posicao do cursor
    //cont2 = contador da posicao do cursor ate a posicao da palavra procurada
    //cont = soma dos dois contadores

    if ((*txtList) != NULL)
    {
        auxList = (*txtList);
        auxCursor = (*cursor2);
    }
    else
    {
        return -1;
    }

    while (auxList != (*cursor2))
    {
        auxList = auxList->next;
        cont1++;
    }

    while (auxCursor != NULL)
    {
       if ((strcmp(auxCursor->texto, newstring)) == 0)
       {
           cont = cont1 + cont2;
           return cont;
       }
       else
       {
           auxCursor = auxCursor->next;
           cont2++;
       }
    }
    return -1;
}

void forwardCursor(List *txtList, int number, List *cursor2)
{
    Node *auxList;

    if ((txtList == NULL) || ((*txtList) == NULL))
    {
        return;
    }

    if ((cursor2 == NULL) || ((*cursor2) == NULL))
    {
        return;
    }

    auxList = (*cursor2);
    for (int i = 0; i < number; i++)
    {
        if (auxList->next != NULL)
        {
            auxList = auxList->next;
        }
    }
    if (auxList != NULL)
    {
        (*cursor2) = auxList;
    }
}

void backwardCursor(List *txtList, int number, List *cursor2)
{
    Node *auxList;

    if ((!txtList) || ((*txtList) == NULL))
    {
        return;
    }

    if ((!cursor2) || ((*cursor2) == NULL))
    {
        return;
    }

    number = number * (-1);
    auxList = (*cursor2);
    for (int i = 0; i < number; i++)
    {
        if (auxList->previous != NULL)
        {
            auxList = auxList->previous;
        }
    }

    if (auxList != NULL)
    {
        (*cursor2) = auxList;
    }
}


void removeWord(List *txtList, List *cursor2)
{
    Node *auxList;

    if ((!txtList) || ((*txtList) == NULL))
    {
        return;
    }
    if ((!cursor2) || ((*cursor2) == NULL))
    {
        return;
    }

    auxList = (*cursor2);
    if ((*cursor2)->next != NULL)
    {
        (*cursor2)->previous->next = (*cursor2)->next;
        (*cursor2)->next->previous = (*cursor2)->previous;
        (*cursor2) = (*cursor2)->next;
        free(auxList);
    }
    else
    {
        (*cursor2)->previous->next = NULL;
        (*cursor2) = (*cursor2)->previous;
        free(auxList);
    }

}

void initialCursor(List *txtList, List *cursor2)
{
    if ((!cursor2) || (!txtList))
    {
        return;
    }

    (*cursor2) = (*txtList);
}

void endingCursor(List *txtList, List *cursor2)
{
    Node *auxList;

    if ((!cursor2) || (!txtList))
    {
        return;
    }

    auxList = (*txtList);

    while ((auxList->next) != NULL)
    {
        auxList = auxList->next;
    }

    if (auxList != NULL)
    {
        (*cursor2) = auxList;
    }
}

void freeList(List *txtList)
{
     if (txtList != NULL)
    {
        Node* no;
        while ((*txtList) != NULL)
        {
            no = (*txtList);
            (*txtList) = (*txtList)->next;
            free(no);
        }
        free(txtList);
    }
}
