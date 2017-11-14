#if NUM_THREADS > 1
#include <pthread.h>
 #ifdef __APPLE__
 #include "../mac_pthread.h"
 #endif
#endif

// The source code in this file is really bad, in the long term we want to extend
// the configuration file to be accessible from the command line.
// Right now it does it's job

#include <string>
#include <vector>
#include <cstdio>
#include <time.h>
#include <zlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <readline/readline.h>
#include <readline/history.h>
#include "glxwindower.hxx"

extern pthread_spinlock_t spinsum;
extern pthread_barrier_t startbarr;
extern pthread_barrier_t firstbarr;
struct thread_data {
  int thread_id;
  int argc;
  char **argv;
};

struct inputvari_t {
  std::string name;
  std::function<void(const std::string)> modificatore;
  std::function<std::string(void)> desc;
};

extern "C" void print_buffer(FILE *out);
#if (defined(UNIFORM) || defined(XLINK))
extern double beta_uniform;
#endif
#if defined(LOCALIZED)
extern double beta_localized;
#endif
#if defined(XLINK)
extern double xlink_conc;
#endif

extern float BackGround[4];

struct mc_time_t {
  unsigned long long t;
  unsigned long long DYN_STEPS;
  unsigned long long RELAX_TIME;
  unsigned long long CORRL_TIME;
  int STATISTIC;
};
extern mc_time_t mc_time;

struct simufiles_t {
  gzFile *xyzfile;
  FILE *accfile;
  FILE *ctcfile;
  FILE *inffile;
  FILE *lplfile;
  FILE *locfile;
  FILE *rndfile;
  FILE *chkfile;
};
extern simufiles_t simufiles;
extern "C" void flushfiles();

#if defined(FASTEXP)
extern "C" void populate_lookup_tables();
#endif

const std::vector <inputvari_t> inputvari = {
  {"help",
    [] (const std::string cosa) {
      std::cout << "\nYou can type the following commands,\n"
                   "multiple commands can be chained by ; character\n\n";

      for (int i = 0; i < inputvari.size(); i++)
	std::cout << inputvari[i].name << inputvari[i].desc() << std::endl;

      std::cout << std::endl;
      },
    []() -> std::string {
      return "\t\t\tshow this list of possible commands";
    }
  },

  {"exit",
    [] (const std::string cosa) {
    },
    []() -> std::string {
      return "\t\t\tclose the command terminal";
    }
  },

  {"quit",
    [] (const std::string cosa) {
      exit(0);
    },
    []() -> std::string {
      return "\t\t\tquit the simulation";
    }
  },

  {"blink",
    [] (const std::string cosa) {
      float oldbg[4] = { BackGround[0], BackGround[1],
			 BackGround[2], BackGround[3] };

      BackGround[0] = 0.0; BackGround[1] = 0.0; 
      BackGround[2] = 0.0; BackGround[3] = 1.0;

      struct timespec waittime; 
      waittime.tv_sec = 0;
      waittime.tv_nsec = 100000000L;

      pthread_spin_unlock (&spinsum);
      nanosleep(&waittime, &waittime);
      pthread_spin_lock (&spinsum);

      BackGround[0] = oldbg[0]; BackGround[1] = oldbg[1]; 
      BackGround[2] = oldbg[2]; BackGround[3] = oldbg[3];
    },
    []() -> std::string {
      return "\t\t\tblink the graphical window";
    }
  },

  {"dump",
    [] (const std::string cosa) {
      print_buffer(stdout);
    },
    []() -> std::string {
      return "\t\t\tdump the current configuration";
    }
  },

  {"checkpoint",
    [] (const std::string cosa) {
      rewind(simufiles.chkfile);
      ftruncate(fileno(simufiles.chkfile), 0);
      print_buffer(simufiles.chkfile);
      fflush(simufiles.chkfile);
      std::cout << "Done" << std::endl;
    },
    []() -> std::string {
      return "\t\tmake a checkpoint";
    }
  },

  {"times",
    [] (const std::string cosa) {
      fprintf(stderr,
	      "TOTAL STEPS: %llu\n"
	      "RELAX TIME:  %llu\n"
	      "STEPS DONE:  %llu\n"
	      "STEPS TO GO: %llu\n", 
	      mc_time.DYN_STEPS,
	      mc_time.RELAX_TIME,
	      mc_time.DYN_STEPS - mc_time.t,
	      mc_time.t);
    },
    []() -> std::string {
      return "\t\t\tshow the time constants";
    }
  },

  {"flush",
    [] (const std::string cosa) {
      flushfiles();
      std::cout << "Done" << std::endl;
    },
    []() -> std::string {
      return "\t\t\tflush the output files write cache";
    }
  },
#if (defined(UNIFORM) || defined(XLINK))
  {"beta_uniform=",
    [] (const std::string cosa) {
      double temp = boost::lexical_cast<double>(cosa);
      beta_uniform = temp;
#if defined(FASTEXP)
      populate_lookup_tables();
#endif
      std::cout << "beta_uniform=" << beta_uniform << std::endl;
    },
    []() -> std::string {
      std::ostringstream oss;
      oss.setf(std::ostringstream::showpoint);
      oss << std::setprecision(4) << beta_uniform << "\tset the variable";
      return oss.str();
    }
  },
#endif
#if defined(LOCALIZED)
  {"beta_localized=",
    [] (const std::string cosa) {
      double temp = boost::lexical_cast<double>(cosa);
      beta_localized = temp;
#if defined(FASTEXP)
      populate_lookup_tables();
#endif
      std::cout << "beta_localized=" << beta_localized << std::endl;
    },
    []() -> std::string {
      std::ostringstream oss;
      oss.setf(std::ostringstream::showpoint);
      oss << std::setprecision(4) << beta_localized << "\tset the variable";
      return oss.str();
    }
  },
#endif
#if defined(XLINK)
  {"xlink_conc=",
   [] (const std::string cosa) {
      double temp = boost::lexical_cast<double>(cosa);
      xlink_conc = temp;
      std::cout << "xlink_conc=" << xlink_conc << std::endl;
    },
   []() -> std::string {
     std::ostringstream oss;
     oss.setf(std::ostringstream::scientific);
     oss << std::setprecision(2) << xlink_conc << "\tset the variable";
     return oss.str();
   }
  }
#endif
};

static inline std::string readcomando(char *prompt) {
  char *a = readline(prompt);
  if (!a)
    std::cout << std::endl;

  add_history(a);

  std::string retval = (a ? a : "exit");
  free(a);

  return retval;
}

extern "C" void *inputmain(void *threadarg) {
  pthread_barrier_wait(&startbarr);
  pthread_barrier_wait(&firstbarr);
  int argc = ((struct thread_data *) threadarg) -> argc;
  char **argv = ((struct thread_data *) threadarg) -> argv;
  close(open("/tmp/accia_history", O_CREAT | O_EXCL, 0644));
  read_history("/tmp/accia_history");

  std::string comando;

  char *prompt;
  //  if (isatty(fileno(stdin)))
  //    prompt = (char *)"] ";
  //  else
    prompt = (char *)"";

  while (comando != "exit") {

    comando = readcomando(prompt);

    // here start the parsing
    pthread_spin_lock (&spinsum);
    try {
      comando.erase(std::remove_if(comando.begin(), comando.end(),
				   isspace),
		    comando.end());

      std::vector <std::string> subcommands;
      boost::split(subcommands, comando, boost::is_any_of(";"));

      for (int j = 0; j < subcommands.size(); j++)
	for (int i = 0; i < inputvari.size(); i++) {
	  if (boost::starts_with(subcommands[j], inputvari[i].name))
	    inputvari[i].
	      modificatore(subcommands[j].substr(inputvari[i].name.length(),
						 std::string::npos));
	}
    } catch (boost::bad_lexical_cast) {
      std::cout << "Syntax error" << std::endl;
    }
    pthread_spin_unlock (&spinsum);

  }

  write_history("/tmp/accia_history");
  close(open("/tmp/accia_history", O_CREAT | O_EXCL, 0644));
  history_truncate_file("/tmp/accia_history", 1000);
  close(open("/tmp/accia_history", O_CREAT | O_EXCL, 0644));
  return NULL;
}
