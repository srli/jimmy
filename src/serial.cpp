#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "soautils.h"

#define SERIAL_DEBUG 0

int open_port( const char *dev ) {

  int id;
  id = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);

  if (id <0 ) {
    print_error ("Failed to open serial port.!\n");
    perror(dev);
    return -1;
  }

  return id;
}

void close_port(int id) {
  if( 0 != close(id) ) {
    perror( "close_port" );
  }  
}

/*--------------------------------------------------------------------------
 * setCommParameters
 *
 * parameters:  portNum  : a serial port number
 *              baudrate : the communication flow rate (speed).
 *              charsize : the character size (7,8 bits)
 *              parity   : 0=none, 1=odd, 2=even
 *              stopbits : the number of stop bits (1 or 2)
 *
 * returns:     M3D_COMM_OK if the settings succeeded.
 *              M3D_COMM_FAILED if there was an error.
 *--------------------------------------------------------------------------*/

int set_comm_parameters(int id,
    int baudrate,
    int dataBits,
    int parity,
    int stopBits ) {

  struct termios portOptions;
  int status;
  int BAUD, DATABITS, PARITY, PARITYON, STOPBITS;

  /*  get port options for speed, etc. */
  tcgetattr(id, &portOptions);
  tcflush(id, TCIFLUSH);

  /* baudrate settings */
  switch (baudrate)
  {
    case 115200:
      BAUD = B115200;
      break;
    case 57600:
      BAUD = B57600;
      break;
    case 38400:
      BAUD = B38400;
      break;
    case 19200:
      BAUD  = B19200;
      break;
    case 9600:
      BAUD  = B9600;
      break;
    default:
      printf( "Warning: using default baud, 38400\n" );
      BAUD = B38400;
  }

  /* databits settings */
  switch (dataBits) {
    case 8:
      DATABITS = CS8;
      break;
    case 7:
      DATABITS = CS7;
      break;
    default:
      DATABITS = CS8;
  }

  /* stop bits */
  switch (stopBits) {
    case 1:
      STOPBITS = 0;
      break;
    case 2:
      STOPBITS = CSTOPB;
      break;
    default:
      STOPBITS = 0;

  }

  /* parity */
  switch (parity) {
    case 0: /* no parity */
      PARITYON = 0;
      PARITY = 0;
      break;
    case 1: /* odd parity */
      PARITYON = PARENB;
      PARITY = PARODD;
      break;
    case 2: /* event parity */
      PARITYON = PARENB;
      PARITY = 0;
      break;
    default: /* none */
      PARITYON = 0;
      PARITY = 0;
  }

  /* We are not setting CRTSCTS which turns on hardware flow control */
  portOptions.c_cflag = BAUD | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;

  //   printf( "HC: %d\n", portOptions.c_cflag & CRTSCTS );

  //    portOptions.c_cflag = portOptions.c_cflag & ~CRTSCTS;

  /*  set for non-canonical (raw processing, no echo, etc.) */
  portOptions.c_iflag = IGNPAR; /* ignore parity check */
  portOptions.c_oflag = 0; /* raw output */
  portOptions.c_lflag = 0; /* raw input  */
  /* CGA: above achieves: new_kbd_mode.c_lflag &= ~(ICANON | ECHO); */

  cfsetospeed(&portOptions, BAUD);  /* redundant with cflag setting, above */
  cfsetispeed(&portOptions, BAUD);

  /* CGA additions */
  portOptions.c_cc[VTIME] = 0;
  portOptions.c_cc[VMIN] = 0;

  status = tcsetattr(id, TCSANOW, &portOptions);

  if (status != 0)
    return -1;
  else
    return 0;
}

int send_data(int id, const void *command, int commandLength)
{
  int n_to_send = commandLength;
  int n_sent = 0;
  int n;
  while (n_to_send > 0) {
    if ((n = write(id, (char *)command+n_sent, n_to_send)) < 0) {
      fprintf(stderr, "write %d, %s\n", id, strerror(errno));
      return -1;
    }   
    n_to_send -= n;
    n_sent += n;

    // printf("write %g \%\n", (double)n_sent / (double)len);
  }
  return commandLength;
}

/*
int receive_data(int id, void *response, int responseLength, bool special_flag, char special_start) {
  int n_to_read = responseLength;
  int n_read = 0;
  int n;

  char inchar;
  while (n_read < responseLength) {
    n = read(id, &inchar, 1);
    if (special_flag && n_read == 0 && inchar != special_start)
      continue;
    ((char *)response)[n_read++] = inchar;
  }

  return n_read;
}
*/

int receive_data(int id, void *response, int responseLength, bool special_flag, char special_start) {

  int n, bytesRead, attempts;
  char inchar;

  struct timeval timeout;
  fd_set readfs;    // file descriptor set
  //    struct termios portOptions;
  int maxPorts;
  int portCount;
  int TIMEOUT = 50000;      // time to wait for port to respond, in microseconds
  int MAXATTEMPTS = 200;    // maximum number of attempts to read characters
  int WAITCHARTIME = 1000;  // time to wait for a char to arrive.

  // select will wait for port to respond or timeout
  maxPorts = id+1;
  timeout.tv_usec = TIMEOUT;  // microseconds
  timeout.tv_sec  = 0;        // seconds
  FD_ZERO(&readfs);
  FD_SET(id, &readfs);  // set testing for id
  if (SERIAL_DEBUG) printf("waiting for port to respond\n");
  portCount = select(maxPorts, &readfs, NULL, NULL, &timeout);  // block until input becomes available
  if ((portCount==0) || (!FD_ISSET(id, &readfs))) {
    if (SERIAL_DEBUG) printf(" - timeout expired!\n");
    return -1;
  }
  if (SERIAL_DEBUG) printf("time remaining %d ms.\n", (int)(timeout.tv_usec/1000));

  // Read data into the response buffer.
   // until we get enough data or exceed the maximum
   //number of attempts
   //
  bytesRead = 0;
  attempts = 0;
  while (bytesRead < responseLength && attempts++ < MAXATTEMPTS) {
    n = read(id, &inchar, 1);
    if (n == 1) {
      if (SERIAL_DEBUG) printf(".%d %u %2x %c", n, inchar, inchar, inchar);
      
      if (special_flag && bytesRead == 0 && inchar != special_start)
        continue;

      ((char *)response)[bytesRead++] = inchar;
    }
    else
      usleep(WAITCHARTIME);  // sleep a while for next byte.
  }
  if (SERIAL_DEBUG) printf("\nattempts %d", attempts);
  if (SERIAL_DEBUG) printf("\nreceiveData: bytes read: %d   expected: %d\n", bytesRead, responseLength);

  if (bytesRead != responseLength) {
    if (SERIAL_DEBUG) printf("mismatch leng\n");
    return -1;
  }
  else
    return responseLength;
}
