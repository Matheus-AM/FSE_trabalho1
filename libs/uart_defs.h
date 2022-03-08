#ifndef UART_DEFS_H
#define UART_DEFS_H

#define GET_INTERNAL_TEMP 0xC1
#define GET_POTENTIOMETER 0xC2
#define GET_KEY_VALUE 0xC3
#define SEND_CONTROL_SIGNAL 0xD1
#define SEND_REFERENCE_SIGNAL 0xD2
#define SEND_SYSTEM_STATE 0xD3
#define SEND_CONTROL_MODE 0xD4

typedef struct Number_type {
  int int_value;
  float float_value;
} Number_type;

#endif /* UART_DEFS_H */
