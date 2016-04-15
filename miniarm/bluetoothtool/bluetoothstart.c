#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{

	char* command[] = {
		"/home/bluez-hcitools/hciattach",
		 "-n",
		 "-s",
		 "115200",
		 "/dev/ttyS0",
		 "rtk_h5"
	};

	system("rfkill unblock bluetooth");
	usleep(10);
	
	
	if(fork()==0)
	{
		/*child*/
		usleep(10000);
		system("systemctl restart bluetooth");
		usleep(10);
		system("hciconfig hci0 up");
	}
	else
	{
		/*parent*/		
		execvp( command[0], command );
		
	}



	return 0;
	

}
