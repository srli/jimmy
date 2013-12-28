#ifndef __ACCUSWAY_SERIAL_H
#define __ACCUSWAY_SERIAL_H

int open_port(const char *);
int set_comm_parameters(int id,
                        int baudrate,
                        int dataBits,
                        int parity,
                        int stopBits );
void close_port(int id);
int send_data(int id, const void *command, int commandLength);
int receive_data(int id, void *response, int response_length, bool special_flag = false, char special_start = 0xFF);

#endif
