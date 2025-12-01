#include <stdio.h>
#include <stdlib.h>

/* Node struct. */
typedef struct ListNode {
    int val;               /* Value of node. */
    struct ListNode* next; /* Next node. */
} ListNode;

/* Head struct. */
typedef struct head_t {
    ListNode* first; /* First node. */
    ListNode* last;  /* Last node. */
} head_t;

void ClearScreen( void );
void PressEnter( void );

void Insert( head_t*, int );
void Show( const head_t* );
void Clear( head_t* );

void reorderList( ListNode* );

int main( void ) {
    ClearScreen();

    head_t* sentinela = calloc( 1, sizeof( head_t ) );
    if ( !sentinela ) {
        printf( "Error - Memory could not be allocated dynamically!\n\n" );
        return 1;
    }

    for ( int i = 0; i < 24; i++ ) {
        Insert( sentinela, i + 1 );
    }

    Show( sentinela );

    reorderList( sentinela->first );

    sentinela->last = sentinela->first;

    while ( sentinela->last->next ) {
        sentinela->last = sentinela->last->next;
    }

    Show( sentinela );

    Clear( sentinela );

    free( sentinela );

    sentinela = NULL;

    return 0;
}

/*!
 * @brief Reorder linked list.
 * @param [in] head Linked list variable.
 * @par Returns
 *    Nothing.
 */
void reorderList( struct ListNode* head ) {
    if ( !head || !head->next || !head->next->next ) {
        return;
    }

    struct ListNode* firstHalf = head;
    struct ListNode* aux = head;

    while ( aux->next && aux->next->next ) {
        firstHalf = firstHalf->next;
        aux = aux->next->next;
    }

    struct ListNode* secondHalf = firstHalf->next;

    aux = NULL;
    firstHalf->next = NULL; // Cut in the middle.

    while ( secondHalf ) {
        struct ListNode* tempNext = secondHalf->next;

        secondHalf->next = aux;
        aux = secondHalf;
        secondHalf = tempNext;
    }

    firstHalf = head;
    secondHalf = aux;

    while ( secondHalf ) {
        struct ListNode* tempNextFirstHalf = firstHalf->next;
        struct ListNode* tempNextSecondHalf = secondHalf->next;

        firstHalf->next = secondHalf;
        secondHalf->next = tempNextFirstHalf;

        firstHalf = tempNextFirstHalf;
        secondHalf = tempNextSecondHalf;
    }
}

/*!
 * @brief Clear prompt/terminal.
 * @par Parameters
 *    None.
 * @par Returns
 *    Nothing.
 */
void ClearScreen( void ) {
    printf( "\033[H\033[2J\033[3J" );
}

/*!
 * @brief Press ENTER to continue.
 * @par Parameters
 *    None.
 * @par Returns
 *    Nothing.
 */
void PressEnter( void ) {
    printf( "Press ENTER..." );
    getchar();
}

/*!
 * @brief Insert node into linked list.
 * @param [in] sentinela Linked list variable.
 * @param [in] val Value of node.
 * @par Returns
 *    Nothing.
 */
void Insert( head_t* sentinela, const int val ) {
    ListNode* node = calloc( 1, sizeof( ListNode ) );
    if ( !node ) {
        printf( "Error - Insufficient memory!\n\n" );
        PressEnter();
        return;
    }

    node->val = val;

    if ( !sentinela->first ) {
        sentinela->first = node;
    } else {
        sentinela->last->next = node;
    }
    sentinela->last = node;
}

/*!
 * @brief List values in the linked list.
 * @param [in] sentinela Linked list variable.
 * @par Returns
 *    Nothing.
 */
void Show( const head_t* sentinela ) {
    ClearScreen();
    const ListNode* aux = sentinela->first;

    while ( aux ) {
        printf( "| %d ", aux->val );

        aux = aux->next;
    }
    printf( "|->NULL\n\n" );
    PressEnter();
    ClearScreen();
}

/*!
 * @brief Clear linked list.
 * @param [in] sentinela Linked list variable.
 * @par Returns
 *    Nothing.
 */
void Clear( head_t* sentinela ) {
    while ( sentinela->first ) {
        ListNode* remover = sentinela->first;

        sentinela->first = sentinela->first->next;

        free( remover );
    }

    sentinela->last = sentinela->first;
}
