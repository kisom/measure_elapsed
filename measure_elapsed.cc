#include <err.h>
#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;


const auto out_file_default = "elapsed.txt";


static inline void
_write_measurement(string out_file, uint64_t n)
{
	ofstream	out(out_file, ios::out);
	if (!out) {
		cerr << "Failed to open output file!";
		exit(1);
	}

	out << n << endl;
	out.close();
}

static void
write_measurement(string out_file, uint64_t n)
{
	if (n > 0) {
		_write_measurement(out_file+".prev", n-1);
	}

	_write_measurement(out_file, n);
}


static void
usage(const char *argv0, ostream& out)
{
	out << "Usage: " << argv0 << " [-d] [-f output file] [-h]"  << endl;
	out << "\tEvery second, this program will write the elapsed time to" << endl;
	out << "\tthe output file, or " << out_file_default << ".\n";
	out << "\n\tFlags:\n";
#ifndef OSX
	out << "\t\t-d\t\tRun in the background (daemon mode).\n";
#endif
	out << "\t\t-f file\t\tSpecify an output file.\n";
	out << "\t\t-h\t\tPrint this help message.\n";
	out << "\t\t-v\t\tPrint a message every tick.\n";
}


int
main(int argc, char *argv[])
{
	string		out = out_file_default;
	uint64_t	n = 0;
	int		ch;
	bool		quiet = true;
#ifndef OSX
	bool		foreground = true;
#endif

	while (-1 != (ch = getopt(argc, argv, "df:hv"))) {
		switch (ch) {
		case 'd':
#ifndef OSX			
			foreground = false;
#else
			cerr << "Daemonising not supported on OSX.\n";
			cerr << "\t(daemon(3) was marked as deprecated in OS X 10.5.)\n";
#endif
			break;
		case 'f':
			out = string(optarg);
			break;
		case 'h':
			usage(argv[0], cout);
			exit(EXIT_SUCCESS);
		case 'v':
			quiet = false;
			break;			
		default:
			usage(argv[0], cerr);
			exit(EXIT_FAILURE);
		}
	}

	if ((argc - optind) > 0) {
		usage(argv[0], cerr);
		exit(EXIT_FAILURE);
	}

#ifndef OSX	
	if (!foreground && !quiet) {
		cerr << "Warning: verbose mode and daemon mode don't make sense together!\n";
	}

	if (!foreground) {
		if (-1 == daemon(0, 0)) {
			cerr << "Failed to daemonise, aborting.\n";
			abort();
		}
	}
#endif

	while (1) {
		if (!quiet) {
			cout << "Tick...\n";
		}
		write_measurement(out, n);
		n++;
		sleep(1);
	}
}
