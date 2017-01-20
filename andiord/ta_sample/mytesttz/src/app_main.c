/*
@file app_main.c
@brief App main entry point.

*/
/*===========================================================================
   Copyright (c) 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE
  $Header: //source/qcom/qct/core/pkg/trustzone/rel/2.0/trustzone_images/core/securemsm/trustzone/qsapps/sampleapp/src/app_main.c#19 $
  $DateTime: 2014/09/11 16:15:52 $
  $Author: pwbldsvc $
cat /d/tzdbg/qsee_log

# when       who     what, where, why
# --------   ---     ---------------------------------------------------------
  08/4/14   pdosi      Added missing fs api tests
===========================================================================*/
#include <stdarg.h>
#include <stdio.h>
#include "qsee_sfs.h"
#include "qsee_spi.h"
#include "qsee_log.h"

typedef struct send_cmd{
  uint32_t cmd_id;
  unsigned int data;
  unsigned int data2;
  uint32_t len;
  uint32_t start_pkt;
  uint32_t end_pkt;
  uint32_t test_buf_size;
} send_cmd_t;

typedef struct send_cmd_rsp{
  uint32_t data;
  int32_t status;
} send_cmd_rsp_t;

void TEE_LogPrintf(int level, const char* format, ...)
{
    va_list args;
	char log_str[1024];

	/* Temporary workaround for not trying to print floats
     * that will crash the MSM9884 platform */
    if (strstr(format, "%f") || strstr(format, "%.") || strstr(format, "%10f")) {
    	qsee_log(QSEE_LOG_MSG_ERROR,"Trying to print floats %s", format);
  	} else {
		va_start(args, format);
		vsprintf(log_str, format, args);
		if(level) {
    		qsee_log(QSEE_LOG_MSG_ERROR, "%s", log_str);
		} else {
			qsee_log(QSEE_LOG_MSG_ERROR, "%s", log_str);
		}
    	va_end(args);
	}
}

void tz_app_init(void)
{
    QSEE_LOG(QSEE_LOG_MSG_ERROR, "mytesttz start");
}

void tz_app_cmd_handler(void* cmd, uint32_t cmdlen, 
                        void* rsp, uint32_t rsplen)
{
	struct send_cmd_rsp *rsp_ptr = (struct send_cmd_rsp *)rsp;
	QSEE_LOG(QSEE_LOG_MSG_ERROR, "tz_app_cmd_handler entered");

	if (cmd == NULL || rsp == NULL || cmdlen == 0 || rsplen == 0){
		QSEE_LOG(QSEE_LOG_MSG_ERROR, "TZ App invalid inparam");
		return;
	}
	rsp_ptr->data=2;
	rsp_ptr->status=3;

}

/**
  @brief 
    App specific shutdown
    App will be given a chance to shutdown gracefully
*/
void tz_app_shutdown(void)
{
	QSEE_LOG(QSEE_LOG_MSG_ERROR, "mytesttz stop");
}

