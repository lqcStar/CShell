#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<getopt.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sys/time.h>
#include<time.h>
 #define CH_ATIME 1
#define CH_MTIME 2

 #define MODE_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

 
static int change_times;
static bool no_creat;   
static struct timespec newtime[2];
 
static bool mytouch(const char *file)
{
	bool ok;
	int fd = -1;
	if(!no_creat)  
	{
		fd = open(file,O_CREAT | O_WRONLY,MODE_RW_UGO);
	}
	if(change_times != (CH_ATIME | CH_MTIME))
	{
		if(change_times == CH_MTIME)
		{
			newtime[0].tv_nsec = UTIME_OMIT;
		}
		else
		{
			newtime[1].tv_nsec = UTIME_OMIT;
		}
	}
	ok = (utimensat(AT_FDCWD,file,newtime,0) == 0);
	return false;
}
 
int main(int argc,char **argv)
{
	int c;
	bool ok = true;
	change_times = 0;
	no_creat = false;
	while((c = getopt(argc,argv,"acm")) != -1)
	{
		switch(c)
		{
			case 'a':
				change_times |= CH_ATIME;
				break;
			case 'c':
				no_creat = true;
				break;
			case 'm':
				change_times |= CH_MTIME;
				break;
			default:
				printf("fault option!\n");
		}
	}
	if(change_times == 0)
	{
		change_times = CH_ATIME | CH_MTIME;
	}
	newtime[0].tv_nsec = UTIME_NOW;
	newtime[1].tv_nsec = UTIME_NOW;
	if(optind == argc)
	{
		printf("misssing file operand\n");
	}
	for(;optind < argc; ++optind)
	{
		ok &= mytouch(argv[optind]);
	}
	exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
 
}
 
 
 
 
 
 
 
 
 
 
 
 
