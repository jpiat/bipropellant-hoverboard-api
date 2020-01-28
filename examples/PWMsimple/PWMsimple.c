#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>


typedef struct MsgToHoverboard_t{
  unsigned char SOM;  // Start of Message
  unsigned char CI;   // continuity counter
  unsigned char len;  // len is len of bytes to follow, NOT including CS
  unsigned char cmd;  // read or write
  unsigned char code; // code of value to write
  int32_t pwm1;           // absolute value ranging from -1000 to 1000 .. Duty Cycle *10 for first wheel
  int32_t pwm2;           // absolute value ranging from -1000 to 1000 .. Duty Cycle *10 for second wheel
  unsigned char CS;   // checksumm
} MsgToHoverboard_t;

typedef union UART_Packet_t{
  MsgToHoverboard_t msgToHover;
  unsigned char UART_Packet[sizeof(MsgToHoverboard_t)];
} UART_Packet_t;

char hoverboardCI = 0;  // Global variable which tracks CI

void setHoverboardPWM(int fd,  int32_t pwm1, int32_t pwm2 )
{
  UART_Packet_t ups;

  ups.msgToHover.SOM = 4 ;    // Start of Message, 4 for No ACKs;
  ups.msgToHover.CI = ++hoverboardCI; // Message Continuity Indicator. Subsequent Messages with the same CI are discarded, need to be incremented.
  ups.msgToHover.len = 1 + 1 + 4 + 4 ; // cmd(1), code(1), pwm1(4) and pwm2(4)
  ups.msgToHover.cmd  = 'r';  // Pretend to send answer to read request. This way HB will not reply. Change to 'W' to get confirmation from board
  ups.msgToHover.code = 0x0E; // "simpler PWM"
  ups.msgToHover.pwm1 = pwm1;
  ups.msgToHover.pwm2 = pwm2;
  ups.msgToHover.CS = 0;

  for (int i = 0; i < (2 + ups.msgToHover.len); i++){  // Calculate checksum. 2 more for CI and len.
    ups.msgToHover.CS -= ups.UART_Packet[i+1];
  }

  int written = 0 ;
  printf("Sending packet of size %d \n", sizeof(UART_Packet_t));
  while(written < sizeof(UART_Packet_t)){
        int ret = write(fd, &(ups.UART_Packet[written]), (sizeof(UART_Packet_t) - written));
        if(ret > 0){
                written += ret ;
        }
  }
}


int setup() 
{
        char * portname = "/dev/ttyS0" ;
        int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_SYNC);
        if (fd < 0) {
                printf("Error opening %s: %s\n", portname, strerror(errno));
                exit(-1);
        }
        struct termios tty;

        if (tcgetattr(fd, &tty) < 0) {
                printf("Error from tcgetattr: %s\n", strerror(errno));
                exit(-1);
        }

        cfsetospeed(&tty, (speed_t) 115200);
        cfsetispeed(&tty, (speed_t) 115200);

        tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;         /* 8-bit characters */
        tty.c_cflag &= ~PARENB;     /* no parity bit */
        tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
        tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

        /* setup for non-canonical mode */
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
        tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        tty.c_oflag &= ~OPOST;

        /* fetch bytes as they become available */
        tty.c_cc[VMIN] = 0; //Set non blocking
        tty.c_cc[VTIME] = 1;

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
                printf("Error from tcsetattr: %s\n", strerror(errno));
                exit(-1);
        }

        return fd ;


int main(int argc, char ** argv)
{
  int fd = setup();
  int speed1 = 0, speed2 = 0 ;
  if(argc > 2){
        speed1 = atoi(argv[1]);
        speed2 = atoi(argv[2]);
  }else{
        printf("Not enough parameters");
  }
  while(1){
     usleep(5000);
     setHoverboardPWM(fd, speed1, speed2);
  }
}
