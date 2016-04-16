/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2000-2001  Qualcomm Incorporated
 *  Copyright (C) 2002-2003  Maxim Krasnyansky <maxk@qualcomm.com>
 *  Copyright (C) 2002-2010  Marcel Holtmann <marcel@holtmann.org>
 *  Copyright (C) 2013-2014  Realtek Semiconductor Corp.
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <termios.h>
#include <time.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include "lib/bluetooth.h"
#include "lib/hci.h"
#include "lib/hci_lib.h"

#include "hciattach.h"

#define RTK_FRAG_LEN 252

#define OCF_READ_ROM_VERSION		0x006d
typedef struct {
        uint8_t status;
        uint8_t version;
} __attribute__ ((packed)) read_rom_version_rp;
#define READ_ROM_VERSION_RP_SIZE 2

#define OCF_RTK_DOWNLOAD		0x0020
typedef struct {
	uint8_t index;
	uint8_t data[RTK_FRAG_LEN];
} __attribute__ ((packed)) rtk_download_cp;

typedef struct {
        uint8_t status;
        uint8_t index;
} __attribute__ ((packed)) rtk_download_rp;
#define RTK_DOWNLOAD_RP_SIZE 2

#define RTK_ROM_LMP_3499	0x3499
#define RTK_ROM_LMP_8723A	0x1200
#define RTK_ROM_LMP_8723B	0x8723
#define RTK_ROM_LMP_8723B2	0x4ce1
#define RTK_ROM_LMP_8821A	0x8821
#define RTK_ROM_LMP_8761A	0x8761

static const uint8_t rtk_epatch_signature[8] = {
	0x52, 0x65, 0x61, 0x6C, 0x74, 0x65, 0x63, 0x68
};

typedef struct {
	uint8_t signature[8];
	uint32_t fw_version;
	uint16_t num_patches;
} __attribute__ ((packed)) rtk_epatch_header;

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#ifndef FIRMWARE_DIR
#define FIRMWARE_DIR "/lib/firmware/"
#endif

static int rtk_download_firmware(int dd, const unsigned char *fw_data,
		int fw_len)
{
	struct hci_request rq;
	rtk_download_cp cp;
	rtk_download_rp rp;
	int frag_num = fw_len / RTK_FRAG_LEN + 1;
	int frag_len = RTK_FRAG_LEN;
	int i;

	for (i = 0; i < frag_num; i++) {
		cp.index = i;

		if (i == (frag_num - 1)) {
			cp.index |= 0x80;
			frag_len = fw_len % RTK_FRAG_LEN;
		}
		memcpy(cp.data, fw_data, frag_len);

		rq.ogf    = OGF_VENDOR_CMD;
		rq.ocf    = OCF_RTK_DOWNLOAD;
		rq.event  = 0;
		rq.cparam = &cp;
		rq.clen   = frag_len + 1;
		rq.rparam = &rp;
		rq.rlen   = RTK_DOWNLOAD_RP_SIZE;

		if (hci_send_req(dd, &rq, 1000) < 0)
			return -1;

		if (rp.status) {
			errno = EIO;
			return -1;
		}

		fw_data += RTK_FRAG_LEN;
	}

	return 0;
}

static int rtk_parse_firmware(int dd, uint16_t lmp_subver,
		unsigned char **fw_data, int fw_len)
{
	struct hci_request rq;
	read_rom_version_rp rp;
	uint8_t rom_version = 0;
	const uint8_t extension_sig[4] = { 0x51, 0x04, 0xfd, 0x77 };
	rtk_epatch_header *epatch_info;
	unsigned char *buf;
	const unsigned char *fwptr, *chip_id_base;
	const unsigned char *patch_length_base, *patch_offset_base;
	uint8_t opcode, length, data;
	uint32_t patch_offset = 0;
	uint16_t patch_length;
	size_t min_size;
	int project_id = -1;
	int i;
	const uint16_t project_id_to_lmp_subver[] = {
		RTK_ROM_LMP_8723A,
		RTK_ROM_LMP_8723B,
		RTK_ROM_LMP_8723B2,
		RTK_ROM_LMP_8821A,
		RTK_ROM_LMP_8761A,
	};

	rq.ogf    = OGF_VENDOR_CMD;
	rq.ocf    = OCF_READ_ROM_VERSION;
	rq.event  = 0;
	rq.cparam = NULL;
	rq.clen   = 0;
	rq.rparam = &rp;
	rq.rlen   = READ_ROM_VERSION_RP_SIZE;

	if (hci_send_req(dd, &rq, 1000) < 0)
		return -1;

	if (rp.status == 0)
		rom_version = rp.version;

	min_size = sizeof(*epatch_info) + sizeof(extension_sig) + 3;
	if (fw_len < min_size)
		return -1;

	fwptr = *fw_data + fw_len - sizeof(extension_sig);
	if (memcmp(fwptr, extension_sig, 4) != 0)
		return -1;

	while (fwptr >= *fw_data + (sizeof(*epatch_info) + 3)) {
		opcode = *--fwptr;
		length = *--fwptr;
		data = *--fwptr;

		if (opcode == 0xff)
			break;

		if (length == 0)
			return -1;

		if (opcode == 0 && length == 1) {
			project_id = data;
			break;
		}

		fwptr -= length;
	}

	if (project_id < 0)
		return -1;

	if (project_id > ARRAY_SIZE(project_id_to_lmp_subver))
		return -1;

	if (lmp_subver != project_id_to_lmp_subver[project_id])
		return -1;

	epatch_info = (rtk_epatch_header *)*fw_data;
	if (memcmp(epatch_info->signature, rtk_epatch_signature, 8) != 0)
		return -1;

	min_size += 8 * epatch_info->num_patches;
	if (fw_len < min_size)
		return -1;

	chip_id_base = *fw_data + sizeof(*epatch_info);
	patch_length_base = chip_id_base +
			(sizeof(uint16_t) * epatch_info->num_patches);
	patch_offset_base = patch_length_base +
			(sizeof(uint16_t) * epatch_info->num_patches);
	for (i = 0; i < epatch_info->num_patches; i++) {
		uint16_t chip_id = *(uint16_t *)(chip_id_base +
				(i * sizeof(uint16_t)));
		if (chip_id == rom_version + 1) {
			patch_length = *(uint16_t *)(patch_length_base +
					(i * sizeof(uint16_t)));
			patch_offset = *(uint32_t *)(patch_offset_base +
					(i * sizeof(uint32_t)));
			break;
		}
	}

	if (!patch_offset)
		return -1;

	min_size = patch_offset + patch_length;
	if (fw_len < min_size)
		return -1;

	buf = malloc(patch_length);
	if (!buf)
		return -1;
	memcpy(buf, *fw_data + patch_offset, patch_length);
	free(*fw_data);

	memcpy(buf + patch_length - 4, &epatch_info->fw_version, 4);

	*fw_data = buf;
	return patch_length;
}

static int rtk_request_firmware(const char *filename, unsigned char **fw_data)
{
	unsigned char *buf;
	struct stat st;
	size_t len;
	int fd;

	if (stat(filename, &st) < 0) {
		fprintf(stderr, "can't access firmware:%s", filename);
		return -1;
	}

	len = st.st_size;
	buf = malloc(len);
	if (!buf)
		return -1;

	if ((fd = open(filename, O_RDONLY)) < 0) {
		free(buf);
		return -1;
	}

	if (read(fd, buf, len) != len) {
		free(buf);
		close(fd);
		return -1;
	}

	close(fd);

	*fw_data = buf;
	return len;
}

static int rtk_patch_firmware(int dd)
{
	struct hci_request rq;
	read_local_version_rp rp;
	uint16_t lmp_subver;
	const char *fw_name = NULL;
	unsigned char *fw_data = NULL;
	int ret;

	rq.ogf    = OGF_INFO_PARAM;
	rq.ocf    = OCF_READ_LOCAL_VERSION;
	rq.event  = 0;
	rq.cparam = NULL;
	rq.clen   = 0;
	rq.rparam = &rp;
	rq.rlen   = READ_LOCAL_VERSION_RP_SIZE;

	if (hci_send_req(dd, &rq, 1000) < 0)
		return -1;

	if (rp.status) {
		errno = EIO;
		return -1;
	}

	lmp_subver = rp.lmp_subver;

	switch (lmp_subver) {
	case RTK_ROM_LMP_8723B:
		fw_name = FIRMWARE_DIR"rtl_bt/rtl8723b_fw.bin";
		break;
	default:
		fprintf(stderr, "not support yet!");
		return -1;
	}

	ret = rtk_request_firmware(fw_name, &fw_data);
	if (ret < 0)
		return ret;

	ret = rtk_parse_firmware(dd, lmp_subver, &fw_data, ret);
	if (ret < 0)
		goto out;

	ret = rtk_download_firmware(dd, fw_data, ret);

out:
	free(fw_data);
	return ret;
}

int rtk_init(int fd, int *speed, struct termios *ti)
{
	ti->c_cflag |= PARENB;
	ti->c_cflag &= ~(PARODD);

	if (tcsetattr(fd, TCSANOW, ti) < 0) {
		perror("cannot set port settings");
		return -1;
	}

	/* TODO set_speed */

	return 0;
}

int rtk_post(int fd, struct termios *ti)
{
	int dev_id, dd;
	struct timespec tm = { 0, 50000 };
	int ret;

	sleep(1);

	dev_id = ioctl(fd, HCIUARTGETDEVICE, 0);
	if (dev_id < 0) {
		perror("cannot get device id");
		return dev_id;
	}

	dd = hci_open_dev(dev_id);
	if (dd < 0) {
		perror("HCI device open failed");
		return dd;
	}

	if (ioctl(dd, HCIDEVUP, dev_id) < 0 && errno != EALREADY) {
		perror("HCI device up failed");
		hci_close_dev(dd);
		return -1;
	}

	ret = rtk_patch_firmware(dd);

	nanosleep(&tm, NULL);
	hci_close_dev(dd);

	return ret;
}
