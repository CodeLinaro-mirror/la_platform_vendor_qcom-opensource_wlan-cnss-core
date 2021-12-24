/* Copyright (c) 2008-2019, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/ratelimit.h>
#include <linux/timer.h>
#include <linux/kmemleak.h>
#include <linux/sched.h>
#include <linux/platform_device.h>
#ifdef CONFIG_DIAG_OVER_USB
#include <linux/usb/usbdiag.h>
#endif
#include <asm/current.h>
#include "diagmem.h"
#include "diagchar.h"
#include "diagfwd.h"
#ifdef CONFIG_ARCH_QCOM
#include "diagchar_hdlc.h"
#include "diagfwd_cntl.h"
#include "diag_dci.h"
#include "diag_debugfs.h"
#include "diag_usb.h"
#include "diag_memorydevice.h"
#include "diag_ipc_logging.h"
#include "diagfwd_peripheral.h"
#endif
#include "diag_masks.h"
#include "diagfwd_bridge.h"
#include "diag_mux.h"
#include "msm_mhi.h"
#include "diagchar.h"

#include <linux/coresight-stm.h>
#include <linux/kernel.h>
#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

#include <net/netlink.h>
#include <net/sock.h>

MODULE_DESCRIPTION("Diag Char Driver");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0");

#define MIN_SIZ_ALLOW 4
#define INIT	1
#define EXIT	-1
struct diagchar_dev *driver;
struct diagchar_priv {
	int pid;
};

/* Memory pool variables */
/* Used for copying any incoming packet from user space clients. */
static unsigned int poolsize = 12;
module_param(poolsize, uint, 0);

/*
 * Used for HDLC encoding packets coming from the user
 * space.
 */
static unsigned int poolsize_hdlc = 10;
module_param(poolsize_hdlc, uint, 0);

/* Used for reading data from the remote device. */
static unsigned int itemsize_mdm = DIAG_MDM_BUF_SIZE;
static unsigned int poolsize_mdm = 18;
module_param(itemsize_mdm, uint, 0);
module_param(poolsize_mdm, uint, 0);

static int diag_remote_init(void)
{
	uint32_t itemsize = DIAG_MAX_REQ_SIZE;
	uint32_t itemsize_hdlc = DIAG_MAX_HDLC_BUF_SIZE + APF_DIAG_PADDING;
	diagmem_setsize(POOL_TYPE_COPY, itemsize, poolsize);
	diagmem_setsize(POOL_TYPE_HDLC, itemsize_hdlc, poolsize_hdlc);
	diagmem_setsize(POOL_TYPE_MDM, itemsize_mdm, poolsize_mdm);

	diagmem_init(driver, POOL_TYPE_COPY);
	diagmem_init(driver, POOL_TYPE_HDLC);
	diagmem_init(driver, POOL_TYPE_MDM);

	driver->hdlc_encode_buf = kzalloc(DIAG_MAX_HDLC_BUF_SIZE, GFP_KERNEL);
	if (!driver->hdlc_encode_buf)
		return -ENOMEM;
	driver->hdlc_encode_buf_len = 0;

	driver->hdlc_buf = kzalloc(DIAG_MAX_HDLC_BUF_SIZE, GFP_KERNEL);
	if (!driver->hdlc_buf)
		return -ENOMEM;
	driver->hdlc_buf_len = 0;

	return 0;
}

static void diag_remote_exit(void)
{
	diagmem_exit(driver, POOL_TYPE_COPY);
	diagmem_exit(driver, POOL_TYPE_HDLC);
	diagmem_exit(driver, POOL_TYPE_MDM);

	if(driver->hdlc_encode_buf)
		kfree(driver->hdlc_encode_buf);

	if(driver->hdlc_buf)
		kfree(driver->hdlc_buf);
}

static int diag_mhi_probe(struct platform_device *pdev)
{
	int ret;

	printk("diag_mhi_probe start \n");
	if (!mhi_is_device_ready(&pdev->dev, "qcom,mhi"))
		return -EPROBE_DEFER;
	driver->pdev = pdev;
	ret = diag_remote_init();
	if (ret) {
		diag_remote_exit();
		return ret;
	}
	ret = diagfwd_bridge_init();
	if (ret) {
		diagfwd_bridge_exit();
		return ret;
	}
	printk("diag: mhi device is ready\n");
	return 0;
}

static const struct of_device_id diag_mhi_table[] = {
	{.compatible = "qcom,diag-mhi"},
	{},
};

static struct platform_driver diag_mhi_driver = {
	.probe = diag_mhi_probe,
	.driver = {
		.name = "DIAG MHI Platform",
		.owner = THIS_MODULE,
		.of_match_table = diag_mhi_table,
	},
};

#ifdef CONFIG_WLAN_CNSS_CORE
int diagchar_init(void)
#else
static int __init diagchar_init(void)
#endif
{
	int ret = 0;

	printk("diagchar initializing ..\n");
	driver = kzalloc(sizeof(struct diagchar_dev) + 5, GFP_KERNEL);
	if (!driver)
		return -ENOMEM;
	kmemleak_not_leak(driver);

	driver->time_sync_enabled = 0;
	driver->uses_time_api = 0;
	driver->poolsize = poolsize;
	driver->poolsize_hdlc = poolsize_hdlc;

	driver->logging_mode = DIAG_LOCAL_MODE;
	driver->mask_check = 0;
	driver->hdlc_encode_buf = NULL;
	driver->hdlc_buf = NULL;

	mutex_init(&driver->hdlc_disable_mutex);
	mutex_init(&driver->diagchar_mutex);
	mutex_init(&driver->diag_maskclear_mutex);
	mutex_init(&driver->diag_notifier_mutex);
	mutex_init(&driver->msg_mask_lock);
	mutex_init(&driver->hdlc_recovery_mutex);
	mutex_init(&driver->diag_hdlc_mutex);
	driver->num = 1;

	printk("diagchar initialized now");
	platform_driver_register(&diag_mhi_driver);
	return 0;
}

#ifdef CONFIG_WLAN_CNSS_CORE
void diagchar_exit(void)
#else
static void diagchar_exit(void)
#endif
{
	printk(KERN_INFO "diagchar exiting ..\n");
	diagfwd_bridge_exit();
	diag_remote_exit();
	printk(KERN_INFO "done diagchar exit\n");
}

#ifndef CONFIG_WLAN_CNSS_CORE
module_init(diagchar_init);
module_exit(diagchar_exit);
#endif
