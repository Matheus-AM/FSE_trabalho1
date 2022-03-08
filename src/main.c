#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bme280.h"
#include "display.h"
#include "gpio.h"
#include "pid.h"
#include "thermometer.h"
#include "uart.h"

#define true 1
#define false 0

int use_terminal = false;
struct bme280_dev bme_connection;
int uart_filesystem, key_gpio = 1;

void pid_routine() {
  system("clear");
  float TI, TR, TE;
  int seconds = 0;
  int value_to_send = 0;
  printf("rotina PID\n");
  pid_setup_constants(5, 1, 5);
  do {
    write_uart_get(uart_filesystem, GET_INTERNAL_TEMP);
    TI = read_uart(uart_filesystem, GET_INTERNAL_TEMP).float_value;

    double value_to_send = pid_control(TI);

    pwm_control(value_to_send);

    write_uart_get(uart_filesystem, GET_POTENTIOMETER);
    TR = read_uart(uart_filesystem, GET_POTENTIOMETER).float_value;
    seconds++;

    pid_update_reference(TR);

    TE = get_temperature(&bme_connection);
    printf("\tseconds: %d TI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", seconds, TI, TR, TE);
    print_display(TI, TR, TE);

    write_uart_send(uart_filesystem, value_to_send, SEND_CONTROL_SIGNAL);
    seconds++;
  } while (1);
}

void init() {
  wiringPiSetup();
  turn_resistance_off();
  turn_fan_off();
  connect_display();
  bme_connection = connect_bme();
  uart_filesystem = connect_uart();
  system("clear");
}

void shutdown_program() {
  system("clear");
  printf("Programa encerrado\n");
  turn_resistance_off();
  turn_fan_off();
  close_uart(uart_filesystem);
  exit(0);
}

int main() {
  init();
  signal(SIGINT, shutdown_program);
  printf("\nTrabalho 1 de FSE\n"
          "Matheus Amaral Moreira - 170080703\n"
          );

  pid_routine();
  return 0;
}
