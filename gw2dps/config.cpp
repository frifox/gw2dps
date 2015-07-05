#ifndef CONFIG_CPP
#define CONFIG_CPP

#include "config.h"

ifstream in_config_file;
ofstream out_config_file;
ptree config_pt;

void init_config()
{
	// read gw2dps directory from shared memory
	shared_memory_object shm(boost::interprocess::open_only, "config_path", boost::interprocess::read_only);
	mapped_region region(shm, boost::interprocess::read_only);
	wchar_t* cur_path = (wchar_t*) region.get_address();
	
	wstring abs_file_name = wstring(cur_path) + L"\\" + CONFIG_FILE_NAME;
	
	in_config_file.open(abs_file_name.c_str(), fstream::in);
	if (!in_config_file.is_open())
	{
		out_config_file.open(abs_file_name.c_str(), fstream::out);
		out_config_file << DEFAULT_CONFIG_FILE;
		out_config_file.flush();
		out_config_file.close();
	}
	// TODO: rewind is better than reopen
	in_config_file.close();
	in_config_file.open(abs_file_name.c_str(), fstream::in);
	read_ini(in_config_file, config_pt);
}

void close_config()
{
	in_config_file.close();
}

string read_config_value(string key)
{
	try
	{
		return config_pt.get<string>(key);
	}
	catch (std::exception e)
	{
		return "";
	}
}

HotKey* read_hotkey(string config_key)
{
	string config_value = read_config_value(config_key);
	if (config_value.compare("") != 0)
	{
		return new HotKey(config_value);
	}
	else
	{
		return NULL;
	}
}

#endif