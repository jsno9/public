#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <cutils/uevent.h>
#define NL_RX_BUFFER_MAX    (64 * 1024)
void main()
{
	int nl_fd;
	fd_set readfds;
	int fd_cnt;
	int rx_byte;
	char buffer[NL_RX_BUFFER_MAX];
	nl_fd = uevent_open_socket(NL_RX_BUFFER_MAX, 1);
	if( nl_fd == -1 )  {
		printf("error open socket\n");
		return (void *)-1;
	}
	printf("ok open socket\n");
	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(nl_fd, &readfds);

		fd_cnt = select(nl_fd + 1, &readfds, NULL, NULL, NULL);
		printf("get message %d\n",fd_cnt);
		if(fd_cnt>0)
		{
			printf("get message 2\n");
			if( FD_ISSET(nl_fd, &readfds) )  {
				rx_byte = uevent_kernel_multicast_recv(nl_fd, buffer, sizeof(buffer));
			}
		}
	}

	
}


