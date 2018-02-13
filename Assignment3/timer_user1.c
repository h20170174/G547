#include "timer.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>              /* open */
#include <unistd.h>             /* exit */
#include <sys/ioctl.h>          /* ioctl */

/*
 * Functions for the ioctl calls
 */

int ioctl_delay(int file_desc, int sleep_time)
{
    int ret_val;
    int t_sleep;
    t_sleep = sleep_time; 

    ret_val = ioctl(file_desc, IOCTL_DELAY, &t_sleep);     //Calling ioctl from user to kernel space

    if (ret_val < 0) 
	{
        printf("ioctl call failed:%d\n", ret_val);
        exit(-1);
    }

	else
	   {
		printf("You have given sleep time as : %d secs\n\n", (sleep_time/1000));
		 }
    return 0;
}






int main(int argc, char *argv[])
{
	int file_desc;
	int temp,k;
	int ret_val;
	int current_count;
	long conv;
	char *p;
	
////////////////////////////////////////////////Open////////////////////////////////////
	file_desc = open(DEVICE_FILE_NAME, 0);
	
       if (file_desc < 0) 
		{
        printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        exit(-1);
		}
 	
	  else	{
		printf("Opening Device File: %s\n", DEVICE_FILE_NAME);
			
		}

    	
			
////////////////////////////////////////////////////Ioctl///////////////////////////////			
			conv = strtol(argv[1], &p, 10);  //To covert the character value of argv into integer
			temp = conv;
			ioctl_delay(file_desc, temp);

/////////////////////////////////////////////////Read//////////////////////////////////
			
	ret_val = read(file_desc,&current_count,1);
	if (ret_val < 0) 
		{
        	printf("Can't read device file: %d\n", ret_val);
        	exit(-1);
    	}
	else{
        	printf("Reading from device file: %s\n", DEVICE_FILE_NAME);
		    printf("Curent count value is :  %d\n" ,current_count);
		}
			
////////////////////////////////////////////////////Close//////////////////////////////
	close(file_desc);
    return 0;
}
