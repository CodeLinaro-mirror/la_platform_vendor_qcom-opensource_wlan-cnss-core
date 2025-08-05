/* Copyright (c) 2016-2019, The Linux Foundation. All rights reserved.
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

#ifndef _CNSS_MAIN_H
#define _CNSS_MAIN_H

#include <linux/version.h>
#include <linux/etherdevice.h>
#include <linux/gpio.h>
#include <linux/pm_qos.h>
#ifdef CONFIG_ARCH_QCOM
#include <net/cnss2.h>
#include <linux/esoc_client.h>
#include <linux/msm-bus.h>
#include <soc/qcom/memory_dump.h>
#include <soc/qcom/subsystem_restart.h>
#endif

#include "qmi.h"
#include "cnss2.h"

#define MAX_NO_OF_MAC_ADDR		4
#define CNSS_RDDM_TIMEOUT_MS		20000
#define MAX_FIRMWARE_NAME_LEN		32
#define UNUSED(x)			(void)(x)

#define CNSS_EVENT_SYNC   BIT(0)
#define CNSS_EVENT_UNINTERRUPTIBLE BIT(1)
#define CNSS_EVENT_SYNC_UNINTERRUPTIBLE (CNSS_EVENT_SYNC | \
				CNSS_EVENT_UNINTERRUPTIBLE)
#define QCN7605_CALDB_SIZE 614400
#define HOST_WAKE_GPIO_IN 144

#define POWER_ON_RETRY_MAX_TIMES	4
#define POWER_ON_RETRY_DELAY_MS		500

#define CNSS_FW_PATH_MAX_LEN 32

extern unsigned long quirks;
enum cnss_bdf_type {
    CNSS_BDF_BIN,
    CNSS_BDF_ELF,
    CNSS_BDF_FLASH,
    CNSS_BDF_EEPROM,
    CNSS_BDF_REGDB,
};

#define BDFSTR(bdf) \
       bdf == CNSS_BDF_BIN ? "BDF_BIN" : \
       bdf == CNSS_BDF_ELF ? "BDF_ELF" : \
       bdf == CNSS_BDF_FLASH ? "BDF_FLASH" : \
       bdf == CNSS_BDF_EEPROM ? "BDF_EEPROM" : \
       bdf == CNSS_BDF_REGDB ? "BDF_REGDB" : \
       "unknow BDF"

enum cnss_dev_bus_type {
	CNSS_BUS_NONE = -1,
	CNSS_BUS_PCI,
	CNSS_BUS_USB,
	CNSS_BUS_SDIO,
	CNSS_BUS_MAX,
};

struct cnss_vreg_info {
	struct regulator *reg;
	const char *name;
	u32 min_uv;
	u32 max_uv;
	u32 load_ua;
	u32 delay_us;
};

struct cnss_pinctrl_info {
	struct pinctrl *pinctrl;
	struct pinctrl_state *bootstrap_active;
	struct pinctrl_state *wlan_en_active;
	struct pinctrl_state *wlan_en_sleep;
};

#ifndef CONFIG_ARCH_QCOM
/* Dummy structure to eliminate compiler warning */
struct subsys_desc {
	;
};
#endif

struct cnss_subsys_info {
	struct subsys_device *subsys_device;
#ifdef CONFIG_ARCH_QCOM
	struct subsys_desc subsys_desc;
#endif
	void *subsys_handle;
};

struct cnss_ramdump_info {
	struct ramdump_device *ramdump_dev;
	unsigned long ramdump_size;
	void *ramdump_va;
	phys_addr_t ramdump_pa;
#ifdef CONFIG_ARCH_QCOM
	struct msm_dump_data dump_data;
#endif
};

struct cnss_dump_seg {
	unsigned long address;
	void *v_address;
	unsigned long size;
	u32 type;
};

struct cnss_dump_data {
	u32 version;
	u32 magic;
	char name[32];
	phys_addr_t paddr;
	int nentries;
	u32 seg_version;
};

struct cnss_ramdump_info_v2 {
	struct ramdump_device *ramdump_dev;
	unsigned long ramdump_size;
	void *dump_data_vaddr;
	bool dump_data_valid;
	struct cnss_dump_data dump_data;
};

struct cnss_esoc_info {
	struct esoc_desc *esoc_desc;
	bool notify_modem_status;
	void *modem_notify_handler;
	int modem_current_status;
};

struct cnss_bus_bw_info {
	struct msm_bus_scale_pdata *bus_scale_table;
	u32 bus_client;
	int current_bw_vote;
};

struct cnss_wlan_mac_addr {
	u8 mac_addr[MAX_NO_OF_MAC_ADDR][ETH_ALEN];
	u32 no_of_mac_addr_set;
};

struct cnss_wlan_mac_info {
	struct cnss_wlan_mac_addr wlan_mac_addr;
	bool is_wlan_mac_set;
};

struct cnss_fw_mem {
	size_t size;
	void *va;
	phys_addr_t pa;
	bool valid;
	int type;
	phys_addr_t phys_addr;
	void *pre_aligned;
};

enum cnss_driver_event_type {
	CNSS_DRIVER_EVENT_SERVER_ARRIVE,
	CNSS_DRIVER_EVENT_SERVER_EXIT,
	CNSS_DRIVER_EVENT_REQUEST_MEM,
	CNSS_DRIVER_EVENT_FW_MEM_READY,
	CNSS_DRIVER_EVENT_FW_READY,
	CNSS_DRIVER_EVENT_COLD_BOOT_CAL_START,
	CNSS_DRIVER_EVENT_COLD_BOOT_CAL_DONE,
	CNSS_DRIVER_EVENT_REGISTER_DRIVER,
	CNSS_DRIVER_EVENT_UNREGISTER_DRIVER,
	CNSS_DRIVER_EVENT_RECOVERY,
	CNSS_DRIVER_EVENT_FORCE_FW_ASSERT,
	CNSS_DRIVER_EVENT_POWER_UP,
	CNSS_DRIVER_EVENT_POWER_DOWN,
	CNSS_DRIVER_EVENT_CAL_UPDATE,
	CNSS_DRIVER_EVENT_CAL_DOWNLOAD,
	CNSS_DRIVER_EVENT_MAX,
};

enum cnss_driver_state {
	CNSS_QMI_WLFW_CONNECTED,
	CNSS_FW_MEM_READY,
	CNSS_FW_READY,
	CNSS_COLD_BOOT_CAL,
	CNSS_DRIVER_LOADING,
	CNSS_DRIVER_UNLOADING,
	CNSS_DRIVER_PROBED,
	CNSS_DRIVER_RECOVERY,
	CNSS_FW_BOOT_RECOVERY,
	CNSS_DEV_ERR_NOTIFY,
	CNSS_DRIVER_DEBUG,
	CNSS_DEV_REMOVED,
	CNSS_IN_PANIC,
	CNSS_DEV_SHUTDOWN,
};

struct cnss_recovery_data {
	enum cnss_recovery_reason reason;
};

enum cnss_pins {
	CNSS_WLAN_EN,
	CNSS_PCIE_TXP,
	CNSS_PCIE_TXN,
	CNSS_PCIE_RXP,
	CNSS_PCIE_RXN,
	CNSS_PCIE_REFCLKP,
	CNSS_PCIE_REFCLKN,
	CNSS_PCIE_RST,
	CNSS_PCIE_WAKE,
};

struct cnss_pin_connect_result {
	u32 fw_pwr_pin_result;
	u32 fw_phy_io_pin_result;
	u32 fw_rf_pin_result;
	u32 host_pin_result;
};

enum cnss_debug_quirks {
	LINK_DOWN_SELF_RECOVERY,
	SKIP_DEVICE_BOOT,
	USE_CORE_ONLY_FW,
	SKIP_RECOVERY,
	ENABLE_PCI_LINK_PS,
};

struct cnss_cal_data {
	u32 index;
	u32 total_size;
};

struct cnss_plat_data {
	struct platform_device *plat_dev;
	enum cnss_driver_mode driver_mode;
	void *bus_priv;
	enum cnss_dev_bus_type bus_type;
	struct gpio_desc *gpio_wl_en;
	struct cnss_vreg_info *vreg_info;
	struct cnss_pinctrl_info pinctrl_info;
	struct cnss_subsys_info subsys_info;
	struct cnss_ramdump_info ramdump_info;
	struct cnss_ramdump_info_v2 ramdump_info_v2;
	struct cnss_esoc_info esoc_info;
	struct cnss_bus_bw_info bus_bw_info;
	struct notifier_block modem_nb;
	struct cnss_platform_cap cap;
	struct pm_qos_request qos_request;
	unsigned long device_id;
	struct cnss_wlan_driver *driver_ops;
	enum cnss_driver_status driver_status;
	u32 recovery_count;
	struct cnss_wlan_mac_info wlan_mac_info;
	unsigned long driver_state;
	struct list_head event_list;
	spinlock_t event_lock; /* spinlock for driver work event handling */
	struct work_struct event_work;
	struct workqueue_struct *event_wq;
	struct qmi_handle *qmi_wlfw_clnt;
	struct work_struct qmi_recv_msg_work;
	struct notifier_block qmi_wlfw_clnt_nb;
	struct wlfw_rf_chip_info_s_v01 chip_info;
	struct wlfw_rf_board_info_s_v01 board_info;
	struct wlfw_soc_info_s_v01 soc_info;
	struct wlfw_fw_version_info_s_v01 fw_version_info;
	char firmware_name[CNSS_FW_PATH_MAX_LEN];
	struct cnss_dev_mem_info dev_mem_info[CNSS_MAX_DEV_MEM_NUM];
	u32 fw_mem_seg_len;
	struct cnss_fw_mem fw_mem[QMI_WLFW_MAX_NUM_MEM_SEG_V01];
	struct cnss_fw_mem m3_mem;
	u32 *qdss_reg;
	struct cnss_pin_connect_result pin_result;
	struct dentry *root_dentry;
	atomic_t pm_count;
	struct timer_list fw_boot_timer;
	int cssr_count;
	unsigned int cssr_timeout;
	int cssr_detected;
	struct timer_list cssr_timer;
	struct completion power_up_complete;
	struct mutex dev_lock; /* mutex for register access through debugfs */
	u32 diag_reg_read_addr;
	u32 diag_reg_read_mem_type;
	u32 diag_reg_read_len;
	u8 *diag_reg_read_buf;
	void *caldb_mem;
	bool cal_done;
	struct completion rddm_complete;
	bool fw_pcie_gen_switch;
	u8 pcie_gen_speed;
	bool is_converged_dt;
	struct device_node *dev_node;
	u32 wlan_en_gpio;
	u8 wlan_en_active;
	u8 power_on;
	bool single_msi;
	u32 wow_ssr_count;
	u8 wow_ssr_suppressed;
	struct wlchip_serial_id_v01 serial_id;
};

struct cnss_plat_data *cnss_get_plat_priv(struct platform_device *plat_dev);
unsigned long *cnss_get_debug_quirks(void);
int cnss_driver_event_post(struct cnss_plat_data *plat_priv,
			   enum cnss_driver_event_type type,
			   u32 flags, void *data);
int cnss_get_vreg(struct cnss_plat_data *plat_priv);
int cnss_get_pinctrl(struct cnss_plat_data *plat_priv);

#ifndef CONFIG_MSM_GVM_QUIN
int cnss_power_on_device(struct cnss_plat_data *plat_priv);
void cnss_power_off_device(struct cnss_plat_data *plat_priv);
#else /* CONFIG_MSM_GVM_QUIN */
static inline int cnss_power_on_device(struct cnss_plat_data *plat_priv)
{
	return 0;
}

static inline void cnss_power_off_device(struct cnss_plat_data *plat_priv)
{
}
#endif /* CONFIG_MSM_GVM_QUIN */

int cnss_register_subsys(struct cnss_plat_data *plat_priv);
void cnss_unregister_subsys(struct cnss_plat_data *plat_priv);
int cnss_register_ramdump(struct cnss_plat_data *plat_priv);
void cnss_unregister_ramdump(struct cnss_plat_data *plat_priv);
void cnss_set_pin_connect_status(struct cnss_plat_data *plat_priv);
u32 cnss_get_wake_msi(struct cnss_plat_data *plat_priv);
bool *cnss_get_qmi_bypass(void);
bool is_qcn7605_device(u16 device_id);
void cnss_set_wlan_chip_to_host_wakeup(unsigned int wakeup_gpio_num);
void cnss_set_driver_status(enum cnss_driver_status driver_status);
u8 *cnss_common_get_wlan_mac_address(struct device *dev, u32 *num);

int cnss_set_wlan_unsafe_channel(u16 *unsafe_ch_list, u16 ch_count);
int cnss_get_wlan_unsafe_channel(u16 *unsafe_ch_list,
				 u16 *ch_count, u16 buf_len);
int cnss_wlan_set_dfs_nol(const void *info, u16 info_len);
int cnss_wlan_get_dfs_nol(void *info, u16 info_len);
#endif /* _CNSS_MAIN_H */
