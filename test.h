void init_agi();
void dump_agi();
int checkresult();
struct agi_env {
		char request[100]; 	// guessing here
		char channel[80]; 	// from definition in channels.h
		char lang[20];		// from definition in channels.h
		char type[100];		// guessing
		char callerid[100];	// guessing
		char dnid[100];		// guessing
		char context[80];	// from definition in channels.h
		char extension[80];	// from definition in channels.h
		int priority;		// from definition in channels.h
	};
