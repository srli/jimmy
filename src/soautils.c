#include "soautils.h"
#include <sys/time.h>
//#include "search_dirs.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <math.h>


void *safe_malloc( size_t size ) {
  void *memory;

  if( size == 0 ) return NULL;

  memory = malloc( size );
  if( memory == NULL ) {
    perror( "safe malloc: malloc failed" );
  }

  return memory;
}

void *safe_realloc( void *ptr, size_t size ) {
  void *memory;

  memory = realloc( ptr, size );
  if( memory == NULL && size != 0 ) {
    perror( "safe realloc: realloc failed" );
  }

  return memory;
}

void print_error( const char *format, ... ) {
  va_list list;
  va_start( list, format );
  char err_msg[512];

  snprintf( err_msg, 512, "--------\nERROR: %s\n--------\n", format );

  vfprintf( stderr, err_msg, list );

}


void _print_debug( const char *format, int _line, const char *_file, ... ) {
  va_list list;
  va_start( list, _file );
  char err_msg[512];

  snprintf( err_msg, 512, "debug: %s:%d: %s\n", _file, _line, format );

  vfprintf( stdout, err_msg, list );
}

static enum SleepMode sleep_mode = SLEEP_NANO;

void set_sleep_mode( enum SleepMode mode ) {
  sleep_mode = mode;
}

void reliable_usleep( int usec ) {
  if( usec <= 0 ) return;
  if( sleep_mode == SLEEP_NANO ) {
    struct timespec tv, remain;
    tv.tv_sec = (int)(usec/1e6);
    tv.tv_nsec = (int)((usec%(int)1e6)*1000);
    
    while( 0 != nanosleep( &tv, &remain ) ) {
      if (errno != EINTR ) {
	perror( "nanosleep" );
	break;
      }
      tv.tv_sec = remain.tv_sec;
      tv.tv_nsec = remain.tv_nsec;
    }
  }
  else {
    struct timeval start,now;
    gettimeofday( &start, NULL );
    do {
      gettimeofday( &now, NULL );
    } while( usec_diff( &now, &start ) < usec );
  }
}

static int _sign( double v ) {
  return v>0?1:v<0?-1:0;
}

int usec_diff( struct timeval *a, struct timeval *b ) {
  if( abs(a->tv_sec - b->tv_sec) > 1000 ) {
    return (int)(_sign(a->tv_sec - b->tv_sec) * 1e6);
  }
  return (a->tv_sec - b->tv_sec)*(int)1e6 + a->tv_usec - b->tv_usec;
}

/*  
void print_gsl_matrix( gsl_matrix *A ) {  
  if( A == NULL ) {
    printf( "NULL Matrix\n" );
    return;
  }

  unsigned int i,j;
  for( j=0; j<A->size2; j++ ) {
    printf( " %6d ", j );
  }
  printf( "\n" );
  for( i=0; i<A->size1; i++ ) {
    printf( "%2d: ", i );
    for( j=0; j<A->size2; j++ ) {
      printf( "%+06.4f ", gsl_matrix_get( A, i, j ) );
    }
    printf( "\n" );
  }
}
*/

/*writes matrix to file in raw db format*/
/*
void write_matrix( const char *filename, gsl_matrix *A ){
  FILE *file = fopen( filename, "w" );
  if( file == NULL ) {
    print_error( "Failed to open %s for writing", filename );
    return;
  }

  unsigned int d = A->size2;
  fwrite( &d, 1, sizeof( unsigned int ), file );
  d=0;
  fwrite( &d, 1, sizeof( unsigned int ), file );

  int r,c;

  for( c=0; c<A->size2; c++ ) {
    for( r=0; r<A->size1; r++ ) {
      double data = gsl_matrix_get( A, r, c );
      fwrite( &data, 1, sizeof( double ), file );
    }
  }

  fclose( file );
}

void write_vector( const char *filename, gsl_vector *A ){
  gsl_matrix_view mv = gsl_matrix_view_vector( A, 1, A->size );
  write_matrix( filename, &mv.matrix );
}

gsl_matrix *read_gsl_matrix( const char *filename ) {
  unsigned int r,c;
  double *data = read_matrix_ex( filename, &c, &r );
  if( !data ) return NULL;

  gsl_matrix *result = gsl_matrix_alloc( r, c );
  unsigned int r_idx, c_idx;
  for( r_idx=0; r_idx < r; r_idx++ ) {
    for( c_idx=0; c_idx < c; c_idx++ ) {
      gsl_matrix_set( result, r_idx, c_idx, data[c_idx + r_idx*c] );
    }
  }

  return result;
}

double *read_matrix( const char *filename ) {
  unsigned int r_size, n_records;
  
  return read_matrix_ex( filename, &r_size, &n_records );
}

double *read_matrix_ex( const char *filename, unsigned int *r_size, unsigned int *n_records ) {
  char *sfn = search_file( filename );

  FILE *file = fopen( sfn, "r" );

  free( sfn );
  if( file == NULL ) {
    print_error( "Failed to open %s for reading", filename );
    return NULL;
  }

  unsigned int tmp;
  fread( r_size, 1, sizeof( unsigned int ), file );
  fread( &tmp, 1, sizeof( unsigned int ), file );

  if( tmp != 0 ) {
    printf( "%s is a strange matrix (rng_d = %d)\n", filename, tmp );
  }

  fseek( file, 0, SEEK_END );
  long n_bytes = ftell( file );

  *n_records = (n_bytes-sizeof(unsigned int)*2)/((*r_size+tmp)*sizeof(double));
  //  printf( "%s is a %d (row) x %d (col) matrix\n", filename, *n_records, *r_size );
  
  fseek( file, sizeof(unsigned int)*2, SEEK_SET );

  double *data = (double *)safe_malloc( sizeof(double) * *n_records * *r_size );

  int r,c;
  for( c=0; c<*r_size; c++ ) {
    for( r=0; r<*n_records; r++ ) {
      fread( &(data[r* *r_size+c]), 1, sizeof( double ), file );
    }
  }

  return data;
}


void print_gsl_vector( gsl_vector *a ) {
  if( a == NULL ) {
    printf( "NULL Vector\n" );
    return;
  }

  unsigned int i;
  char *sep = a->size > 10?"\n":", ";
  for( i=0; i<a->size; i++ ) {
    printf( "%d: %6.5f%s", i, gsl_vector_get( a, i ), sep );
  }
  if( !(a->size > 10 ) ) {
    printf( "\n" );
  }
}
*/

void print_vector( float *a, int size ) {
  if( a == NULL ) {
    printf( "NULL Vector\n" );
    return;
  }

  int i;
  for( i=0; i<size; i++ ) {
    printf( "%d: %6.5f\n", i, a[i] );
  }
}

void print_vectord( double *a, int size ) {
  if( a == NULL ) {
    printf( "NULL Vector\n" );
    return;
  }

  int i;
  for( i=0; i<size; i++ ) {
    printf( "%d: %6.5f\n", i, a[i] );
  }
}

/*sets time to seconds*/
void set_timeval( double seconds, struct timeval *time ) {
  time->tv_sec = (time_t)seconds;
  time->tv_usec = (suseconds_t)((seconds - time->tv_sec)*1e6);
}
