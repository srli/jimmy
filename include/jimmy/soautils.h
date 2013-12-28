#ifdef __cplusplus
extern "C" {
#endif

#ifdef print_debug
#undef print_debug
#endif 

#ifdef DEBUG
#define print_debug( x, ... ) _print_debug( x, __LINE__, __FILE__, __VA_ARGS__ )
#else
#define print_debug( x, ... ) (void)(0)
#endif

#ifndef __SOA_UTILS
#define __SOA_UTILS


  /* *** GSL Includes ****/
  //#include <gsl/gsl_linalg.h>


#include <stdlib.h>
  //#include "config_file.h"
  /*
#include "mrdplot_helper.h"
#include "traj_wrap.h"
#include "permute.h"
#include "gsl_useful.h"
#include "string_util.h"
#include "rotation.h"
#include "lqr.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
*/

#define DEG2RAD(x) (x*0.01745329251994)
#define RAD2DEG(x) (x*57.29577951308232)

#ifndef CLAMP
#define CLAMP( x, min, max ) (((x)!=(x) || (x)<(min))?(min):(x)>(max)?(max):(x))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#define MIN_TIME(a,b) ((timercmp((a),(b),<))?(a):(b))
#define MAX_TIME(a,b) ((timercmp((a),(b),>))?(a):(b))

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

  enum SleepMode {
    SLEEP_NANO,
    SLEEP_BUSY
  };

  void set_sleep_mode( enum SleepMode mode );

  /*safely allocate memory*/
  void *safe_malloc( size_t size );
  void *safe_realloc( void *ptr, size_t size );
  /*displays an error message*/
  void print_error( const char *format, ... );

  /*sleeps a given number of usecs, requires schedler==FIFO to work well*/
  void reliable_usleep( int usecs );
  void rt_gettimeoday( struct timeval *tv, void *data );

  /*returns usec difference between two timevals*/
  /*if the times are very different a large correctly signed value is returned*/
  int usec_diff( struct timeval *a, struct timeval *b );

  /*sets time to seconds*/
  void set_timeval( double seconds, struct timeval *time );

  /*
  void print_gsl_matrix( gsl_matrix *A );
  void write_matrix( const char *filename, gsl_matrix *A );
  void write_vector( const char *filename, gsl_vector *A );
  double *read_matrix( const char *filename );
  gsl_matrix *read_gsl_matrix( const char *filename );
  double *read_matrix_ex( const char *filename, unsigned int *rows, unsigned int *cols );
  void print_gsl_vector( gsl_vector *a );

  void _print_debug( const char *format, int line, const char *file, ... );

  //gsl_vector *read_named_ordered_fields( ConfigurationData *cd, const char *name, struct Order *ord );
  */
  void print_vector( float *a, int size );
  void print_vectord( double *a, int size );

#endif

#ifdef __cplusplus
}
#endif


