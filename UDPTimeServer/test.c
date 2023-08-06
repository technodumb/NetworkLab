#include<stdio.h>
#include<time.h>

void main(){
    time_t cur_time = time(NULL);
    // ctime(&cur_time);
    printf("Current Time: %s", ctime(&cur_time));

}