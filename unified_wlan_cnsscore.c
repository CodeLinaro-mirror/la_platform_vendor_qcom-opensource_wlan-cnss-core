/* Copyright (c) 2011-2019, The Linux Foundation. All rights reserved.
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


#include "unified_wlan_cnsscore.h"

static int unified_pdrv_init(void)
{
	int ret;

	/* mhi Registration */
#ifdef CONFIG_MHI_BUS
	ret = mhi_init();
	if (ret){
		printk("%s: updrv: failed to register mhi bus\n",__func__);
		goto fail1;
	}
#endif

#ifdef CONFIG_QRTR
	/* ipc_router Registration */
	ret = qrtr_proto_init();  
	if (ret){
		printk("%s: updrv: failed to register qrtr\n",__func__);
		goto fail2;
	}
#endif

#ifdef CONFIG_MSM_SUBSYSTEM_RESTART
	/* ipc_router Registration */
	ret = subsys_restart_init();  
	if (ret){
		printk("%s: updrv: failed to register subsys\n",__func__);
		goto fail3;
	}
#endif

#ifdef CONFIG_QRTR
	/* ipc_router Registration */
	ret = mhi_driver_init();  
	if (ret){
		printk("%s: updrv: failed to register mhi driver\n",__func__);
		goto fail4;
	}
#endif


#ifdef CONFIG_DIAG_CHAR
	/* diag Registration */
	ret = diagchar_init();
	if (ret){
		printk("%s: updrv: failed to register diag char\n",__func__);
		goto fail5;
	}

	ret = diag_rpmsg_driver_init();
	if (ret){
		printk("%s: updrv: failed to register diag rpmsg\n",__func__);
		goto fail6;
	}
#endif

#ifdef CONFIG_CNSS2
	/* cnss Registration */
	ret = cnss_initialize();
	if (ret){
		printk("%s: updrv: failed to register cnss\n",__func__);
		goto fail7;
	}
#endif


#ifdef CONFIG_CNSS_GENL
	/* cnss nl Registration */
	ret = cld80211_init();
	if (ret){
		printk("%s: updrv: failed to register cnss nl\n",__func__);
		goto fail8;
	}
#endif
	printk("unified_pdrv_init success\n");

	return 0;

#ifdef CONFIG_CNSS_GENL
fail8:
	cld80211_exit();
#endif
#ifdef CONFIG_CNSS2
fail7:
	cnss_exit();
#endif
#ifdef CONFIG_DIAG_CHAR
fail6:
	diag_rpmsg_driver_exit();
fail5:
	diagchar_exit();
#endif
#ifdef CONFIG_MHI_BUS
fail4:
	mhi_driver_exit();
#endif
#ifdef CONFIG_MSM_SUBSYSTEM_RESTART
fail3:

#endif
#ifdef CONFIG_QRTR
fail2: 
	qrtr_proto_fini();  
#endif
#ifdef CONFIG_MHI_BUS
fail1:
#endif
	printk("unified_pdrv_init failure %d\n", ret);
	return ret;
}

static void unified_pdrv_deinit(void)
{
#ifdef CONFIG_CNSS_GENL
	cld80211_exit();
#endif
#ifdef CONFIG_CNSS2
	cnss_exit();
#endif
#ifdef CONFIG_DIAG_CHAR
	diag_rpmsg_driver_exit();
	diagchar_exit();
#endif
#ifdef CONFIG_MHI_BUS
	mhi_driver_exit();
#endif
#ifdef CONFIG_MSM_SUBSYSTEM_RESTART

#endif
#ifdef CONFIG_QRTR
	qrtr_proto_fini();	
#endif
#ifdef CONFIG_MHI_BUS
#endif
}

module_init(unified_pdrv_init);
module_exit(unified_pdrv_deinit);
MODULE_DESCRIPTION("Unified Platform Driver");
MODULE_LICENSE("GPL v2");
