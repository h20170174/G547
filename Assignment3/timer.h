/* timer.h - the header file with the ioctl definitions*/

#include <linux/ioctl.h>
#include <linux/types.h>


#ifndef TIMER_H
#define TIMER_H





#define IOCTL_DELAY _IOW('q', 1, int *)


#define DEVICE_FILE_NAME "/dev/timer0"
#endif
