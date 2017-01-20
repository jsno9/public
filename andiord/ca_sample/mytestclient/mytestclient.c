#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/eventfd.h>
#include <errno.h>
#include <linux/msm_ion.h>
#include <utils/Log.h>
#include "QSEEComAPI.h"
#include <SecureUILib.h>
#include "common_log.h"
#include <sys/mman.h>
#include <getopt.h>
#include "comdef.h"
#include "cp_utils.h"
#include <dlfcn.h>
#include "content_protection_copy.h"

struct qsc_send_cmd {
     uint32_t cmd_id;
     uint32_t data;
     uint32_t data2;
     uint32_t len;
     uint32_t start_pkt;
     uint32_t end_pkt;
     uint32_t test_buf_size;
};

struct qsc_send_cmd_rsp {
  uint32_t data;
  int32_t status;
};

int main()
{
	struct QSEECom_handle *l_QSEEComHandle = NULL;
	int ret;
	struct qsc_send_cmd *msgreq;	/* request data sent to QSEE */
	struct qsc_send_cmd_rsp *msgrsp;	/* response data sent from QSEE */
	void *send_buf;
	int32_t req_len = 0;
	int32_t rsp_len = 0;
	printf("%s: main start\n",__func__);
	msgreq=(struct qsc_send_cmd *)malloc(sizeof(struct qsc_send_cmd));
	msgreq->cmd_id = 1;
	msgreq->data = 2;
	msgreq->data2 = 3;
	send_buf = (void *)msgreq;
	req_len = sizeof(struct qsc_send_cmd);
	rsp_len = sizeof(struct qsc_send_cmd_rsp);
	//LOGD("send cmd to 32bit app, req data = %d", msgreq->data);
	msgrsp=(struct qsc_send_cmd_rsp *)malloc(sizeof(struct qsc_send_cmd_rsp));

	ret = QSEECom_start_app(&l_QSEEComHandle, "/system/etc/firmware","mytesttz", 1024);
	if (ret) {
	   LOGE("Loading app -mytesttz failed\n");
	   printf("%s: Loading app -mytesttz failed\n",__func__);
		return 0;
	} else {
	   LOGD("Loading app -mytesttz succeded\n");
	}
	printf("%s: Loading app -mytesttz ok\n",__func__);
	ret = QSEECom_send_cmd(l_QSEEComHandle,
				send_buf,
				req_len,
				msgrsp,
				rsp_len);
	if (ret) {
	   LOGE("send command failed with ret = %d\n", ret);
	   printf("%s: Send command failed with ret = %d\n",__func__,ret);
	}
	LOGD("rsp data = %d, rsp status = %x\n", msgrsp->data, msgrsp->status);
	printf("rsp data = %d, rsp status = %x\n", msgrsp->data, msgrsp->status);

	ret = QSEECom_shutdown_app(&l_QSEEComHandle);
	if (ret) {
	  LOGE("Shutdown app failed with ret = %d", ret);
	  printf("%s: Shutdown app failed with ret = %d\n",__func__,ret);
	} else {
	  LOGD("shutdown app: pass");
		printf("%s: Shutdown app ok with ret = %d\n",__func__,ret);
	}
	
	return 0;
}
