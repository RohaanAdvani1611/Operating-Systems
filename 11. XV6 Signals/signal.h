// Define Max number of signals
#define NSIG 32

// Define Signal Handler
typedef void (*sighandler_t)(int);

//https://stackoverflow.com/questions/30702288/what-does-void-int-1-mean
#define SIG_ERR     	((void(*)(int)) -1)  //error return
#define SIG_DFL     	((void(*)(int))  0)  //default signal handler	
#define SIG_IGN 	((void(*)(int))  1)  //ignore signal

// Standard x86 signal numbers as per man page
//https://man7.org/linux/man-pages/man7/signal.7.html

#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGIOT		 6
#define SIGBUS		 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
#define SIGCONT		18
#define SIGSTOP		19
#define SIGTSTP		20
#define SIGTTIN		21
#define SIGTTOU		22
#define SIGURG		23
#define SIGXCPU		24
#define SIGXFSZ		25
#define SIGVTALRM	26
#define SIGPROF		27
#define SIGWINCH	28
#define SIGIO		29
#define SIGPOLL		SIGIO
#define SIGPWR		30
#define SIGSYS		31
#define	SIGUNUSED	31
#define SIGRTMIN	32
