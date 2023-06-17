/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr), John Petropoulos (johnpetr@csd.uoc.gr)
 * @Version 30-11-2022
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Implementation of the "pss.h" header file for the Public Subscribe System,
 * function definitions
 *
 *
 ***************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include "pss.h"

/*!!!!!!!!!!!!!!!!!!!!!!FUNCTIONS OF LEAF ORIENTED TREES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
struct TreeInfo *SearchLeafOriented(struct TreeInfo *root, int id, int tm)
{
    struct TreeInfo *t;
    t = root;
    if(t){
        while (t->tlc != NULL && t->trc != NULL)
        {
            if (t->ttm >= tm)
            {
                t = t->tlc;
            }
            else if (t->ttm < tm)
            {
                t = t->trc;
            }
        }
        if (t->ttm == tm && t->tlc == NULL && t->trc == NULL)
        {
            printf("found %d\n", t->ttm);
            return t;
        }
        else
        {
            printf("not found\n");
            return NULL;
        }
    }
}
struct TreeInfo *newNodeCons(int id, int tm)
{
    struct TreeInfo *n;
    n = (struct TreeInfo *)malloc(sizeof(struct TreeInfo));
    n->tId = id;
    n->ttm = tm;
    n->tp = NULL;
    n->prev = NULL;
    n->next = NULL;
    n->tlc = NULL;
    n->trc = NULL;
    return n;
}
struct TreeInfo *InsertLeafOriented(struct TreeInfo *root, int id, int tm)
{
    struct TreeInfo *t, *n1, *n2;
    if (root == NULL)
    {
        root = newNodeCons(id, tm);
    }
    else
    {
        t = root;
        while (t->tlc != NULL && t->trc != NULL)
        {
            if (t->ttm >= tm)
            {
                t = t->tlc;
            }
            else if (t->ttm < tm)
            {
                t = t->trc;
            }
        }
        if (t->ttm > tm)
        {
            n1 = (struct TreeInfo *)malloc(sizeof(struct TreeInfo));
            n2 = (struct TreeInfo *)malloc(sizeof(struct TreeInfo));
            n1->tId = id;
            n1->ttm = tm;
            n1->tlc = NULL;
            n1->trc = NULL;
            n1->next = n2;
            n2->tId = t->tId;
            n2->ttm = t->ttm;
            n2->tlc = NULL;
            n2->trc = NULL;
            t->ttm = tm;
            t->tId = id;
            n2->prev = n1;
            t->tlc = n1;
            t->trc = n2;
            n2->tp = t;
            n1->tp = t;
            if (t->next != NULL)
            {
                n2->next = t->next;
                t->next->prev = n2;
            }
            if (t->prev != NULL)
            {
                n1->prev = t->prev;
                t->prev->next = n1;
            }
        }
        else if (t->ttm <= tm)
        {
            n1 = (struct TreeInfo *)malloc(sizeof(struct TreeInfo)); /*left child*/
            n2 = (struct TreeInfo *)malloc(sizeof(struct TreeInfo)); /*right child*/
            n2->tId = id;
            n2->ttm = tm;
            n2->tlc = NULL;
            n2->trc = NULL;
            n2->prev = n1;
            n1->tId = t->tId;
            n1->ttm = t->ttm;
            n1->tlc = NULL;
            n1->trc = NULL;
            n1->next = n2;
            t->tId = id;
            t->ttm = tm;
            t->tlc = n1;
            t->trc = n2;
            n2->tp = t;
            n1->tp = t;
            if (t->next != NULL)
            {
                n2->next = t->next;
                t->next->prev = n2;
                t->next = NULL;
            }
            if (t->prev != NULL)
            {
                n1->prev = t->prev;
                t->prev->next = n1;
                t->prev = NULL;
            }
        }
    }

    return root;
}
struct TreeInfo *Delete(struct TreeInfo *root, int id, int tm)
{
    int found = 0;
    struct TreeInfo *t = root;
    struct TreeInfo *prev, *tmp, *prev1;
    tmp = SearchLeafOriented(root, id, tm);
    if (tmp != NULL)
    {
        found = 1;
        t = tmp;
        if (t->next != NULL && t->prev != NULL && t->tp->tlc == t)
        {
            t->next->prev = t->prev;
            t->prev->next = t->next;
            free(t->tp->tlc);
        }
        else if (t->next != NULL && t->prev != NULL && t->tp->trc == t)
        {
            t->next->prev = t->prev;
            t->prev->next = t->next;
            free(t->tp->trc);
        }
        else if (t->next == NULL && t->tp->trc == t)
        {
            t->prev->next = NULL;
            t->prev = NULL;
            free(t->tp->trc);
        }
        else if (t->prev == NULL && t->tp->tlc == t)
        {
            t->next->prev = NULL;
            t->next = NULL;
            free(t->tp->tlc);
        }
    }
    if (found == 1)
    {
        free(t);
    }

    return root;
}

void deleteTree(struct TreeInfo *root)
{
    if (root == NULL)
        return;
    /* Delete Left sub-tree */
    deleteTree(root->tlc);
    /* Delete right sub-tree */
    deleteTree(root->trc);

    /* At last, delete root node */
    printf("Deleting Node : %d\n", root->ttm);
    free(root);

    return;
}

/*!!!!!!!!!!!!!!!!!!!!!!FUNCTIONS OF LEAF ORIENTED TREES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

struct Group G[MG];
struct SubInfo *T[MG];
int hashnumber;
int hashsize;
int a;
int b;
int j = 0;
int hashfunction(int key, int a, int b)
{
    int pos;
    pos = (((a * key) + b) % hashnumber) % hashsize;
    return pos;
}

void InsertinG(struct Subscription **head, int sid)
{
    int found = 0;
    struct Subscription *sub, *sub1, *subtmp;
    sub = (struct Subscription *)malloc(sizeof(struct Subscription));
    sub->sId = sid;
    subtmp = (*head);
    while (subtmp != NULL)
    {
        if (subtmp->sId == sub->sId)
        {
            found = 1;
            if (found == 1)
            {
                return;
            }
        }
        subtmp = subtmp->snext;
    }
    sub->snext = NULL;

    if (*head == NULL) /*insert when the list is empty*/
    {
        sub->snext = *head;
        *head = sub;
    }
    else /*insert at certain position in the list*/
    {
        sub1 = *head;
        while (sub1->snext != NULL)
        {
            sub1 = sub1->snext;
        }
        sub->snext = sub1->snext;
        sub1->snext = sub;
    }
}
struct Info *newNode(int id, int tm)
{
    struct Info *info;
    info = (struct Info *)malloc(sizeof(struct Info));
    info->iId = id;
    info->itm = tm;
    info->irc = NULL;
    info->ilc = NULL;
    info->ip = NULL;
    return info;
}
struct Info *InsertBST(struct Info *root, int tm, int id)
{
    struct Info *var, *tmp;
    if (root == NULL)
    {
        return newNode(id, tm);
    }
    if (id < root->iId)
    {
        root->ilc = InsertBST(root->ilc, tm, id);
    }
    else if (id > root->iId)
    {
        root->irc = InsertBST(root->irc, tm, id);
    }
    return root;
}
void InorderTraversal(struct Info *root)
{
    if (root != NULL)
    {

        InorderTraversal(root->ilc);
        printf("(%d,%d),", root->iId, root->itm);
        InorderTraversal(root->irc);
    }
}

void InOrderPrint(struct TreeInfo *root)
{
    if (root != NULL)
    {

        InOrderPrint(root->tlc);
        printf("(%d,%d),", root->tId, root->ttm);
        InOrderPrint(root->trc);
    }
}
struct Info *PostTraversalTree(struct Info *root, int tm, int i)
{
    struct Subscription *subhead;
    int pos;
    if (root == NULL)
    {
        return NULL;
    }
    else
    {
        if (root->itm <= tm)
        {
            subhead = G[i].gsub;
            while (subhead != NULL)
            {
                pos = hashfunction(subhead->sId, a, b);
                if (T[pos]->tgp[pos] != (struct TreeInfo *)1)
                {
                    T[pos]->tgp[pos] = InsertLeafOriented(T[pos]->tgp[pos], root->iId, root->itm);
                }
                subhead = subhead->snext;
            }
        }
        root->ilc = PostTraversalTree(root->ilc, tm, i);
        root->irc = PostTraversalTree(root->irc, tm, i);
    }
    return root;
}

void InsertatHash(struct SubInfo **index, int sId, int stm)
{
    int i, found = 0;
    j = j + 1;
    struct SubInfo *tmp, *tmp1, *subtmp;
    tmp = (struct SubInfo *)malloc(sizeof(struct SubInfo));
    tmp->sId = sId;
    subtmp = (*index);
    while (subtmp != NULL)
    {
        if (subtmp->sId == tmp->sId)
        {
            found = 1;
            if (found == 1)
            {
                return;
            }
        }
        subtmp = subtmp->snext;
    }
    tmp->stm = stm;
    if (j == 1)
    {
        for (i = 0; i < MG; i++)
        {
            tmp->tgp[i] = (struct TreeInfo *)1;
            tmp->sgp[i] = (struct TreeInfo *)1;
        }
    }
    //tmp->tgp[sId] = NULL;
    //tmp->sgp[sId] = NULL;
    //tmp->snext = NULL;
    if ((*index) == NULL) /*insert when the list is empty*/
    {
        tmp->snext = (*index);
        (*index) = tmp;
    }
    else /*insert at certain position in the list*/
    {
        tmp1 = (*index);
        while (tmp1->snext != NULL && tmp1->snext->sId < tmp->sId)
        {
            tmp1 = tmp1->snext;
        }
        tmp->snext = tmp1->snext;
        tmp1->snext = tmp;
    }
}
void deleteSub(struct SubInfo **head, int sid)
{
    int i;
    struct SubInfo *current, *prev;
    current = *head;
    if ((current != NULL) && (current->sId == sid)) /*if we delete the first element of the list*/
    {
        *head = current->snext;
        free(current);
        current = NULL;
        return;
    }
    while (current != NULL && current->sId != sid) /*delete at certain position in the list
                                                     with prev pointer also*/
    {
        prev = current;
        current = current->snext;
    }
    if (current == NULL)
    {
        return;
    }

    prev->snext = current->snext;
    for (i = 0; i < MG; i++)
    {
        if (current->sgp[i] != NULL || current->sgp[i] != (struct TreeInfo *)1)
        {
            Delete(current->sgp[i], current->sId, current->stm);
        }
        if (current->tgp[i] != NULL || current->tgp[i] != (struct TreeInfo *)1)
        {
            Delete(current->tgp[i],  current->sId, current->stm);
        }
    }
    free(current);
    current = NULL;
}
int deletefromG(struct Subscription **head, int sid)
{
    struct Subscription *current, *prev;
    current = *head;
    if ((current != NULL) && (current->sId == sid)) /*delete the first element of the list*/
    {
        *head = current->snext;
        current = NULL;
        return 1;
    }
    while (current != NULL && current->sId != sid) /*delete at certain position with prev pointer*/
    {
        prev = current;
        current = current->snext;
    }
    if (current == NULL)
    {
        return 0;
    }
    prev->snext = current->snext;
    free(current);
    current = NULL;
    return 1;
}

struct Info *DeleteInfo(struct Info *root, int tm)
{

    int min;
    struct Info *tmp;
    if (root == NULL)
    {
        return NULL;
    }
    else
    {
        root->ilc = DeleteInfo(root->ilc, tm);
        root->irc = DeleteInfo(root->irc, tm);
        if (root->itm <= tm)
        {
            if (root->ilc == NULL && root->irc == NULL)
            {
                root = NULL;
                return NULL;
            }
            else if (root->ilc == NULL)
            {
                tmp = root->irc;
                root = NULL;
                return tmp;
            }
            else if (root->irc == NULL)
            {
                tmp = root->ilc;
                root = NULL;
                return tmp;
            }
            else
            {
                tmp = root->irc;
                while (tmp->ilc != NULL)
                {
                    tmp = tmp->ilc;
                }
                min = tmp->itm;
                root->itm = tmp->itm;
                root->irc = DeleteInfo(root->irc, min);
            }
        }
    }

    return root;
}
void printfunc()
{
    int i, k = 0;
    struct Info *infohead;
    struct Subscription *subhead;
    struct SubInfo *tablehead;
    for (i = 0; i < MG; i++)
    {
        infohead = G[i].gr;
        subhead = G[i].gsub;
        printf("GROUPID = <%d> , INFOLIST<", G[i].gId);
        InorderTraversal(infohead);
        printf(">,");
        printf("SUBLIST<");
        while (subhead != NULL)
        {
            printf("%d,", subhead->sId);
            subhead = subhead->snext;
        }
        printf(">\n");
    }
    printf("SUBSCRIBER LIST<");
    for (i = 0; i < MG; i++)
    {
        tablehead = T[i];
        while (tablehead != NULL)
        {
            k = k + 1;
            printf("%d,", tablehead->sId);
            tablehead = tablehead->snext;
        }
    }
    printf(">\n");
    for (i = 0; i < MG; i++)
    {
        tablehead = T[i];
        while (tablehead != NULL)
        {
            printf("SUBSCRIBERID<%d>,\n", tablehead->sId);
            printf("GROUPLIST<%d>,", G[i].gId);
            printf("TREELIST<");
            if (tablehead->tgp[i] != NULL && tablehead->tgp[i] != (struct TreeInfo *)1)
            {
                InOrderPrint(tablehead->tgp[i]);
            }
            tablehead = tablehead->snext;
        }
    }
    printf(">\n");
    printf("NO_GROUPS=<%d>,", MG);
    printf("NO_SUBSRIBERS=<%d>", k);
}
/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of the hash table.
 * @param p Prime number for the universal hash functions.
 *
 * @return 0 on success
 *         1 on failure
 */

int initialize(int m, int p)
{

    int i;
    for (i = 0; i < MG; i++)
    {
        G[i].gr = NULL;
        G[i].gId = i;
        G[i].gsub = NULL;
    }

    hashnumber = p;
    hashsize = m;
    struct SubInfo *T[hashsize];
    a = (int)rand() % hashnumber;
    b = (int)rand() % hashnumber;
    return EXIT_SUCCESS;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void)
{
    return EXIT_SUCCESS;
}

/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Insert_Info(int iTM, int iId, int *gids_arr, int size_of_gids_arr)
{
    int i, gid;
    struct Info *infohead;
    for (i = 0; i < size_of_gids_arr - 1; i++) /*insert the infos in the positions that the gids_arr tells us*/
    {
        gid = gids_arr[i];
        G[gid].gr = InsertBST(G[gid].gr, iTM, iId); /*insert at certain group*/
        G[gid].gr->igp[gid] = 1;
    }
    for (i = 0; i < size_of_gids_arr - 1; i++)
    {
        infohead = G[gids_arr[i]].gr;
        printf("GROUPID = <%d> , INFOLIST<", G[gids_arr[i]].gId);
        InorderTraversal(G[gids_arr[i]].gr);
        printf(">\n");
    }

    return EXIT_SUCCESS;
}
/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(int sTM, int sId, int *gids_arr, int size_of_gids_arr)
{
    int i, gid, pos;
    struct SubInfo *subhead;
    struct Subscription *temp;
    pos = hashfunction(sId, a, b);
    for (i = 0; i < size_of_gids_arr - 1; i++) /*insert to the Subscription and then to the G Subscription*/
    {
        gid = gids_arr[i];
        InsertatHash(&T[pos], sId, sTM);
        InsertinG(&G[gids_arr[i]].gsub, sId);
        /*if (gid < MG)
        {

        }*/
    }
    subhead = T[pos];
    printf("SUBSCRIBER LIST<");
    while (subhead != NULL)
    {
        printf("%d,", subhead->sId);
        subhead = subhead->snext;
    }
    printf(">\n");
    for (i = 0; i < size_of_gids_arr - 1; i++)
    {
        gid = gids_arr[i];
        /*if (gid < MG)
        {}*/
        printf("GROUPID=<%d>, SUBLIST<", G[gids_arr[i]].gId);
        temp = G[gids_arr[i]].gsub;
        while (temp != NULL)
        {
            printf("%d,", temp->sId);
            temp = temp->snext;
        }
        printf(">\n");
    }

    return EXIT_SUCCESS;
}
/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
int Prune(int tm)
{
    int i, pos;
    struct Info *infohead, *infohead1;
    struct Subscription *subhead;
    struct SubInfo *tablehead;
    struct TreeInfo *temp;
    for (i = 0; i < MG; i++)
    {
        G[i].gr = PostTraversalTree(G[i].gr, tm, i);
    }
    for (i = 0; i < MG; i++)
    {
        G[i].gr = DeleteInfo(G[i].gr, tm);
    }
    for (i = 0; i < MG; i++)
    {
        infohead = G[i].gr;
        subhead = G[i].gsub;
        if (infohead != NULL)
        {
            printf("GROUPID = <%d> , INFOLIST<", G[i].gId);
            InorderTraversal(infohead);
            printf(">,");
            printf("SUBLIST<");
            while (subhead != NULL)
            {
                printf("%d,", subhead->sId);
                subhead = subhead->snext;
            }
            printf(">\n");
        }
        tablehead = T[i];
        if (tablehead != NULL)
        {
            printf("SUBSCRIBER LIST<");
            while (tablehead != NULL)
            {
                printf("%d,", tablehead->sId);
                printf(">\n");
                printf("GROUPID=<%d>, TREELIST<", G[i].gId);
                temp = tablehead->tgp[i];
                if (temp != NULL && temp != (struct TreeInfo *)1)
                {
                    InOrderPrint(temp);
                }
                printf(">\n");
                tablehead = tablehead->snext;
            }
        }
    }

    return EXIT_SUCCESS;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId)
{

    int i, pos;
    struct Info *infohead, *infohead1;
    struct TreeInfo *treeroot;
    struct SubInfo *trav, *trav1, *tablehead;
    pos = hashfunction(sId, a, b);
    trav1 = T[pos];
    if (trav1 != NULL)
    {
        for (i = 0; i < MG; i++)
        {
            treeroot = trav1->tgp[i];
            if (treeroot != (struct TreeInfo *)1 && treeroot != NULL)
            {
                while (treeroot->trc != NULL)
                {
                    treeroot = treeroot->trc;
                }
                T[pos] = trav1;
                T[pos]->sgp[i] = treeroot;
            }
        }
    }

    tablehead = T[pos];
    while (tablehead != NULL)
    {
        for (i = 0; i < MG; i++)
        {
            infohead = G[i].gr;
            if (tablehead->tgp[i] != (struct TreeInfo *)1 && tablehead->tgp[i] != NULL)
            {
                printf("GROUPID = <%d> , TREELIST<", G[i].gId);
                InOrderPrint(tablehead->tgp[i]);
                printf(">,");
                printf("NEWSGP<");
                if (tablehead->sgp[i] != (struct TreeInfo *)1 && tablehead->tgp[i] != NULL)
                {
                    printf("%d", tablehead->sgp[i]->tId);
                }
                printf(">\n");
            }
        }
        tablehead = tablehead->snext;
    }

    return EXIT_SUCCESS;
}
/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId)
{
    int i, l, array[MG], pos;
    struct SubInfo *subhead, *tablehead;
    struct Subscription *temp;
    for (i = 0; i < MG; i++)
    {
        l = deletefromG(&G[i].gsub, sId); /*delete the subscriber from the group*/
        array[i] = l;
    }
    pos = hashfunction(sId, a, b);
    deleteSub(&T[pos], sId);
    printf("SUBSCRIBER LIST<");
    subhead = T[pos];
    while (subhead != NULL)
    {
        printf("%d,", subhead->sId);
        subhead = subhead->snext;
    }
    printf(">\n");
    for (i = 0; i < MG; i++)
    {
        if (array[i] == 1)
        {
            printf("GROUPID=<%d>, SUBLIST<", G[i].gId);
            temp = G[i].gsub;
            while (temp != NULL)
            {
                printf("%d,", temp->sId);
                temp = temp->snext;
            }
            printf(">\n");
        }
    }

    return EXIT_SUCCESS;
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void)
{
    printfunc();
    return EXIT_SUCCESS;
    
}
