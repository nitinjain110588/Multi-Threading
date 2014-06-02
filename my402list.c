#include<stdlib.h> 
#include"my402list.h" 
#include"cs402.h" 
#include<stdio.h> 
#include<sys/time.h>
#include<string.h> 
 
extern int My402ListLength(My402List *anc) 
{ 
    int length=0;
    length=anc->num_members;
    return length; 
} 
 
 
extern int My402ListEmpty(My402List *anc) 
{ 
    if(anc->num_members == 0) 
    	return TRUE; 
    else 
    	return FALSE; 
} 
 
extern int My402ListAppend(My402List *anc,void *obj) 
{ 
     
    My402ListElem *newnode=NULL,*last=NULL;
    newnode = (My402ListElem *)malloc(sizeof(My402ListElem));
    last = (My402ListElem *)malloc(sizeof(My402ListElem)); 
     
    if(newnode != NULL)
    {
	last=(anc->anchor).prev;
        newnode->next=&(anc->anchor);
        newnode->prev=last;
        newnode->obj=obj;
        last->next=newnode;
        (anc->anchor).prev=newnode;
        anc->num_members=anc->num_members + 1;
     
        return TRUE;
    }
 
    else return FALSE;     
     
} 
 
extern int My402ListPrepend(My402List *anc,void *obj) 
{ 
    My402ListElem *newnode=NULL,*first=NULL; 
    newnode = (My402ListElem *)malloc(sizeof(My402ListElem));
    first = (My402ListElem *)malloc(sizeof(My402ListElem)); 
     
    if(newnode != NULL)
    {
	first=(anc->anchor).next;
        newnode->next=first;
        newnode->prev=&(anc->anchor);
        newnode->obj=obj;
	(anc->anchor).next=newnode;
        first->prev=newnode;
        
        anc->num_members=anc->num_members + 1;
     
        return TRUE;
    }
 
    else return FALSE;     
     
} 
 
 
extern void My402ListUnlink(My402List *anc,My402ListElem *elem) 
{ 
    (elem->prev)->next=elem->next; 
    (elem->next)->prev=elem->prev;
    elem->obj=NULL; 
    free (elem); 
    anc->num_members=(anc->num_members) - 1; 
} 
 
extern void My402ListUnlinkAll(My402List *anc) 
{ 
    
    (anc->anchor).next=&(anc->anchor);
    (anc->anchor).prev=&(anc->anchor);
    (anc->anchor).obj=NULL;  
    anc->num_members=0;
    
} 
 
extern int My402ListInsertBefore(My402List *anc,void *obj,My402ListElem *elem) 
{ 
    My402ListElem *newnode=NULL,*prv=NULL;
    int ret; 
    newnode = (My402ListElem*)malloc(sizeof(My402ListElem));
    prv = (My402ListElem*)malloc(sizeof(My402ListElem));  
    
    if(newnode != NULL) 
    { 
        if(elem == NULL) 
        { 
            ret=My402ListPrepend(anc,obj); 
            anc->num_members=anc->num_members+1; 
        } 
         
        else 
        { 
	    prv=elem->prev;		    
            newnode->next=elem;
            newnode->prev=prv; 
             
            newnode->obj=obj; 
            prv->next=newnode; 
            elem->prev=newnode; 
            anc->num_members=anc->num_members+1; 
            ret=TRUE; 
        } 
 
    } 
 
           else ret=FALSE; 
    return ret; 
} 
     
extern int My402ListInsertAfter(My402List *anc,void *obj,My402ListElem *elem) 
{ 
    My402ListElem *newnode=NULL,*nxt=NULL;
    newnode = (My402ListElem*)malloc(sizeof(My402ListElem));  
    int ret; 
    if(newnode != NULL) 
    { 
        if(elem == NULL) 
        { 
            ret=My402ListAppend(anc,obj); 
            anc->num_members=anc->num_members+1; 
        } 
         
        else 
        { 
	    nxt=elem->next;
	    newnode->next=nxt; 
            newnode->prev=elem; 
            
            newnode->obj=obj; 
	    nxt->prev=newnode; 
	    elem->next=newnode; 
            
            
            anc->num_members=anc->num_members+1; 
            ret=TRUE; 
        } 
 
    } 
 
           else ret=FALSE; 
    return ret; 
} 
     
 
extern My402ListElem* My402ListFirst(My402List *anc) 
{ 
    My402ListElem *first=NULL;
    first = (My402ListElem *)malloc(sizeof(My402ListElem));  
     
    if(anc->num_members == 0) 
        first=NULL; 
 
    else 
        first=(anc->anchor).next; 
 
    return first; 
} 
 
extern My402ListElem* My402ListLast(My402List *anc) 
{ 
    My402ListElem *last=NULL;
    last = (My402ListElem *)malloc(sizeof(My402ListElem));  
     
    if(anc->num_members == 0) 
        last=NULL; 
 
    else 
        last=(anc->anchor).prev; 
 
    return last; 
} 
 
 
extern My402ListElem* My402ListNext(My402List *anc,My402ListElem *elem) 
{ 
    My402ListElem *nxt=NULL;
    nxt = (My402ListElem *)malloc(sizeof(My402ListElem));  
 
    if(elem->next == &(anc->anchor)) 
        nxt=NULL; 
 
    else 
        nxt=elem->next; 
 
    return nxt; 
} 
 
extern My402ListElem* My402ListPrev(My402List *anc,My402ListElem *elem) 
{ 
    My402ListElem *prv=NULL;
    prv = (My402ListElem *)malloc(sizeof(My402ListElem));  
 
    if(elem->prev == &(anc->anchor)) 
        prv=NULL; 
 
    else 
        prv=elem->prev; 
 
    return prv; 
} 
 
extern My402ListElem* My402ListFind(My402List *anc,void *obj) 
{ 
    My402ListElem *found,*elem=NULL;
    int i;
    found=NULL;
    elem=(anc->anchor).next;
    for(i=0;i<(anc->num_members);i++)
    {
	if(elem->obj != obj)
	{
		elem=elem->next;
 	}
 	
	else found=elem;
    }
    return found; 
} 
 
extern int My402ListInit(My402List *anc) 
{ 
    (anc->anchor).next=&(anc->anchor); 
    (anc->anchor).prev=&(anc->anchor); 
    (anc->anchor).obj=NULL; 
    anc->num_members=0; 
 
    return TRUE; 
}
