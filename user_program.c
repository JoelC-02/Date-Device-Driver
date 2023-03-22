#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];

int main() {
        int fd = open("/dev/date",O_RDWR);
        char buf[100];
        read(fd, buf, sizeof(100));
        printf("Date: %s\n", buf);
        memset(buf, 0, 256);
	printf("Set time: ");
        scanf("%s",buf);
        write(fd,buf,strlen(buf));
        close(fd);
        return 0;
}
