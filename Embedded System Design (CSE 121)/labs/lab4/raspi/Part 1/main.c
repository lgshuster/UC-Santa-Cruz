#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>

#define BAUDRATE B115200 // UART speed
#define LED_PIN 1

int main (int argc, char * argv[]) {
  wiringPiSetup();
  pinMode (LED_PIN, PWM_OUTPUT);

  struct termios serial; // Structure to contain UART parameters

  char* dev_id = "/dev/serial0"; // UART device identifier
  //char* str = "Hello World"; // Data to be transmitted
  //char rxbuffer[100]; // Receive data buffer
  u_int8_t rxbuffer;

  printf("Opening %s\n", dev_id);
  int fd = open(dev_id, O_RDWR | O_NOCTTY | O_NDELAY);
  // O_RDWR indicates the device needs to both written and read.
  // O_NOCTTY states that the device is not used as a console.
  // O_NDELAY indicates that read and writes should be nonblocking.

  if (fd == -1){ // Open failed
    perror(dev_id);
    return -1;
  }
  

  // Get UART configuration
  if (tcgetattr(fd, &serial) < 0){
    perror("Getting configuration");
    return -1;
  }
  

  // Set UART parameters in the termios structure
  serial.c_iflag = 0;
  serial.c_oflag = 0;
  serial.c_lflag = 0;
  serial.c_cflag = BAUDRATE | CS8 | CREAD | PARENB | PARODD;
  // Speed setting + 8-bit data + Enable RX + Enable Parity + Odd Parity
  
  
  serial.c_cc[VMIN] = 0; // 0 for Nonblocking mode
  serial.c_cc[VTIME] = 0; // 0 for Nonblocking mode

  for (;;) {
  	int read_bytes = read(fd, &rxbuffer, sizeof(rxbuffer));
  	if (read_bytes < 0){
      perror("Read");
      return -1;
    }
    printf("%d\n", rxbuffer);
    pwmWrite (LED_PIN, rxbuffer*4);
  }

  close(fd);
}
