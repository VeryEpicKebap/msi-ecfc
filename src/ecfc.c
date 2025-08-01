#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <time.h>
#include <sched.h>

#define EC_CMD_PORT  0x66
#define EC_DATA_PORT 0x62
#define EC_FAN_REG   0xEE
#define EC_FAN_VAL   0x54

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

unsigned char ec_read(unsigned char addr) {
    ec_wait_input();
    outb(0x80, EC_CMD_PORT);     
    ec_wait_input();
    outb(addr, EC_DATA_PORT);
    ec_wait_output();
    return inb(EC_DATA_PORT);
}

int main() {
    if (ioperm(EC_CMD_PORT, 1, 1) || ioperm(EC_DATA_PORT, 1, 1)) {
        perror("ioperm");
        return 1;
    }
    struct sched_param sp = {0};
    sched_setscheduler(0, SCHED_IDLE, &sp);
    struct timespec delay = {0, 5 * 1000000};

    while (1) {
        if (ec_read(EC_FAN_REG) != EC_FAN_VAL) {
            ec_write(EC_FAN_REG, EC_FAN_VAL);
        }
        nanosleep(&delay, NULL);
    }

    return 0;
}