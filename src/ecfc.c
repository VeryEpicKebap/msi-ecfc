#include <stdio.h>
#include <unistd.h>
#include <sys/io.h> 

#define EC_CMD_PORT  0x66
#define EC_DATA_PORT 0x62

void ec_wait_input() {
    while (inb(EC_CMD_PORT) & 0x02); 
}

void ec_wait_output() {
    while (!(inb(EC_CMD_PORT) & 0x01)); 
}

void ec_write(unsigned char addr, unsigned char value) {
    ec_wait_input();
    outb(0x81, EC_CMD_PORT);   
    ec_wait_input();
    outb(addr, EC_DATA_PORT);  
    ec_wait_input();
    outb(value, EC_DATA_PORT); 
}

int main() {
    if (ioperm(EC_CMD_PORT, 1, 1) || ioperm(EC_DATA_PORT, 1, 1)) {
        perror("ioperm");
        return 1;
    }

    while (1) {
        ec_write(0xEE, 0x54); // T is the highest EC fan value for the MSI CX640. Adjust as needed.
        usleep(2000);       
    }

    return 0;
}