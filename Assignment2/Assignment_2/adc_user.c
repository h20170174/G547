#include "adc.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>              /* open */
#include <unistd.h>             /* exit */
#include <sys/ioctl.h>          /* ioctl */

/*
 * Functions for the ioctl calls
 */

int ioctl_set_channel(int file_desc, int channel)
{
    int ret_val;
    adc_arg_t q;
    q.channel = channel; 

    ret_val = ioctl(file_desc, IOCTL_SET_CHANNEL, &q);

    if (ret_val < 0) 
	{
        printf("ioctl_set_channel failed:%d\n", ret_val);
        exit(-1);
    }

	else
	{
		if(q.channel >= 0 && q.channel < 8)
		 {
		printf("You have selected channel no : %d\n", q.channel);
		printf("Output: %d\n", q.value);
		 }
		else
		printf("Invalid Channel number; it should be between 0 to 7\n");
	}
    return 0;
}






int main(int argc, char *argv[])
{
	int file_desc;
	int temp;
	long conv;
	char *p;

	file_desc = open(DEVICE_FILE_NAME, O_RDWR);
        if (file_desc < 0) 
	{
        printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        exit(-1);
	}
 	
	conv = strtol(argv[1], &p, 10);  //To covert the character value of argv into integer
	temp = conv;
	ioctl_set_channel(file_desc, temp);

	close(file_desc);
    return 0;
}
        

