#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void Show( const int*, int );
void Test( int, const char*, int*, int, int );
bool CanPlaceFlowers( int*, int, int );
void Reset( int*, int );

int main( void ) {
    /* Main array. */
    int* flowerbed = (int*)calloc( 5, sizeof( int ) );
    if ( !flowerbed ) {
        return 1;
    }

    flowerbed[ 0 ] = 1;
    flowerbed[ 4 ] = 1;

    Show( flowerbed, 5 );
    Test( 1, "true", flowerbed, 5, 1 );
    Test( 2, "false", flowerbed, 5, 2 );

    /* New array size. */
    int* newFlowerBed = realloc( flowerbed, 7 * sizeof( int ) );
    if ( !newFlowerBed ) {
        free( flowerbed );
        return 1;
    }

    flowerbed = newFlowerBed;
    Reset( flowerbed, 7 );

    Show( flowerbed, 7 );
    Test( 3, "true", flowerbed, 7, 1 );
    Test( 4, "true", flowerbed, 7, 2 );
    Test( 5, "false", flowerbed, 7, 3 );

    free( flowerbed );

    return 0;
}

/*!
 * @brief Show array.
 * @param [in] flowerbed Flower bed array.
 * @param [in] flowerbedSize Size of the flower bed.
 * @par Returns
 *    Nothing.
 */
void Show( const int* flowerbed, const int flowerbedSize ) {
    printf( "\n|" );
    for ( int i = 0; i < flowerbedSize; i++ ) {
        printf( " %d |", flowerbed[ i ] );
    }
    printf( "\n\n" );
}

/*!
 * @brief Performing tests.
 * @param [in] testNumber Test number.
 * @param [in] expected Expected output.
 * @param [in] flowerbed Flower bed array.
 * @param [in] flowerbedSize Size of the flower bed.
 * @param [in] n Quantity to place.
 * @par Returns
 *    Nothing.
 */
void Test( int testNumber, const char* expected, int* flowerbed, int flowerbedSize, int n ) {
    printf( "Test %d (N=%d): %s (Output) -- %s (Expected)\n", testNumber, n,
            CanPlaceFlowers( flowerbed, flowerbedSize, n )
                ? "true"
                : "false", expected );
    Reset( flowerbed, flowerbedSize );
}

/*!
 * @brief Check if it's possible to add new flowers to the flower bed array.
 * @details Time complexity: O(N)
 * @details Space complexity: O(1)
 * @param [in] flowerbed Flower bed array.
 * @param [in] flowerbedSize Size of the flower bed.
 * @param [in] n Quantity to place.
 * @retval True Can Place.
 * @retval False Can't Place.
 */
bool CanPlaceFlowers( int* flowerbed, int flowerbedSize, int n ) {
    if ( n == 0 ) {
        return true;
    }

    if ( flowerbedSize == 1 ) {
        if ( flowerbed[ 0 ] == 0 && n == 1 ) {
            return true;
        } else {
            return false;
        }
    }

    int anterior = -1;
    for ( int i = 0; i < flowerbedSize; i++ ) { //O(N)
        if ( i == 0 && flowerbed[ i ] == 0 ) {
            if ( flowerbed[ i + 1 ] == 0 ) {
                flowerbed[ i ] = 1;
                n--;
            }
        } else if ( i + 1 < flowerbedSize ) {
            if ( anterior == 0 && flowerbed[ i ] != 1 && flowerbed[ i + 1 ] == 0 ) {
                flowerbed[ i ] = 1;
                n--;
            }
        } else {
            if ( anterior == 0 && flowerbed[ i ] == 0 ) {
                flowerbed[ i ] = 1;
                n--;
            }
        }
        anterior = flowerbed[ i ];

        if ( n == 0 ) {
            return true;
        }
    }

    return false;
} // O(N)

/*!
 * @brief Reset the array to perform new tests.
 * @param [in] flowerbed Flower bed array.
 * @param [in] flowerbedSize Size of the flower bed.
 * @par Returns
 *    Nothing.
 */
void Reset( int* flowerbed, const int flowerbedSize ) {
    for ( int i = 0; i < flowerbedSize; i++ ) {
        if ( i == 0 || i + 1 == flowerbedSize ) {
            flowerbed[ i ] = 1;
        } else {
            flowerbed[ i ] = 0;
        }
    }
}
