#if NUM_THREADS > 1
 #include <pthread.h>
 #ifdef __APPLE__
 #include "mac_pthread.h"
 #endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "simulazione.h"
#include "hex.h"


struct thread_data thread_data_array[NUM_THREADS];

static void usage(int argc, char *argv[]) {
  printf ("Usage: %s { start | resume } N outstring inconf.gz inlpl inloc "
	  "big_sigma beta_uniform beta_localized conf_volume xlink_conc\n"
	  "\n"
	  "Suggested parameters: start 192 ciao RAND NULL NULL 0.05 0 0 1.0 1e-2\n"
	  "\n",
	  argv[0]);
  exit (-1);
}

static int error_arguments(int argc, char *argv[]) {
  return (argc < 2) || !((!strcmp(argv[1], "start") && (argc == 12)) ||
			 (!strcmp(argv[1], "resume") && (argc == 12)));
}

void sigusr1_handler(int sig) {
  fprintf(stderr,
	  "TOTAL STEPS: %llu\n"
	  "STEPS DONE:  %llu\n"
	  "STEPS TO GO: %llu\n", 
	  mc_time.DYN_STEPS, mc_time.DYN_STEPS - mc_time.t, mc_time.t);
  flushfiles();
}

void sigusr2_handler(int sig) {
  rewind(simufiles.chkfile);
  ftruncate(fileno(simufiles.chkfile), 0);
  print_buffer(simufiles.chkfile);
  flushfiles();
}

void sigterm_handler(int sig) {
  fputs("Terminated\n", stderr);
  fflush(stderr);
  fflush(stdout);
  closefiles();
  exit(143);
}

static void install_sighandlers() {
  struct sigaction sausr1;
  sausr1.sa_handler = sigusr1_handler;
  sausr1.sa_flags = 0; // or SA_RESTART
  sigemptyset(&sausr1.sa_mask);
  if (sigaction(SIGUSR1, &sausr1, NULL) == -1) {
    perror("couldn't set USR1 signal");
  }
  struct sigaction sausr2;
  sausr2.sa_handler = sigusr2_handler;
  sausr2.sa_flags = 0; // or SA_RESTART
  sigemptyset(&sausr2.sa_mask);
  if (sigaction(SIGUSR2, &sausr2, NULL) == -1) {
    perror("couldn't set USR2 signal");
  }
  // install the sigterm and sigint signal
  struct sigaction saterm;
  saterm.sa_handler = sigterm_handler;
  saterm.sa_flags = 0; // or SA_RESTART
  sigemptyset(&saterm.sa_mask);
  if (sigaction(SIGTERM, &saterm, NULL) == -1) {
    perror("couldn't set TERM signal");
  }
  struct sigaction saint;
  saint.sa_handler = sigterm_handler;
  saint.sa_flags = 0; // or SA_RESTART
  sigemptyset(&saint.sa_mask);
  if (sigaction(SIGINT, &saint, NULL) == -1) {
    perror("couldn't set INT signal");
  }
}

int main (int argc, char **argv) {

  if (error_arguments(argc, argv))
    usage(argc, argv);

  // install the sigusr signal
  install_sighandlers();

  // create simulation thread
  pthread_t thread[NUM_THREADS];
  pthread_attr_t attr;
  int rc;
  long t;
  void *status;

  /* Initialize and set thread detached attribute */
#if NUM_THREADS > 1
  pthread_spin_init(&spinsum, 0);
  pthread_barrier_init(&startbarr, NULL, NUM_THREADS);
  pthread_barrier_init(&firstbarr, NULL, NUM_THREADS);
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
#endif


  for(t=0; t<NUM_THREADS; t++) {
    thread_data_array[t].thread_id = t;
    thread_data_array[t].argc = argc;
    thread_data_array[t].argv = argv;
    if(t == 0) { // simulation thread
#if NUM_THREADS > 1
      rc = pthread_create(&thread[t], &attr, simulazione,
			  (void *) &thread_data_array[t]);
#else
      simulazione((void *)&thread_data_array[t]);
      rc = 0;
#endif
    } else if(t == 1) { // input thread
#if NUM_THREADS > 1
      void *inputmain(void *threadarg);
      rc = pthread_create(&thread[t], &attr, inputmain,
			  (void *) &thread_data_array[t]);
#endif
    } else if(t == 2) { // glut thread
#if NUM_THREADS > 2
      void *glumain(void *threadarg);
      rc = pthread_create(&thread[t], &attr, glumain,
			  (void *) &thread_data_array[t]);
#endif
    } /*  else { */
    /*   rc = pthread_create(&thread[t], &attr, interactThread, */
    /* 			  (void *) &thread_data_array[t]); */
    /* } */

    if (rc) {
      fprintf(stderr, "ERROR; return code from pthread_create()"
	      " is %d\n", rc);
      exit(-1);
    }
  }



  /* Free attribute and wait for the other threads */
#if NUM_THREADS > 1
  pthread_attr_destroy(&attr);
  for(t=0; t<NUM_THREADS; t++) {
    rc = pthread_join(thread[t], &status);
    if (rc) {
      fprintf(stderr, "ERROR; return code from pthread_join()"
	      " is %d\n", rc);
      exit(-1);
         }
      }

  pthread_spin_destroy(&spinsum);
  pthread_exit(NULL);
#endif

  return 0;

}

