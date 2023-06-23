/*
 * Name: Leonid Shuster
 * Lab Exercise: 5
 * Part: 1
 * Role: Continously reads value from USB and writes to PWM connected to LED
  * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <errno.h>
#include <libusb.h>

#define LED_PIN 1

int main (int argc, char * argv[]){
  // Setup for Pi wiring and PWM
  wiringPiSetup();
  pinMode (LED_PIN, PWM_OUTPUT);

  libusb_device_handle* dev; // Pointer to data structure representing USB device

  u_int8_t ADC_output = 0; // Receives data from USB 
  int rcvd_bytes; // Bytes received

  libusb_init(NULL); // Initialize the LIBUSB library

  // Open the USB device (the Cypress device has
  // Vendor ID = 0x04B4 and Product ID = 0x8051)
  dev = libusb_open_device_with_vid_pid(NULL, 0x04B4, 0x8051);
  
  if (dev == NULL){
    perror("device not found\n");
  }

  // Reset the USB device.
  // This step is not always needed, but clears any residual state from
  // previous invocations of the program.
  if (libusb_reset_device(dev) != 0){
    perror("Device reset failed\n");
  } 

  // Set configuration of USB device
  if (libusb_set_configuration(dev, 1) != 0){
    perror("Set configuration failed\n");
  } 


  // Claim the interface.  This step is needed before any I/Os can be
  // issued to the USB device.
  if (libusb_claim_interface(dev, 0) !=0){
      perror("Cannot claim interface");
    }

  // Continuously read from USB, write to PWM
	for(;;) {
    // Perform the IN transfer (from device to host).
    // This will read the data back from the device.
  
		libusb_bulk_transfer(dev, // Handle for the USB device
				    (0x01 | 0x80), // Address of the Endpoint in USB device
				    // MS bit nust be 1 for IN transfers
				    &ADC_output, // address of receive data buffer
				    1, // Size of data buffer
				    &rcvd_bytes, // Number of bytes actually received
				    0 // Timeout in milliseconds (0 to disable timeout)
				    );
            
		printf("%d \n", ADC_output*1023/255 + 1);
		pwmWrite (LED_PIN, ADC_output*1023/255 + 1); // Scale for range of PWM add 1 so LED can turn off
	} 

  libusb_close(dev);

}




