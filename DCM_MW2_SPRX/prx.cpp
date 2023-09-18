#include <wchar.h>
#include <sys/prx.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/ppu_thread.h>
#include <string.h>
#include <cstring>
#include <sys/sys_time.h>
#include <sys/time_util.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/process.h>
#include <sys/memory.h>
#include <sys/timer.h>
#include <sys/return_code.h>
#include <sys/prx.h>
#include <stddef.h>
#include <math.h>
#include <cmath>
#include <stdarg.h>
#include <cellstatus.h>
#include <typeinfo>
#include <vector>
#include <pthread.h>
#include <locale.h>
#include <cell/error.h>
#include <sys/paths.h>
#include <time.h>

//   -----------------------------   //
//		  INLUDED HEADER FILES       //
		#include "Main.h"
//   -----------------------------   //


SYS_MODULE_INFO( DCM_MW2_SPRX, 0, 1, 1);
SYS_MODULE_START( _DCM_MW2_SPRX_prx_entry );

SYS_LIB_DECLARE_WITH_STUB( LIBNAME, SYS_LIB_AUTO_EXPORT, STUBNAME );
SYS_LIB_EXPORT( _DCM_MW2_SPRX_export_function, LIBNAME );

// An exported function is needed to generate the project's PRX stub export library
extern "C" int _DCM_MW2_SPRX_export_function(void)
{
    return CELL_OK;
}

extern "C" int _DCM_MW2_SPRX_prx_entry(void)
{
	create_thread(Menu_Thread, 0x4AA, 0x7000, "Loading: Lemon Patch v0.1 (Main)", MainThread);
	create_thread(Calls_thread, 0x4AB, 0x7000, "Loading: Call Thread", thrd_Calls);
    return SYS_PRX_RESIDENT;
}
