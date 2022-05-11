#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#include "signal.h"

void kill_signal_test()
{
  int i, pid;
  printf(1, "Testing kill_signal now\n");
  
  pid = fork();
  if(pid == 0) {
    sleep(200);
    i = 0;
    while (1)
      i++;
  } 
  else {
    printf (1, "SIGSTOP testing now\n");
    printf (1, "SIGCONT testing now\n");
    for(i = 0; i < 2; i++){
      sleep(200);
      kill_signal(pid, SIGSTOP);
      sleep(200);
      kill_signal(pid, SIGCONT);
    }
    sleep(200);
    printf (1, "SIGSTOP testing ok\n");
    printf (1, "SIGCONT testing ok\n");
    printf (1, "SIGTERM testing now\n");
    kill_signal(pid, SIGTERM);
    printf (1, "SIGTERM testing ok\n");
    wait();
  }
  
  printf(1, "test 2 now\n");

  pid = fork();
  if(pid == 0) {
    sleep(200);
    i = 0;
    while (1) {
        i++;
    }
  } 
  else
  {
    for(i = 0; i < 20; i++){
      sleep(200);
      kill_signal(pid, SIGSTOP);
      sleep(200);
      kill_signal(pid, SIGCONT);
    }
    sleep(200);
    kill_signal(pid, SIGTERM);
    wait();
  }
  printf(1, "test 2 ok\n");

  printf(1, "testing kill_signal ok\n");
}

void sig_pause(){
  int ret, pid;
  ret = 0;
  pid = fork();

  if(pid == 0) {
    printf(1, "testing pause now\n");
    ret = pause();
    if (-1 == ret)
      printf(1, "testing pause - ok\n");
    } 
  else
  {
    sleep(200);
    kill_signal(pid, SIGTERM);
    wait();
  }
}

void action_handler(int signo){
    printf(1, "SIGNAL %d HANDLED\n", signo);
    return;
}

void sigactiontest(){
    struct sigaction sigact = {action_handler, 0};
    int pid = fork();
    sleep(400);
    printf(1, "Sigaction test starts now\n");
    if (pid == 0){
        sleep(400);
        //printf(1, "childs here\n");
        sigaction(1, &sigact, null);
        sleep(400);
        sigaction(2, &sigact, null);
        sleep(400);
        sigaction(3, &sigact, null);
        sleep(400);
        /*for (int i = 0; i < 1000000; i++){
            if (i % 1000 == 0){
                printf(1, "hi\n");
            }
            printf(1, "");
        }*/
    }
    else{
        sleep(400);
        //printf(1, "pid of child: %d\n", pid);
        sleep(400);
        //printf(1, "signal %d sent to child\n", 1);
        kill_signal(pid, 1);
        sleep(400);
        //printf(1, "signal %d sent to child\n", 2);
        kill_signal(pid, 2);
        sleep(400);
        //printf(1, "signal %d sent to child\n", 3);
        kill_signal(pid, 3);
        sleep(400);
        kill_signal(pid, 9);

        wait();
        sleep(300);
        printf(1, "Sigaction test ok\n");
    }
}   

int
main(int argc, char *argv[])
{
  printf(1, "sigtests starting\n");

  if(open("sigtests.ran", 0) >= 0){
    printf(1, "already ran user tests -- rebuild fs.img\n");
    exit();
  }
  close(open("sigtests.ran", O_CREATE));

  kill_signal_test();
  sig_pause(); 
  sigactiontest();
  exit();
}
