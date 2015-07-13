#ifndef MAIN_APP_CPP
#define MAIN_APP_CPP

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "../gw2dps/config.h"
#include "../gw2dps/keymap.h"
#include "../gw2dps/hotkey.h"

#include "MainApp.h"
#include "MainFrame.h"
#include "MainPane.h"

using boost::interprocess::shared_memory_object;
using boost::interprocess::mapped_region;

bool MainApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;
	
	// write current directory in shared memory
	int cur_path_len = GetCurrentDirectory(0, NULL);
	shared_memory_object shm(boost::interprocess::open_or_create, "config_path", boost::interprocess::read_write);
	shm.truncate(cur_path_len + 1);
	mapped_region region(shm, boost::interprocess::read_write);
	wchar_t* shm_pointer = (wchar_t*) region.get_address();
	GetCurrentDirectory(cur_path_len, shm_pointer);
	shm_pointer[cur_path_len] = '\0';
	
	init_config();
	init_keymap();
	
    MainFrame *frame = new MainFrame("GW2DPS");
	frame->Show(true);
    return true;
}

#endif