#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "default_config.h"
#include "hotkey.h"

#define CONFIG_FILE_NAME L"config.ini"

using boost::property_tree::ptree;
using boost::interprocess::shared_memory_object;
using boost::interprocess::mapped_region;

using namespace std;

extern ifstream in_config_file;
extern ofstream out_config_file;
extern ptree config_pt;
extern ptree def_config_pt;

void init_config();
void close_config();

string read_config_value(string key);
void write_config_value(string key, string value);

void save_config();
HotKey* read_hotkey(string config_key);

#endif