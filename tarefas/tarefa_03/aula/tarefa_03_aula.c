#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>

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

/* Tree struct. */
typedef struct TreeNode {
    int val;                /* Value of node. */
    struct TreeNode* left;  /* Left node. */
    struct TreeNode* right; /* Right node. */
} TreeNode;

void ClearScreen( void );
void PressEnter( void );

void Insert( head_t*, int );
void Show( const head_t* );

TreeNode* sortedListToBST( ListNode* );
ListNode* encontrarMeio( ListNode* );
void ShowTree( TreeNode*, int );
void ClearTree( TreeNode* );

int main( void ) {
    ClearScreen();

    head_t* sentinela = calloc( 1, sizeof( head_t ) );
    if ( !sentinela ) {
        printf( "Error - Memory could not be allocated dynamically!\n\n" );
        return 1;
    }

    Insert( sentinela, -10 );
    Insert( sentinela, -3 );
    Insert( sentinela, 0 );
    Insert( sentinela, 5 );
    Insert( sentinela, 9 );

    Show( sentinela );

    TreeNode* root = sortedListToBST( sentinela->first );

    printf( "BST:\n\n" );
    ShowTree( root, 0 );

    ClearTree( root );

    free( sentinela );

    sentinela = NULL;

    return 0;
}

/*!
 * @brief Linked list to BST.
 * @param [in] head Linked list variable.
 * @return Tree node.
 */
struct TreeNode* sortedListToBST( ListNode* head ) {
    if ( !head ) {
        return NULL;
    }

    struct ListNode* meio = encontrarMeio( head );

    struct TreeNode* root = malloc( sizeof( struct TreeNode ) );
    if ( !root ) {
        return NULL;
    }

    root->val = meio->val;
    root->left = NULL;
    root->right = NULL;

    if ( head not_eq meio ) {
        root->left = sortedListToBST( head );
        root->right = sortedListToBST( meio->next );
    }

    return root;
}

/*!
 * @brief Mid node in Linked list.
 * @param [in] head Linked list variable.
 * @return Mid node.
 */
struct ListNode* encontrarMeio( ListNode* head ) {
    struct ListNode* lento = head;
    struct ListNode* rapido = head;
    struct ListNode* anterior = NULL;

    while ( rapido and rapido->next ) {
        anterior = lento;
        lento = lento->next;
        rapido = rapido->next->next;
    }

    if ( anterior ) {
        anterior->next = NULL;
    }

    return lento;
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

    if ( !sentinela->first ) {
        printf( "Linked list:\n\n" );
        PressEnter();
        ClearScreen();
        return;
    }

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
 * @brief Display tree in a visual format (rotated 90 degrees - Reverse In-order).
 * @param [in] root Tree node.
 * @param [in] level Current depth level.
 * @par Returns
 *    Nothing.
 */
void ShowTree( TreeNode* root, int level ) {
    if ( !root ) {
        return;
    }

    ShowTree( root->right, level + 1 );

    for ( int i = 0; i < level; i++ ) {
        printf( "    " );
    }
    printf( "%d\n", root->val );

    ShowTree( root->left, level + 1 );
}

/*!
 * @brief Clear tree (free all nodes).
 * @param [in] root Tree node.
 * @par Returns
 *    Nothing.
 */
void ClearTree( TreeNode* root ) {
    if ( !root ) {
        return;
    }

    ClearTree( root->left );
    ClearTree( root->right );

    free( root );
}
