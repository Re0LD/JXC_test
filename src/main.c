#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#include "test_log.h"
#include "test_util.h"
#include "test_fifo.h"
#include "test_udp.h"
#include "test_queue.h"

static bool is_running = true;

//----------------------------------------------------------------------------------------------------SIGNAL
void signal_cb(int sig)
{
    if(sig == SIGINT){
        printf("\nGet ctrl c\n");
    }else if(sig == SIGTERM){
        printf("\nGet kill/reboot\n");
    }else{
        printf("\nunknow sig %d\n",sig);
    }

    is_running = false;
    test_jxc_log_destory();
    test_jxc_fifo_destory();
    test_jxc_udp_destory();
    test_jxc_queue_destory();

    printf("test app close\n");
}


//----------------------------------------------------------------------------------------------------MAIN
int main(int argc, char **argv)
{
    printf("this is jxc test\n");

    signal(SIGINT, signal_cb);   // Ctrl+C
    signal(SIGTERM, signal_cb);  // kill 或 reboot

    //----------test jxc_log
    test_jxc_log();
    //----------test jxc_util
    test_jxc_util();
    //----------test jxc_fifo
    test_jxc_fifo();
    //----------test jxc_queue
    test_jxc_queue();
    //----------test jxc_udp
    test_jxc_udp();
    //----------test jxc_tcp
    // test_jxc_tcp();
    //----------test jxc_XXX

    while(is_running)
    {
        sleep(1);
    }
    
    return 0;
}

