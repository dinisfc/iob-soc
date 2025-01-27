#include "system.h"
#include "periphs.h"
#include "iob-uart.h"
#include "iob-gpio.h"
#include "printf.h"

char *send_string = "Sending this string as a file to console.\n"
                    "The file is then requested back from console.\n"
                    "The sent file is compared to the received file to confirm " 
                    "correct file transfer via UART using console.\n"
                    "Generating the file in the firmware creates an uniform "
                    "file transfer between pc-emul, simulation and fpga without"
                    " adding extra targets for file generation.\n";

int fib[] = {0,1}; 
int aux = 0;
int l = 30;

// copy src to dst
// return number of copied chars (excluding '\0')
int string_copy(char *dst, char *src) {
    if (dst == NULL || src == NULL) {
        return -1;
    }
    int cnt = 0;
    while(src[cnt] != 0){
        dst[cnt] = src[cnt];
        cnt++;
    }
    dst[cnt] = '\0';
    return cnt;
}

// 0: same string
// otherwise: different
int compare_str(char *str1, char *str2, int str_size) {
    int c = 0;
    while(c < str_size) {
        if (str1[c] != str2[c]){
            return str1[c] - str2[c];
        }
        c++;
    }
    return 0;
}

int main()
{
  //init uart
  uart_init(UART_BASE,FREQ/BAUD);

  //init gpio
  gpio_init(GPIO_BASE);
  
  //test puts
  uart_puts("\n\n\nHello world!\n\n\n");

  l = gpio_get();

  //print fibonacci
  printf("Fibonacci Sequence (%d first values)\n--START--\n%d\t%d\t", l,fib[0],fib[1]);
  
  for(int n = 3; n<l+1; n++){
    aux = fib[1];
    fib[1] = fib[1]+fib[0];
    fib[0] = aux;
    aux = 0;
    
    if((n-1)%10 == 0) //new line every 10 values
      printf("\n");   
    printf("%d\t", fib[1]); 
  }
  gpio_set(fib[1]);
  printf("\n--END--\n\n");
    
  /**
  //test file send
  char *sendfile = malloc(1000);
  int send_file_size = 0;
  send_file_size = string_copy(sendfile, send_string);
  uart_sendfile("Sendfile.txt", send_file_size, sendfile);

  //test file receive
  char *recvfile = malloc(10000);
  int file_size = 0;
  file_size = uart_recvfile("Sendfile.txt", recvfile);

  //compare files
  if (compare_str(sendfile, recvfile, send_file_size)) {
      printf("FAILURE: Send and received file differ!\n");
  } else {
      printf("SUCCESS: Send and received file match!\n");
  }

  free(sendfile);
  free(recvfile);
  **/
  uart_finish();
}
