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

#ifndef __UNIFIED_PLATFORM_DRIVER__
#define __UNIFIED_PLATFORM_DRIVER__


int diagchar_init(void);
void diagchar_exit(void);
int cnss_initialize(void);
void cnss_exit(void);
int mhi_init(void);
void mhi_exit(void);
int mhi_driver_init(void);
void mhi_driver_exit(void);
int diag_rpmsg_driver_init(void);
void diag_rpmsg_driver_exit(void);
int cld80211_init(void);
void cld80211_exit(void);
int qrtr_proto_init(void);
void qrtr_proto_fini(void);
int subsys_restart_init(void);
void subsys_restart_exit(void);


#endif
