#ifndef LOG_H_DEFINED
#define LOG_H_DEFINED

#include <iostream>
#include <fstream>
#include <string>

class Log
{
public:
	// Logs msg with prefix "Error: ".
	static void error(std::string msg);

	// Logs msg with prefix "Warning: ".
	static void warning(std::string msg);

	// Logs msg.
	static void log(std::string msg);

	// Logs msg without end of line.
	static void log_raw(std::string msg);

	// Logs msg with prefix "Debug: ".
	static void debug(std::string msg);

	static void verbose(std::string msg);

	// Turns debug on/off.
	static void debugMode(bool option);

	// Turns verbose on/off.
	static void verboseMode(bool option);

	// Enables/disables terminal escape codes at _stdout_ output.
	static void colors(bool option);

	static void clearLine();

private:
	static bool _isDebugMode;
	static bool _isVerboseMode;
};

#endif /* LOG_H_DEFINED */
