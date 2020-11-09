#include "log.hpp"

using namespace std;

bool Log::_isDebugMode = true;
bool Log::_isVerboseMode = true;

string bold_red = "\e[1;31m";
string red = "\e[0;31m";
string bold_cyan = "\e[1;36m";
string cyan = "\e[0;36m";
string bold_green = "\e[1;32m";
string green = "\e[0;32m";
string bold_yellow = "\e[1;33m";
string yellow = "\e[0;33m";
string reset_color = "\e[0m";
string clear_screen = "[H[2J";
string clear_line = "\r\e[0K";

// Prefixes
string errorPrefix = "[  ERROR  ]  ";
string warningPrefix = "[ Warning ]  ";
string debugPrefix = "[  Debug  ]  ";
string verbosePrefix = "[ Verbose ]  ";

void Log::error(string msg)
{
	cerr << bold_red << errorPrefix << msg << reset_color << endl;
}

void Log::warning(string msg)
{
	cerr << bold_yellow << warningPrefix << msg << reset_color << endl;
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
void Log::colors(bool option)
{
	if (option)
	{
		bold_red = "\e[1;31m";
		red = "\e[0;31m";
		bold_cyan = "\e[1;36m";
		cyan = "\e[0;36m";
		bold_green = "\e[1;32m";
		green = "\e[0;32m";
		bold_yellow = "\e[1;33m";
		yellow = "\e[0;33m";
		reset_color = "\e[0m";
		clear_screen = "[H[2J";
		clear_line = "\r\e[0K";
	}
	else
	{
		bold_red = "";
		red = "";
		bold_cyan = "";
		cyan = "";
		bold_green = "";
		green = "";
		bold_yellow = "";
		yellow = "";
		reset_color = "";
		clear_screen = "";
		clear_line = "";
	}
}
