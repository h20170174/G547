///////////////////////////*********************Readme File for Low Level IO Mapped Access to Timer*************//////////////////////////////


1. In this project a char driver is implemented for accessing timer hardware(PIT 8254)

2. 1 milisecond delay function is been implemented and using that sleep function is also been implemented in kernel space

3. User can also read the current count value of the timer/counter using read function



///////////////////How to Compile and Run?///////////////////////////////////////////////////

1. First make a excutable of the user space code timer_user1.c by compiling ( gcc -o timer_user1 timer_user1.c)

2. Compile the kernel space code using "make" command

3. Insert module into kernel space(sudo insmod timer_module.ko)

4. Go to dev directory and change the device file permission to 777(sudo chmod 777 timer0)

5. Execute the user space code and give the sleep time (in miliseconds) as command line argument(./timer_user1 3000)

6. You can see and verify the sleep time value entered by you in (seconds) and the current counter value will also be shown

7. If you want to verify further whether the sleep function is implemented correctly or not, go to the log file (using dmesg command) and you  	  can see the exact time before entering and after exiting sleep mode.
