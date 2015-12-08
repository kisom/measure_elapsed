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
	out << "\t\t-d\t\tPrint a message every tick.\n";
	out << "\t\t-f file\t\tSpecify an output file.\n";
	out << "\t\t-h\t\tPrint this help message.\n\n";
}


int
main(int argc, char *argv[])
{
	string		out = out_file_default;
	uint64_t	n = 0;
	int		ch;
	bool		quiet = true;

	while (-1 != (ch = getopt(argc, argv, "df:h"))) {
		switch (ch) {
		case 'd':
			quiet = false;
			break;
		case 'f':
			out = string(optarg);
			break;
		case 'h':
			usage(argv[0], cout);
			exit(EXIT_SUCCESS);
		default:
			usage(argv[0], cerr);
			exit(EXIT_FAILURE);
		}
	}

	if ((argc - optind) > 0) {
		usage(argv[0], cerr);
		exit(EXIT_FAILURE);
	}

	while (1) {
		if (!quiet) {
			cout << "Tick...\n";
		}
		write_measurement(out, n);
		n++;
		sleep(1);
	}
}
