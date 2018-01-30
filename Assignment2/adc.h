/*
 *  adc.h - the header file with the ioctl definitions*/


#ifndef ADC_H
#define ADC_H

#include <linux/ioctl.h>

typedef struct   //As we have more than 1 value to deal with
{
	int channel,value;
} adc_arg_t;

#define IOCTL_SET_CHANNEL _IOWR('q', 1, adc_arg_t *)


#define DEVICE_FILE_NAME "/dev/adc0"

#endif
