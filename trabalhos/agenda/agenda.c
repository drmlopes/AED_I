#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>

/* Length of the strings 'name', 'email' and 'search name'. */
#define TAM_STRING ( 50 * sizeof( char ) )
/* [0] Menu select (int), [1] Number of persons (int), [2] Temporary buffer (int), [3] Search name (char)[50 bytes]. */
#define HEADER ( 3 * sizeof( int ) + TAM_STRING )
/* Name (char)[50 bytes], Age (int), Email (char)[50 bytes]. */
#define AGENDA ( TAM_STRING + sizeof( int ) + TAM_STRING )

/* Macros for access to buffer fields. */

/*
 * Option selected in the menu (1-5).
 * Status flag in RemovePerson/SearchForPerson.
 * Temporary index storage.
 */
#define MENU_SELECT(buf) ( (int*)(buf) )
#define NUM_PERSONS(buf) ( *( (int*)(buf) + 1 ) )
/*
 * Iteration index in loops.
 * Buffer for getchar() in ClearKeyboardBuffer.
 */
#define TEMP_BUFFER(buf) ( (int*)(buf) + 2 )
#define SEARCH_NAME(buf) ( (char*)( (int*)(buf) + 3 ) )

#define NAME_PERSON(buf) ( (char*)(buf) + HEADER + NUM_PERSONS( buf ) * AGENDA )
#define AGE_PERSON(buf) ( (int*)( (char*)(buf) + HEADER + NUM_PERSONS( buf ) * AGENDA + TAM_STRING ) )
#define EMAIL_PERSON(buf) ( (char*)(buf) + HEADER + NUM_PERSONS( buf ) * AGENDA + TAM_STRING + sizeof( int ) )

#define NAME_PERSON_AGENDA(buf) ( (char*)(buf) + HEADER + *TEMP_BUFFER( buf ) * AGENDA )
#define AGE_PERSON_AGENDA(buf) ( (int*)( (char*)(buf) + HEADER + *TEMP_BUFFER( buf ) * AGENDA + TAM_STRING ) )
#define EMAIL_PERSON_AGENDA(buf) ( (char*)(buf) + HEADER + *TEMP_BUFFER( buf ) * AGENDA + TAM_STRING + sizeof( int ) )

void Menu( void** );
void MenuSelect( void* );

void ClearScreen( void );
void ClearKeyboardBuffer( void* );
void PressEnter( void );

void AddPerson( void** );
void RemovePerson( void** );
void SearchForPerson( void* );
void ListAll( void* );

int main( void ) {
    /* Buffer pointer.

     * [0]: Menu select (int)
     * [1]: Number of persons (int)
     * [2]: Temporary buffer (int)
     * [3]: Search name (char)[50 bytes]
     * [4+]: Agenda:
     *       - Name (char)[50 bytes]
     *       - Age (int)
     *       - Email (char)[50 bytes]
     */
    void* pBuffer = malloc( HEADER );
    if ( !pBuffer ) {
        printf( "Error - Could not dynamically allocate memory!\n\n" );
        return 1;
    }

    /* Menu select. */
    *MENU_SELECT( pBuffer ) = 0;

    /* Number of persons. */
    NUM_PERSONS( pBuffer ) = 0;

    /* Temporary buffer. */
    *TEMP_BUFFER( pBuffer ) = 0;

    /* Search name. */
    memset( SEARCH_NAME( pBuffer ), 0, TAM_STRING );

    Menu( &pBuffer );

    free( pBuffer );
    return 0;
}

/*!
 * @brief Main menu.
 * @param [in] ppBuffer Pointer to buffer pointer.
 * @par Returns
 *    Nothing.
 */
void Menu( void** ppBuffer ) {
    do {
        ClearScreen();
        MenuSelect( *ppBuffer );
        ClearScreen();

        switch ( *MENU_SELECT( *ppBuffer ) ) {
            case 1:
                AddPerson( ppBuffer );
                break;

            case 2:
                RemovePerson( ppBuffer );
                break;

            case 3:
                SearchForPerson( *ppBuffer );
                break;

            case 4:
                ListAll( *ppBuffer );
                break;

            case 5:
                break;

            default:
                printf( "Invalid option! [%d]\n\n", *MENU_SELECT( *ppBuffer ) );
                PressEnter();
                break;
        }
    } while ( *MENU_SELECT( *ppBuffer ) != 5 );
}

/*!
 * @brief Select menu options.
 * @param [in] pMenuSelect Buffer pointer.
 * @par Returns
 *    Nothing.
 */
void MenuSelect( void* pMenuSelect ) {
    /* Menu select. */
    *MENU_SELECT( pMenuSelect ) = 0;
    do {
        printf( ".:MENU:.\n\n" );
        printf( "1 - Add Person\n" );
        printf( "2 - Remove Person\n" );
        printf( "3 - Search for Person\n" );
        printf( "4 - List All\n" );
        printf( "5 - Exit\n\n" );
        printf( "Select an option: " );
        scanf( "%d", MENU_SELECT( pMenuSelect ) );
        ClearKeyboardBuffer( MENU_SELECT( pMenuSelect ) );

        if ( *MENU_SELECT( pMenuSelect ) < 1 or *MENU_SELECT( pMenuSelect ) > 5 ) {
            ClearScreen();
            printf( "Invalid option! [%d]\n\n", *MENU_SELECT( pMenuSelect ) );
            PressEnter();
            ClearScreen();
        }
    } while ( *MENU_SELECT( pMenuSelect ) < 1 or *MENU_SELECT( pMenuSelect ) > 5 );
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
 * @brief Clear keyboard buffer.
 * @param [in] pTemporaryBuffer Buffer pointer.
 * @par Returns
 *    Nothing.
 */
void ClearKeyboardBuffer( void* pTemporaryBuffer ) {
    /* Temporary buffer. */
    *TEMP_BUFFER( pTemporaryBuffer ) = 0;
    while ( ( *TEMP_BUFFER( pTemporaryBuffer ) = getchar() ) != '\n' and *TEMP_BUFFER( pTemporaryBuffer ) != EOF );
}

/*!
 * @brief Press ENTER to continue.
 * @par Parameters
 *    None.
 * @par Returns
 *    Nothing.
 */
void PressEnter( void ) {
    printf( "Press ENTER to continue..." );
    getchar();
}

/*!
 * @brief Add person to agenda.
 * @param [in] ppBuffer Pointer to buffer pointer.
 * @par Returns
 *    Nothing.
 */
void AddPerson( void** ppBuffer ) {
    /* New buffer. */
    void* newBuffer = realloc( *ppBuffer, HEADER + ( NUM_PERSONS( *ppBuffer ) + 1 ) * AGENDA );
    if ( !newBuffer ) {
        printf( "Error - Could not dynamically allocate memory!\n\n" );
        PressEnter();
        return;
    }

    *ppBuffer = newBuffer;

    printf( "Enter the person's name: " );
    fgets( NAME_PERSON( *ppBuffer ), TAM_STRING, stdin );
    ( NAME_PERSON( *ppBuffer ) )[ strcspn( NAME_PERSON( *ppBuffer ), "\n" ) ] = 0;

    /* Age of person. */
    *AGE_PERSON( *ppBuffer ) = 0;

    do {
        printf( "Enter the person's age: " );
        scanf( "%d", AGE_PERSON( *ppBuffer ) );
        ClearKeyboardBuffer( *ppBuffer );

        if ( *AGE_PERSON( *ppBuffer ) <= 0 or *AGE_PERSON( *ppBuffer ) > 120 ) {
            ClearScreen();
            printf( "Error - Invalid age! [%d]\n\n", *AGE_PERSON( *ppBuffer ) );
            PressEnter();
            ClearScreen();

            printf( "Person's name: %s\n", NAME_PERSON( *ppBuffer ) );
        }
    } while ( *AGE_PERSON( *ppBuffer ) <= 0 or *AGE_PERSON( *ppBuffer ) > 120 );

    printf( "Enter the person's email: " );
    fgets( EMAIL_PERSON( *ppBuffer ), TAM_STRING, stdin );
    ( EMAIL_PERSON( *ppBuffer ) )[ strcspn( EMAIL_PERSON( *ppBuffer ), "\n" ) ] = 0;

    ClearScreen();
    printf( "Name: %s\n", NAME_PERSON( *ppBuffer ) );
    printf( "Age: %d\n", *AGE_PERSON( *ppBuffer ) );
    printf( "Email: %s\n\n", EMAIL_PERSON( *ppBuffer ) );

    /* Number of persons. */
    NUM_PERSONS( *ppBuffer ) += 1;
    PressEnter();
}

/*!
 * @brief Remove person to agenda.
 * @param [in] ppBuffer Pointer to buffer pointer.
 * @par Returns
 *    Nothing.
 */
void RemovePerson( void** ppBuffer ) {
    /* Number of persons. */
    if ( NUM_PERSONS( *ppBuffer ) == 0 ) {
        printf( "Error - Empty!\n\n" );
        PressEnter();
        return;
    }

    printf( "Enter the person's name: " );
    fgets( SEARCH_NAME( *ppBuffer ), TAM_STRING, stdin );
    ( SEARCH_NAME( *ppBuffer ) )[ strcspn( SEARCH_NAME( *ppBuffer ), "\n" ) ] = 0;

    /* Reset status flag. */
    *MENU_SELECT( *ppBuffer ) = 0;

    /* Temporary buffer. */
    *TEMP_BUFFER( *ppBuffer ) = 0;

    ClearScreen();
    while ( *TEMP_BUFFER( *ppBuffer ) < NUM_PERSONS( *ppBuffer ) ) {
        if ( strcmp( SEARCH_NAME( *ppBuffer ), NAME_PERSON_AGENDA( *ppBuffer ) ) == 0 ) {
            /* Reset status flag. */
            *MENU_SELECT( *ppBuffer ) = 0;

            do {
                printf( "Contact [%d]\n", *TEMP_BUFFER( *ppBuffer ) + 1 );
                printf( "Name: %s\n", NAME_PERSON_AGENDA( *ppBuffer ) );
                printf( "Age: %d\n", *AGE_PERSON_AGENDA( *ppBuffer ) );
                printf( "Email: %s\n\n", EMAIL_PERSON_AGENDA( *ppBuffer ) );

                printf( "Do you want to remove the contact from your agenda?\n" );
                printf( "1 - Yes\n" );
                printf( "2 - No\n" );
                printf( "3 - Exit\n\n" );
                printf( "Select an option: " );
                scanf( "%d", MENU_SELECT( *ppBuffer ) );

                PressEnter();

                if ( *MENU_SELECT( *ppBuffer ) < 1 or *MENU_SELECT( *ppBuffer ) > 3 ) {
                    ClearScreen();
                    printf( "Invalid option! [%d]\n\n", *MENU_SELECT( *ppBuffer ) );
                    PressEnter();
                }
                ClearScreen();
            } while ( *MENU_SELECT( *ppBuffer ) < 1 or *MENU_SELECT( *ppBuffer ) > 3 );

            if ( *MENU_SELECT( *ppBuffer ) == 3 ) {
                return;
            }

            if ( *MENU_SELECT( *ppBuffer ) == 1 ) {
                /* Temporary index storage. */
                *MENU_SELECT( *ppBuffer ) = *TEMP_BUFFER( *ppBuffer );

                /* Remove the contact. */
                for ( ; *TEMP_BUFFER( *ppBuffer ) < NUM_PERSONS( *ppBuffer ) - 1; *TEMP_BUFFER( *ppBuffer ) += 1 ) {
                    memcpy( (char*)*ppBuffer + HEADER + *TEMP_BUFFER( *ppBuffer ) * AGENDA,
                            (char*)*ppBuffer + HEADER + ( *TEMP_BUFFER( *ppBuffer ) + 1 ) * AGENDA, AGENDA );
                }

                /* Temporary buffer. */
                *TEMP_BUFFER( *ppBuffer ) = *MENU_SELECT( *ppBuffer );

                /* Decrement the number of persons. */
                NUM_PERSONS( *ppBuffer ) -= 1;

                /* New buffer. */
                void* newBuffer = realloc( *ppBuffer, HEADER + NUM_PERSONS( *ppBuffer ) * AGENDA );
                if ( !newBuffer ) {
                    printf( "Error - Could not dynamically allocate memory!\n\n" );
                    NUM_PERSONS( *ppBuffer ) += 1;
                    PressEnter();
                    return;
                }

                *ppBuffer = newBuffer;

                printf( "Contact removed successfully!\n\n" );
                PressEnter();
                ClearScreen();

                *TEMP_BUFFER( *ppBuffer ) -= 1;
            }

            /* Contact found in the agenda. */
            *MENU_SELECT( *ppBuffer ) = 1;
        }

        *TEMP_BUFFER( *ppBuffer ) += 1;
    }

    if ( *MENU_SELECT( *ppBuffer ) == 0 ) {
        printf( "Error - Contact not found! [%s]\n\n", SEARCH_NAME( *ppBuffer ) );
        PressEnter();
    }
}

/*!
 * @brief Search for person in agenda.
 * @param [in] pBuffer Buffer pointer.
 * @par Returns
 *    Nothing.
 */
void SearchForPerson( void* pBuffer ) {
    /* Number of persons. */
    if ( NUM_PERSONS( pBuffer ) == 0 ) {
        printf( "Error - Empty!\n\n" );
        PressEnter();
        return;
    }

    printf( "Enter the person's name: " );
    fgets( SEARCH_NAME( pBuffer ), TAM_STRING, stdin );
    ( SEARCH_NAME( pBuffer ) )[ strcspn( SEARCH_NAME( pBuffer ), "\n" ) ] = 0;

    /* Reset status flag. */
    *MENU_SELECT( pBuffer ) = 0;

    /* Temporary buffer. */
    *TEMP_BUFFER( pBuffer ) = 0;

    ClearScreen();
    while ( *TEMP_BUFFER( pBuffer ) < NUM_PERSONS( pBuffer ) ) {
        if ( strcmp( SEARCH_NAME( pBuffer ), NAME_PERSON_AGENDA( pBuffer ) ) == 0 ) {
            printf( "Contact [%d]\n", *TEMP_BUFFER( pBuffer ) + 1 );
            printf( "Name: %s\n", NAME_PERSON_AGENDA( pBuffer ) );
            printf( "Age: %d\n", *AGE_PERSON_AGENDA( pBuffer ) );
            printf( "Email: %s\n\n", EMAIL_PERSON_AGENDA( pBuffer ) );

            /* Contact found in the agenda. */
            *MENU_SELECT( pBuffer ) = 1;
        }
        *TEMP_BUFFER( pBuffer ) += 1;
    }

    if ( *MENU_SELECT( pBuffer ) == 0 ) {
        printf( "Error - Contact not found! [%s]\n\n", SEARCH_NAME( pBuffer ) );
    }

    PressEnter();
}

/*!
 * @brief List persons in agenda.
 * @param [in] pBuffer Buffer pointer.
 * @par Returns
 *    Nothing.
 */
void ListAll( void* pBuffer ) {
    /* Number of persons. */
    if ( NUM_PERSONS( pBuffer ) == 0 ) {
        printf( "Error - Empty!\n\n" );
        PressEnter();
        return;
    }

    /* Temporary buffer. */
    *TEMP_BUFFER( pBuffer ) = 0;

    while ( *TEMP_BUFFER( pBuffer ) < NUM_PERSONS( pBuffer ) ) {
        printf( "Contact [%d]\n", *TEMP_BUFFER( pBuffer ) + 1 );
        printf( "Name: %s\n", NAME_PERSON_AGENDA( pBuffer ) );
        printf( "Age: %d\n", *AGE_PERSON_AGENDA( pBuffer ) );
        printf( "Email: %s\n\n", EMAIL_PERSON_AGENDA( pBuffer ) );

        *TEMP_BUFFER( pBuffer ) += 1;
    }

    PressEnter();
}
