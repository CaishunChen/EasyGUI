/**	
 * |----------------------------------------------------------------------
 * | Copyright (c) 2017 Tilen Majerle
 * |  
 * | Permission is hereby granted, free of charge, to any person
 * | obtaining a copy of this software and associated documentation
 * | files (the "Software"), to deal in the Software without restriction,
 * | including without limitation the rights to use, copy, modify, merge,
 * | publish, distribute, sublicense, and/or sell copies of the Software, 
 * | and to permit persons to whom the Software is furnished to do so, 
 * | subject to the following conditions:
 * | 
 * | The above copyright notice and this permission notice shall be
 * | included in all copies or substantial portions of the Software.
 * | 
 * | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * | OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * | AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * | HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * | WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * | FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * | OTHER DEALINGS IN THE SOFTWARE.
 * |----------------------------------------------------------------------
 */
#define GUI_INTERNAL
#include "gui_linkedlist.h"

/******************************************************************************/
/******************************************************************************/
/***                           Private structures                            **/
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/***                           Private definitions                           **/
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/***                            Private variables                            **/
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/***                            Private functions                            **/
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/***                                Public API                               **/
/******************************************************************************/
/******************************************************************************/
void __GUI_LINKEDLIST_ADD_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedList_t* element) {
    if (!root->First || !root->Last) {              /* First widget is about to be created */
        element->Prev = NULL;                       /* There is no previous element */
        element->Next = NULL;                       /* There is no next element */
        root->First = element;                      /* Set as first widget */
        root->Last = element;                       /* Set as last widget */
    } else {
        element->Next = NULL;                       /* Next element of last is not known */
        element->Prev = root->Last;                 /* Previous element of new entry is currently last element */
        ((GUI_LinkedList_t *)root->Last)->Next = element;   /* Previous's element next element is current element */
        root->Last = element;                       /* Add new element as last */
    }
}

GUI_LinkedList_t* __GUI_LINKEDLIST_REMOVE_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedList_t* element) {
    GUI_LinkedList_t* prev;
    GUI_LinkedList_t* next;
    
    if (!element) {                                 /* Maybe element is not even valid? */
        return 0;
    }

    prev = element->Prev;                           /* Get previous element of current */
    next = element->Next;                           /* Get next element of current */
    
    if (prev) {                                     /* If current element has previous elemnet */
        prev->Next = next;                          /* Set new next element to previous element */
    }
    if (next) {                                     /* If current element has next element */
        next->Prev = prev;                          /* Set new previous element to next element */
    }
    if (root->First == element) {                   /* If current is the same as first */
        root->First = next;                         /* Set next element as first */
    }
    if (root->Last == element) {                    /* If current is the same as last */
        root->Last = prev;                          /* Set previous as last element */
    }
    
    element->Prev = NULL;                           /*!< Clear element pointer */
    element->Next = NULL;                           /*!< Clear element pointer */
    return element;
}

GUI_LinkedList_t* __GUI_LINKEDLIST_GETNEXT_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedList_t* element) {
    if (!element) { 
        if (root) {
            return (void *)root->First;
        } else {
            return 0;
        }
    }
    return element->Next;                           /* Get next widget of current in linked list */
}

GUI_LinkedList_t* __GUI_LINKEDLIST_GETPREV_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedList_t* element) {
    if (!element) { 
        if (root) {
            return (void *)root->Last;
        } else {
            return 0;
        }
    }
    return element->Prev;                           /* Get next widget of current in linked list */
}

uint8_t __GUI_LINKEDLIST_MOVEDOWN_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedList_t* element) {
    GUI_LinkedList_t* Prev = 0;
    GUI_LinkedList_t* Next = 0;
    GUI_LinkedList_t* NextNext = 0;
    
    if (!element || element == root->Last) {        /* Check if move is available */
        return 0;                                   /* Could not move */
    }
    
    Prev = element->Prev;                           /* Get previous element */
    Next = element->Next;                           /* Get next element */
    if (Next) {                                     /* Check if next is available */
        NextNext = Next->Next;                      /* Get next element of next element */
    }
    
    if (NextNext) {                                 /* If there is available memory */
        NextNext->Prev = element;                   /* Previous element of next next element is current element */
    } else {
        root->Last = element;                       /* There is no next next element so we will be last element after move */
    }
    
    if (Next) {                                     /* If there is next element */
        Next->Next = element;                       /* Next element will become previous and set next element as current */
        Next->Prev = Prev;                          /* Set previous element to next element as current previous */
    }
    
    element->Next = NextNext;                       /* Set next next element to current next element */
    element->Prev = Next;                           /* Set next element as previous (swap current and next elements) */
    
    if (Prev) {                                     /* Check if next exists */
        Prev->Next = Next;                          /* Set previous element to next */
    }
    
    if (root->First == element) {                   /* Check for current element */
        root->First = Next;                         /* Set new element as first in linked list */
    }
    
    return 1;                                       /* Move was successful */
}

uint8_t __GUI_LINKEDLIST_MOVEUP_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedList_t* element) {
    GUI_LinkedList_t* PrevPrev = 0;
    GUI_LinkedList_t* Prev = 0;
    GUI_LinkedList_t* Next = 0;
    
    if (!element || element == root->First) {       /* Check if move is available */
        return 0;                                   /* Could not move */
    }
    
    Prev = element->Prev;                           /* Get previous element */
    Next = element->Next;                           /* Get next element */
    if (Prev) {                                     /* Check if previous is available */
        PrevPrev = Prev->Prev;                      /* Get previous element of previous element */
    }
    
    if (PrevPrev) {                                 /* If there is available memory */
        PrevPrev->Next = element;                   /* Next element of previous previous element is current element */
    } else {
        root->First = element;                      /* There is no previous previous element so we will be first element after move */
    }
    
    if (Prev) {                                     /* If there is previous element */
        Prev->Prev = element;                       /* Previous element will become next and set previous element as current */
        Prev->Next = Next;                          /* Set next element to previous element as current previous */
    }
    
    element->Prev = PrevPrev;                       /* Set previous previous element to current previous element */
    element->Next = Prev;                           /* Set previous element as next (swap current and previous elements) */
    
    if (Next) {                                     /* Check if previous exists */
        Next->Prev = Prev;                          /* Set next element to previous */
    }
    
    if (root->Last == element) {                    /* Check for current element */
        root->Last = Prev;                          /* Set new last as first in linked list */
    }
    
    return 1;                                       /* Move was successful */
}

GUI_LinkedList_t* __GUI_LINKEDLIST_GETNEXT_BYINDEX_GEN(GUI_LinkedListRoot_t* root, uint16_t index) {
    GUI_LinkedList_t* item = __GUI_LINKEDLIST_GETNEXT_GEN(root, 0); /* Get first element */
    while (index-- && item) {                       /* Scroll to next elements */
        item = __GUI_LINKEDLIST_GETNEXT_GEN(0, (GUI_LinkedList_t *)item);
    }
    return item;                                    /* Get that item */
}

GUI_LinkedListMulti_t* __GUI_LINKEDLIST_MULTI_ADD_GEN(GUI_LinkedListRoot_t* root, void* element) {
    GUI_LinkedListMulti_t* ptr;
    ptr = __GUI_MEMALLOC(sizeof(GUI_LinkedListMulti_t));    /* Create memory for linked list */
    if (!ptr) {
        return 0;
    }
    
    ptr->Element = element;                         /* Save pointer to our element */
    __GUI_LINKEDLIST_ADD_GEN(root, &ptr->List);     /* Add element to linked list */
    
    return ptr;
}

uint8_t __GUI_LINKEDLIST_MULTI_REMOVE_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedListMulti_t* element) {
    if (!element) {                                 /* Check valid element */
        return 0;
    }
    __GUI_LINKEDLIST_REMOVE_GEN(root, (GUI_LinkedList_t *)element); /* Remove element from linked list */
    __GUI_MEMFREE(element);                         /* Free memory */
    return 1;
}

GUI_LinkedListMulti_t* __GUI_LINKEDLIST_MULTI_GETNEXT_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedListMulti_t* element) {
    if (!element) { 
        if (root) {
            return (GUI_LinkedListMulti_t *)root->First;
        } else {
            return 0;
        }
    }
    return (GUI_LinkedListMulti_t *)element->List.Next; /* Get next widget of current in linked list */
}

GUI_LinkedListMulti_t* __GUI_LINKEDLIST_MULTI_GETPREV_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedListMulti_t* element) {
    if (!element) { 
        if (root) {
            return (GUI_LinkedListMulti_t *)root->Last;
        } else {
            return 0;
        }
    }
    return (GUI_LinkedListMulti_t *)element->List.Prev; /* Get next widget of current in linked list */
}

uint8_t __GUI_LINKEDLIST_MULTI_MOVEDOWN_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedListMulti_t* element) {
    return __GUI_LINKEDLIST_MOVEDOWN_GEN(root, &element->List); /* Move down elemenet */
}

uint8_t __GUI_LINKEDLIST_MULTI_MOVEUP_GEN(GUI_LinkedListRoot_t* root, GUI_LinkedListMulti_t* element) {
    return __GUI_LINKEDLIST_MOVEUP_GEN(root, &element->List);   /* Move down elemenet */
}

uint8_t __GUI_LINKEDLIST_MULTI_FIND_REMOVE(GUI_LinkedListRoot_t* root, void* element) {
    GUI_LinkedListMulti_t* link;
    uint8_t ret = 0;
    
    __GUI_ASSERTPARAMS(root);                       /* Check input parameters */
    
    for (link = __GUI_LINKEDLIST_MULTI_GETNEXT_GEN(root, NULL); link; link = __GUI_LINKEDLIST_MULTI_GETNEXT_GEN(NULL, link)) {
        if ((void *)__GUI_LINKEDLIST_MULTI_GetData(link) == element) {  /* Check match */
            __GUI_LINKEDLIST_MULTI_REMOVE_GEN(root, link);  /* Remove element from linked list */
            ret = 1;
        }
    }
    return ret;
}

/******************************************************************************/
/******************************************************************************/
/***                          Widget linkedlist API                          **/
/******************************************************************************/
/******************************************************************************/
void __GUI_LINKEDLIST_WidgetAdd(GUI_HANDLE_ROOT_t* root, GUI_HANDLE_p h) {    
    if (root) {
        __GUI_LINKEDLIST_ADD_GEN(&root->RootList, (GUI_LinkedList_t *)h);
    } else {
        __GUI_LINKEDLIST_ADD_GEN(&GUI.Root, (GUI_LinkedList_t *)h);
    }
    __GUI_LINKEDLIST_WidgetMoveToTop(h);            /* Reset by moving to top */
    __GUI_LINKEDLIST_WidgetMoveToBottom(h);         /* Reset by moving to bottom with reorder */
}

void __GUI_LINKEDLIST_WidgetRemove(GUI_HANDLE_p h) {    
    if (__GH(h)->Parent) {
        __GUI_LINKEDLIST_REMOVE_GEN(&((GUI_HANDLE_ROOT_t *)__GH(h)->Parent)->RootList, (GUI_LinkedList_t *)h);
    } else {
        __GUI_LINKEDLIST_REMOVE_GEN(&GUI.Root, (GUI_LinkedList_t *)h);
    }
}

GUI_Byte __GUI_LINKEDLIST_WidgetMoveUp(GUI_HANDLE_p h) {
    if (__GH(h)->Parent) {
        return __GUI_LINKEDLIST_MOVEUP_GEN(&__GHR(__GH(h)->Parent)->RootList, (GUI_LinkedList_t *)h);
    }
    return __GUI_LINKEDLIST_MOVEUP_GEN(&GUI.Root, (GUI_LinkedList_t *)h);
}

GUI_Byte __GUI_LINKEDLIST_WidgetMoveDown(GUI_HANDLE_p h) {
    if (__GH(h)->Parent) {
        return __GUI_LINKEDLIST_MOVEDOWN_GEN(&__GHR(__GH(h)->Parent)->RootList, (GUI_LinkedList_t *)h);
    }
    return __GUI_LINKEDLIST_MOVEDOWN_GEN(&GUI.Root, (GUI_LinkedList_t *)h);
}

GUI_HANDLE_p __GUI_LINKEDLIST_WidgetGetNext(GUI_HANDLE_ROOT_t* parent, GUI_HANDLE_p h) {
    if (!h) {                                       /* Get first widget on list */
        if (parent) {                               /* If parent exists... */
            return (GUI_HANDLE_p)parent->RootList.First;    /* ...get first widget on parent */
        } else {
            return (GUI_HANDLE_p)GUI.Root.First;    /* Get first widget in GUI */
        }
    }
    return (GUI_HANDLE_p)__GH(h)->List.Next;        /* Get next widget of current in linked list */
}

GUI_HANDLE_p __GUI_LINKEDLIST_WidgetGetPrev(GUI_HANDLE_ROOT_t* parent, GUI_HANDLE_p h) {
    if (!h) {                                       /* Get last widget on list */
        if (parent) {                               /* If parent exists... */
            return (GUI_HANDLE_p)parent->RootList.Last; /* ...get last widget on parent */
        } else {
            return (GUI_HANDLE_p)GUI.Root.Last;     /* Get last widget in GUI */
        }
    }
    return (GUI_HANDLE_p)__GH(h)->List.Prev;        /* Get next widget of current in linked list */
}

/***
 * Widget linked list order:
 *
 * 1. Normal widgets
 * 2. Widgets with children support
 * 3. Widgets as dialog base elements
 */
GUI_Byte __GUI_LINKEDLIST_WidgetMoveToBottom(GUI_HANDLE_p h) {
    if (!__GH(h)->List.Next) {
        return 0;
    }
    while (__GH(h)->List.Next) {                    /* While device has next element */
        if (__GUI_WIDGET_IsDialogBase(h)) {         /* Widget is dialog base element */
            if (!__GUI_LINKEDLIST_WidgetMoveDown(h)) {  /* Move down */
                return 0;
            }
        } else if (__GUI_WIDGET_AllowChildren(h)) { /* Widget supports children widgets, go to the end of the list if necessary */
            if (!__GUI_WIDGET_IsDialogBase(__GH(__GH(h)->List.Next))) { /* Go down till dialog base is reached */
                if (__GH(h)->ZIndex >= __GH(__GH(h)->List.Next)->ZIndex) {  /* Check if z-index allows move */
                    if (!__GUI_LINKEDLIST_WidgetMoveDown(h)) {  /* Move down */
                        return 0;
                    }
                } else {
                    break;
                }
            } else {
                break;
            }
        } else {                                    /* Our widget does not allow sub widgets */
            if (!__GUI_WIDGET_AllowChildren(__GH(__GH(h)->List.Next))) {    /* Allow moving down only if next widget does not allow sub widgets */
                if (__GH(h)->ZIndex >= __GH(__GH(h)->List.Next)->ZIndex) {  /* Check if z-index allows move */
                    if (!__GUI_LINKEDLIST_WidgetMoveDown(h)) {  /* Move down */
                        return 0;
                    }
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
    return 1;
}

GUI_Byte __GUI_LINKEDLIST_WidgetMoveToTop(GUI_HANDLE_p h) {
    if (!__GH(h)->List.Prev) {
        return 0;
    }
    while (__GH(h)->List.Prev) {                    /* While device has previous element */
        if (__GUI_WIDGET_IsDialogBase(h)) {         /* Widget is dialog base element */
            if (__GUI_WIDGET_IsDialogBase(__GH(__GH(h)->List.Prev))) {  /* If previous widget is dialog base too */
                if (!__GUI_LINKEDLIST_WidgetMoveUp(h)) {    /* Move up widget */
                    return 0;
                }
            } else {
                break;                              /* Stop execution */
            }
        } else if (__GUI_WIDGET_AllowChildren(h)) { /* If moving widget allows children elements */
            if (__GUI_WIDGET_AllowChildren(__GH(__GH(h)->List.Prev))) { /* If previous widget allows children too */
                if (__GH(h)->ZIndex <= __GH(__GH(h)->List.Prev)->ZIndex) {  /* Check if z-index allows move */
                    if (!__GUI_LINKEDLIST_WidgetMoveUp(h)) {    /* Move up */
                        return 0;
                    }
                } else {
                    break;
                }
            } else {
                break;                              /* Stop execution */
            }
        } else {                                    /* Normal widget */
            if (__GH(h)->ZIndex <= __GH(__GH(h)->List.Prev)->ZIndex) {  /* Check if z-index allows move */
                if (!__GUI_LINKEDLIST_WidgetMoveUp(h)) {    /* Move up */
                    return 0;
                }
            } else {
                break;
            }
        }
    }
    return 1;
}

static
void PrintList(GUI_HANDLE_ROOT_t* root) {
    static uint8_t depth = 0;
    GUI_HANDLE_ROOT_t* h;
    GUI_LinkedListRoot_t* list;
    
    depth++;
    if (root) {
        list = &root->RootList;
    } else {
        list = &GUI.Root;
    }
    for (h = (GUI_HANDLE_ROOT_t *)list->First; h; h = h->Handle.List.Next) {
        __GUI_DEBUG("%*d: W: %s; A: 0x%08X; R: %lu; D: %lu\r\n", depth, depth, h->Handle.Widget->Name, (unsigned int)h, h->Handle.Flags & GUI_FLAG_REDRAW, h->Handle.Flags & GUI_FLAG_REMOVE);
        if (__GUI_WIDGET_AllowChildren(h)) {
            PrintList(h);
        }
    }
    depth--;
}

void __GUI_LINKEDLIST_PrintList(void) {
    PrintList(NULL);
}
