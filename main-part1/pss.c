/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr)
 * @Version 20-10-2020
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

struct Group G[MG];
struct SubInfo *S;
int j = 0;
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

void sortedInsert(struct SubInfo **first, int sId, int stm)
{
    int found = 0, i;
    j = j + 1;
    struct SubInfo *sub, *sub1, *subtmp;
    sub = (struct SubInfo *)malloc(sizeof(struct SubInfo));
    sub->sId = sId;
    subtmp = (*first);
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
    sub->stm = stm;
    if (j == 1)
    {
        for (i = 0; i < MG; i++)
        {
            sub->sgp[i] = (struct Info *)1;
        }
    }
    sub->sgp[sId] = G[sId].gfirst;
    sub->snext = NULL;
    if (*first == NULL || (*first)->stm >= sub->stm) /*insert at the beginning when the list is empty*/
    {
        sub->snext = *first;
        *first = sub;
    }
    else /*insert at certain position in the list*/
    {
        sub1 = *first;
        while (sub1->snext != NULL && sub1->snext->stm < sub->stm)
        {
            sub1 = sub1->snext;
        }
        sub->snext = sub1->snext;
        sub1->snext = sub;
    }
}

void sortedinsertDoubly(struct Info **first, struct Info **last, int iTm, int iId)
{

    int found = 0;
    struct Info *info;
    struct Info *var, *tmp;
    info = (struct Info *)malloc(sizeof(struct Info));
    info->iId = iId;
    tmp = (*first);
    while (tmp != NULL)
    {
        if (tmp->iId == info->iId)
        {
            found = 1;
            if (found == 1)
            {
                return;
            }
        }
        tmp = tmp->inext;
    }
    info->itm = iTm;
    info->inext = NULL;

    if ((*first) == NULL) /*insert when the list is empty*/
    {
        (*first) = info;
        (*last) = info;
        (*first)->iprev = NULL;
        return;
    }
    else if ((info->itm) < ((*first)->itm)) /*insert at the beginning*/
    {
        info->iprev = NULL;
        (*first)->iprev = info;
        info->inext = (*first);
        (*first) = info;
        return;
    }
    else if ((info->itm) > ((*last)->itm)) /*insert at the end*/
    {
        info->iprev = (*last);
        (*last)->inext = info;
        (*last) = info;
        return;
    }

    var = (*first)->inext; /*insert at a certain position in the list so it is sorted*/
    while ((var->itm) < (info->itm))
    {
        var = var->inext;
    }
    (var->iprev)->inext = info;
    info->iprev = var->iprev;
    var->iprev = info;
    info->inext = var;
}
void deleteSub(struct SubInfo **head, int sid)
{
    struct SubInfo *current, *prev;
    current = *head;
    if ((current != NULL) && (current->sId == sid)) /*if we delete the first element of the list*/
    {
        *head = current->snext;
        free(current);
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
    free(current);
}
int deletefromG(struct Subscription **head, int sid)
{
    struct Subscription *current, *prev;
    current = *head;
    if ((current != NULL) && (current->sId == sid)) /*delete the first element of the list*/
    {
        *head = current->snext;
        free(current);
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
    return 1;
}
void printfunc()
{
    struct Info *infohead1;
    struct Subscription *subhead5;
    int i;

    for (i = 0; i < MG; i++)
    {
        infohead1 = G[i].gfirst;
        printf("GROUPID = <%d> , INFOLIST<", G[i].gId);
        while (infohead1 != NULL)
        {
            printf("%d, ", infohead1->iId);
            infohead1 = infohead1->inext;
        }
        printf(">");
        printf("SUBLIST<");
        subhead5 = G[i].ggsub;
        while (subhead5 != NULL)
        {
            printf("%d,", subhead5->sId);
            subhead5 = subhead5->snext;
        }
        printf(">\n");
    }
}
void printfunc2()
{
    int i, k = 0;
    struct SubInfo *subhead6;
    struct Info *grouphead;
    subhead6 = S;
    while (subhead6 != NULL)
    {
        k++;
        printf("SUBSCRIBERID=<%d>", subhead6->sId);
        for (i = 0; i < MG; i++)
        {
            grouphead = subhead6->sgp[i];
            if (grouphead != NULL && grouphead != (struct Info *)1)
            {

                printf("GROUPLIST=<%d>\n", grouphead->iId);
            }
        }
        subhead6 = subhead6->snext;
    }
    printf("NO_GROUPS=<%d>,", MG);
    printf("NO_SUBSCRIBERS<%d>", k);
}

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(void)
{
    int i;
    for (i = 0; i < MG; i++)
    {
        G[i].gfirst = NULL;
        G[i].gId = i;
        G[i].glast = NULL;
        G[i].ggsub = NULL;
    }

    return 1;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void)
{

    return 1;
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
        sortedinsertDoubly(&G[gid].gfirst, &G[gid].glast, iTM, iId); /*insert at certain group*/
        G[gid].gfirst->igp[gid] = 1;
    }
    for (i = 0; i < size_of_gids_arr - 1; i++)
    {
        infohead = G[gids_arr[i]].gfirst;
        printf("GROUPID = <%d> , INFOLIST<", G[gids_arr[i]].gId);
        while (infohead != NULL)
        {
            printf("%d, ", infohead->iId);
            infohead = infohead->inext;
        }
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
    int i, gid;
    struct SubInfo *subhead;
    struct Subscription *temp;
    for (i = 0; i < size_of_gids_arr - 1; i++) /*insert to the Subscription and then to the G Subinfo*/
    {
        gid = gids_arr[i];
        if (gid < MG) /*as long as the value is smaller than MG*/
        {
            sortedInsert(&S, sId, sTM);
            InsertinG(&G[gids_arr[i]].ggsub, sId);
        }
    }
    subhead = S;
    printf("SUBSCRIBER LIST<%d>\n", subhead->sId);
    for (i = 0; i < size_of_gids_arr - 1; i++)
    {
        gid = gids_arr[i];
        if (gid < MG)
        {
            printf("GROUPID=<%d>, SUBLIST<", G[gids_arr[i]].gId);
            temp = G[gids_arr[i]].ggsub;
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
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId)
{
    int array[MG];
    struct SubInfo *head;
    struct Info *ihead;
    struct Info *infosgp, *infosgp2;
    int i, count, count2;
    head = S;
    while (head) /*Subscriber list going to the subscriber that we want to consume*/
    {
        if (head->sId == sId) break;
        head = head->snext;
    }

    for (i = 0; i < MG; i++) /*then we are going to the group that he is interested to consume the info*/
    {
        if(head){
            infosgp = head->sgp[i];
            if (infosgp != (struct Info *)1)
            {
                array[i] = 1;
                printf("GROUPID<%d>,INFOLIST<", G[i].gId);
                while (infosgp != NULL)
                {
                    printf("%d,", infosgp->iId);
                    infosgp = infosgp->inext;
                }
                head->sgp[i] = G[i].glast;
            }
            if (head->sgp[i] != NULL)
            {
                printf(">NEWSGP<%d>\n", head->sgp[i]->iId);
            }
            else
            {
                printf(">\n");
            }
        }
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
    int i, p;
    int array[MG];
    struct SubInfo *shead, *shead1, *shead2;
    struct Subscription *temp;
    deleteSub(&S, sId); /*delete the subscriber from the list*/
    for (i = 0; i < MG; i++)
    {
        p = deletefromG(&G[i].ggsub, sId); /*delete the subscriber from the group*/
        array[i] = p;
    }
    shead = S;
    printf("SUBSCRIBER LIST<");
    while (shead != NULL)
    {
        printf("%d,", shead->sId);
        shead = shead->snext;
    }
    printf(">\n");
    for (i = 0; i < MG; i++)
    {
        if (array[i] == 1) /*array for everyone that is deleted we show their values*/
        {
            printf("GROUPID=<%d>, SUBLIST<", G[i].gId);
            temp = G[i].ggsub;
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
    printfunc2();

    return EXIT_SUCCESS;
}
