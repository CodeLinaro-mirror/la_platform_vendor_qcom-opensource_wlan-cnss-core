ifeq ($(CONFIG_CNSS_OUT_OF_TREE),y)
KBUILD_CPPFLAGS += -DCONFIG_CNSS_OUT_OF_TREE
endif

ifeq ($(CONFIG_CNSS2_DEBUG),y)
KBUILD_CPPFLAGS += -DCONFIG_CNSS2_DEBUG
endif

ifeq ($(CONFIG_CNSS2_QMI),y)
KBUILD_CPPFLAGS += -DCONFIG_CNSS2_QMI
endif

ifeq ($(CONFIG_CNSS2_X86),y)
KBUILD_CPPFLAGS += -DCONFIG_CNSS2_X86
endif

ifeq ($(CONFIG_ONE_MSI_VECTOR),y)
KBUILD_CPPFLAGS += -DCONFIG_ONE_MSI_VECTOR
endif

ifeq ($(CONFIG_DUMP_FW_TO_FILE),y)
KBUILD_CPPFLAGS += -DCONFIG_DUMP_FW_TO_FILE
endif

ifeq ($(CONFIG_CNSS_QCOM_DEVCD_SUPPORT),y)
KBUILD_CPPFLAGS += -DCONFIG_CNSS_QCOM_DEVCD_SUPPORT
endif

ifeq ($(CONFIG_USERSPACE_HELPER_SUPPORT),y)
KBUILD_CPPFLAGS += -DCALL_USER_MODE_HELPER
endif

# CONFIG_CNSS_PLAT_IPC_QMI_SVC should never be "y" here since it
# can be only compiled as a module from out-of-kernel-tree source.
ifeq ($(CONFIG_CNSS_PLAT_IPC_QMI_SVC),m)
KBUILD_CPPFLAGS += -DCONFIG_CNSS_PLAT_IPC_QMI_SVC
endif

ifeq ($(CONFIG_IPC_LOGGING),y)
KBUILD_CPPFLAGS += -DCONFIG_IPC_LOGGING
endif


ifeq ($(CONFIG_PCIE_EMULATION), y)
    KBUILD_CPPFLAGS += -DCONFIG_PCIE_EMULATION
endif

ifeq ($(CONFIG_MHI_BUS), y)
	KBUILD_CPPFLAGS += -DCONFIG_MHI_BUS_M
endif

ifeq ($(CONFIG_MHI_BUS_PCI_GENERIC), y)
	KBUILD_CPPFLAGS += -DCONFIG_MHI_BUS_PCI_GENERIC
endif

ifeq ($(CONFIG_QRTR), y)
	KBUILD_CPPFLAGS += -DCONFIG_QRTR_M
endif

ifeq ($(CONFIG_QRTR_MHI), y)
	KBUILD_CPPFLAGS += -DCONFIG_QRTR_MHI_M
endif

ifeq ($(CONFIG_MHI_BUS_DEBUG), y)
	KBUILD_CPPFLAGS += -DCONFIG_MHI_BUS_DEBUG
endif

ifeq ($(CONFIG_WLAN_CNSS_CORE), y)
    KBUILD_CPPFLAGS += -DCONFIG_WLAN_CNSS_CORE
endif

ifeq ($(CONFIG_CNSS2_SSR_DRIVER_DUMP),y)
KBUILD_CPPFLAGS += -DCONFIG_CNSS2_SSR_DRIVER_DUMP
endif

ifeq ($(CONFIG_WLAN_INTERNAL_SLEEP_CLOCK),y)
KBUILD_CPPFLAGS += -DCONFIG_WLAN_INTERNAL_SLEEP_CLOCK
endif


ifneq ($(CONFIG_WLAN_CNSS_CORE), y)
ifeq ($(CONFIG_FULL_CORE_TECH),y)
obj-$(CONFIG_QRTR) += qrtr/
obj-$(CONFIG_QRTR_MHI) += qrtr/
obj-$(CONFIG_MHI_BUS) += mhi/
obj-$(CONFIG_QCOM_QMI_HELPERS) += qti/
endif
obj-$(CONFIG_CNSS2) += cnss2/
obj-$(CONFIG_CNSS_GENL) += cnss_genl/
obj-$(CONFIG_WCNSS_MEM_PRE_ALLOC) += cnss_prealloc/
obj-y += cnss_utils/

else
QRTR_DIR := qrtr
MHI_DIR := mhi/core
QTI_DIR := qti
IPCLOG_DIR := trace
CNSS_DIR := cnss2
CNSS_UTILS_DIR := cnss_utils
CNSS_PREALLOC_DIR := cnss_prealloc

INIT_OBJS := unified_wlan_cnsscore.o
INIT_INC := -I$(ROOTDIR)

ifeq ($(CONFIG_QRTR), y)
	QRTR_OBJS := $(QRTR_DIR)/qrtr.o                                         \
	             $(QRTR_DIR)/ns.o
ifeq ($(CONFIG_QRTR_SMD),y)
	QRTR_OBJS +=  $(QRTR_DIR)/smd.o
endif
ifeq ($(CONFIG_QRTR_TUN),y)
	QRTR_OBJS +=  $(QRTR_DIR)/tun.o
endif
ifeq ($(CONFIG_QRTR_MHI),y)
	QRTR_OBJS +=  $(QRTR_DIR)/mhi.o
endif
	QRTR_INC := -I$(ROOTDIR)/$(QRTR_DIR)
endif

ifeq ($(CONFIG_MHI_BUS), y)
	MHI_OBJS := $(MHI_DIR)/init.o                                       \
	            $(MHI_DIR)/main.o                                       \
	            $(MHI_DIR)/pm.o                                         \
	            $(MHI_DIR)/boot.o
ifeq ($(CONFIG_MHI_BUS_DEBUG),y)
	MHI_OBJS += $(MHI_DIR)/debugfs.o
endif
	MHI_INC := -I$(ROOTDIR)/$(MHI_DIR)
endif

ifeq ($(CONFIG_QCOM_QMI_HELPERS), y)
	QMI_HELPERS_OBJS := $(QTI_DIR)/qmi_encdec.o                            \
	                    $(QTI_DIR)/qmi_interface.o
	QMI_HELPERS_INC := -I$(QTI_DIR)
endif

ifeq ($(CONFIG_CNSS2), y)
	CNSS_OBJS := $(CNSS_DIR)/main.o                                     \
	             $(CNSS_DIR)/bus.o                                      \
	             $(CNSS_DIR)/debug.o                                    \
	             $(CNSS_DIR)/power.o                                    \
	             $(CNSS_DIR)/genl.o                                     \
	             $(CNSS_DIR)/coredump.o
ifeq ($(CONFIG_CNSS2_PCIE),y)
	CNSS_OBJS += $(CNSS_DIR)/pci.o
endif
ifeq ($(CONFIG_CNSS2_QMI),y)
	CNSS_OBJS += $(CNSS_DIR)/qmi.o                                          \
	             $(CNSS_DIR)/coexistence_service_v01.o                   \
	             $(CNSS_DIR)/ip_multimedia_subsystem_private_service_v01.o
endif
	CNSS_INC := -I$(CNSS_DIR)
endif

ifeq ($(CONFIG_CNSS_UTILS), y)
	CNSS_UTILS_OBJS := $(CNSS_UTILS_DIR)/cnss_utils.o
ifeq ($(CONFIG_CNSS_QMI_SVC),y)
	CNSS_UTILS_OBJS += $(CNSS_UTILS_DIR)/wlan_firmware_service_v01.o     \
                       $(CNSS_UTILS_DIR)/device_management_service_v01.o
endif
ifeq ($(CONFIG_CNSS_PLAT_IPC_QMI_SVC),y)
	CNSS_UTILS_OBJS += $(CNSS_UTILS_DIR)/cnss_plat_ipc_qmi.o             \
                       $(CNSS_UTILS_DIR)/cnss_plat_ipc_service_v01.o
endif
	CNSS_UTILS_INC := -I$(CNSS_UTILS_DIR)
endif

ifeq ($(CONFIG_IPC_LOGGING_MODULE), y)
	IPCLOG_OBJS := $(IPCLOG_DIR)/ipc_logging.o           \
	               $(IPCLOG_DIR)/ipc_logging_debug.o      
	IPCLOG_INC := -I$(ROOTDIR)/(IPCLOG_DIR)
endif

CNSS_PREALLOC_OBJS := $(CNSS_PREALLOC_DIR)/cnss_prealloc.o
CNSS_PREALLOC_INC := $(CNSS_PREALLOC_DIR)

OBJS := $(INIT_OBJS)
OBJS += $(QRTR_OBJS)                                                        \
	    $(MHI_OBJS)                                                     \
	    $(QMI_HELPERS_OBJS)                                             \
	    $(IPCLOG_OBJS)                                                  \
	    $(CNSS_OBJS)                                                    \
	    $(CNSS_UTILS_OBJS)                                              \
	    $(CNSS_PREALLOC_OBJS)

INCS := $(INIT_INC)

INCS += $(QRTR_INC)                                                     \
        $(MHI_INC)                                                      \
        $(QMI_HELPERS_INC)                                              \
        $(IPCLOG_INC)                                                   \
        $(CNSS_INC)                                                     \
        $(CNSS_UTILS_INC)                                               \
        $(CNSS_PREALLOC_INC)

cflags-y += $(INCS)
ccflags-y += -Os -I$(src)/inc -I$(src)/cnss_utils -I$(ROOTDIR)

obj-$(WLAN_CNSSCORE) +=$(MODNAME).o
$(MODNAME)-y := $(OBJS)

endif
