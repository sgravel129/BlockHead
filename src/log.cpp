#include "log.hpp"

using namespace std;

bool Log::_isDebugMode = true;
bool Log::_isVerboseMode = true;
bool Log::_showDestructors = true;

#if defined(_WIN32)
	string bold_red = 		"";
	string red = 			"";
	string bold_cyan = 		"";
	string cyan = 			"";
	string bold_green = 	"";
	string green = 			"";
	string bold_yellow = 	"";
	string yellow = 		"";
	string reset_color = 	"";
	string clear_screen = 	"";
	string clear_line = 	"";
#else
	string bold_red = 		"\e[1;31m";
	string red = 			"\e[0;31m";
	string bold_cyan = 		"\e[1;36m";
	string cyan = 			"\e[0;36m";
	string bold_green = 	"\e[1;32m";
	string green = 			"\e[0;32m";
	string bold_yellow =	"\e[1;33m";
	string yellow = 		"\e[0;33m";
	string reset_color =	"\e[0m";
	string clear_screen = 	"[H[2J";
	string clear_line = 	"\r\e[0K";
#endif

// Prefixes
string errorPrefix = 	"[  ERROR  ]  ";
string warningPrefix = 	"[ Warning ]  ";
string debugPrefix = 	"[  Debug  ]  ";
string verbosePrefix = 	"[ Verbose ]  ";
string destructPrefix = "[ Destroy ]  ";

void Log::error(string msg)
{
	cerr << bold_red << errorPrefix << msg << reset_color << endl;
}

void Log::warning(string msg)
{
	cerr << bold_yellow << warningPrefix << msg << reset_color << endl;
}
void Log::destruct(string msg)
{
	cerr << yellow << destructPrefix << msg << reset_color << endl;
}
void Log::log(string msg)
{
	cout << bold_green << msg << reset_color << endl;
}
void Log::log_raw(string msg)
{
	cout << bold_green << msg << reset_color;
}
void Log::debug(string msg)
{
	if (_isDebugMode)
		cerr << bold_cyan << debugPrefix << msg << reset_color << endl;
}
void Log::verbose(string msg)
{
	if (_isVerboseMode)
		cerr << cyan << verbosePrefix << msg << reset_color << endl;
}
void Log::debugMode(bool option)
{
	if (option == false)
	{
		_isDebugMode = false;
		return;
	}

	_isDebugMode = true;
	Log::debug(">Debug mode activated");
}
void Log::verboseMode(bool option)
{
	if (option == false)
	{
		_isVerboseMode = false;
		return;
	}

	// Prints a nice logo
	_isVerboseMode = true;
	Log::verbose(">Verbose Mode activated");
}
void Log::clearLine()
{
	cout << clear_line;
}
