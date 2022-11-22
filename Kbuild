ifneq ($(CONFIG_CNSS2),)
     KBUILD_CPPFLAGS += -DCONFIG_CNSS2
endif

ifneq ($(CONFIG_CNSS2_DEBUG),)
     KBUILD_CPPFLAGS += -DCONFIG_CNSS2_DEBUG
endif

ifneq ($(CONFIG_CNSS2_QMI),)
     KBUILD_CPPFLAGS += -DCONFIG_CNSS2_QMI
endif

ifneq ($(CONFIG_CNSS_ASYNC),)
     KBUILD_CPPFLAGS += -DCONFIG_CNSS_ASYNC
endif

ifneq ($(CONFIG_CNSS_GENL),)
     KBUILD_CPPFLAGS += -DCONFIG_CNSS_GENL
endif

ifneq ($(CONFIG_MHI_BUS),)
     KBUILD_CPPFLAGS += -DCONFIG_MHI_BUS
endif

ifneq ($(CONFIG_MHI_DEBUG),)
     KBUILD_CPPFLAGS += -DCONFIG_MHI_DEBUG
endif

ifneq ($(CONFIG_MHI_QCOM),)
     KBUILD_CPPFLAGS += -DCONFIG_MHI_QCOM
endif

ifneq ($(CONFIG_QRTR),)
     KBUILD_CPPFLAGS += -DCONFIG_QRTR
endif

ifneq ($(CONFIG_QRTR_MHI),)
     KBUILD_CPPFLAGS += -DCONFIG_QRTR_MHI
endif

ifneq ($(CONFIG_QCOM_QMI_HELPERS),)
     KBUILD_CPPFLAGS += -DCONFIG_QCOM_QMI_HELPERS
endif

ifneq ($(CONFIG_MSM_SUBSYSTEM_RESTART),)
     KBUILD_CPPFLAGS += -DCONFIG_MSM_SUBSYSTEM_RESTART
endif

ifneq ($(CONFIG_DIAG_CHAR),)
    KBUILD_CPPFLAGS += -DCONFIG_DIAG_CHAR
endif

ifneq ($(CONFIG_DIAGFWD_BRIDGE_CODE),)
    KBUILD_CPPFLAGS += -DCONFIG_DIAGFWD_BRIDGE_CODE
endif

ifneq ($(CONFIG_DIAG_OPTIMIZE),)
    KBUILD_CPPFLAGS += -DCONFIG_DIAG_OPTIMIZE
endif

ifneq ($(CONFIG_WLAN_CNSS_CORE),)
    KBUILD_CPPFLAGS += -DCONFIG_WLAN_CNSS_CORE
endif

ifneq ($(CONFIG_PCI_SUSPEND_RESUME),)
    KBUILD_CPPFLAGS += -DPCI_SUPPORT_SUSPEND_RESUME
endif

ifneq ($(CONFIG_WLAN_EN),)
    KBUILD_CPPFLAGS += -DSUPPORT_WLAN_EN
endif

ifneq ($(CONFIG_PCI_RC_SUPPORT_PM),)
    KBUILD_CPPFLAGS += -DPCI_RC_SUPPORT_PM
endif

ifeq ($(CONFIG_ONE_MSI_VECTOR),y)
KBUILD_CPPFLAGS += -DCONFIG_ONE_MSI_VECTOR
endif

CDEFINES :=	-Wall\
		-Werror
KBUILD_CPPFLAGS += $(CDEFINES)



CNSS_GENL_DIR := cnss_genl
CNSS_DIR := cnss2
DIAG_DIR := diag
MHI_DIR := mhi
QMI_DIR := qmi
QRTR_DIR := qrtr


INIT_OBJS := unified_wlan_cnsscore.o
INIT_INC := -I$(ROOTDIR)

ifeq ($(CONFIG_CNSS_GENL),y)
	CNSS_GENL_OBJS := $(CNSS_GENL_DIR)/cnss_nl.o
endif

ifneq ($(CONFIG_CNSS2),)
	CNSS_OBJS := $(CNSS_DIR)/main.o                           \
		     $(CNSS_DIR)/bus.o                            \
		     $(CNSS_DIR)/debug.o                          \
		     $(CNSS_DIR)/pci.o                          \
		     $(CNSS_DIR)/usb.o                          \
		     $(CNSS_DIR)/power.o                          
ifeq ($(CONFIG_CNSS2_DEBUG),y)
	CNSS_OBJS += $(CNSS_DIR)/genl.o									
endif
ifeq ($(CONFIG_CNSS2_QMI),y)
	CNSS_OBJS += $(CNSS_DIR)/qmi.o								
	CNSS_OBJS += $(CNSS_DIR)/wlan_firmware_service_v01.o			
endif
	CNSS_INC := -I$(ROOTDIR)/$(CNSS_DIR)
endif

ifneq ($(CONFIG_QCOM_QMI_HELPERS), )
	QMI_OBJS := $(QMI_DIR)/qmi_encdec.o        \
		    $(QMI_DIR)/qmi_interface.o
	QMI_INC := -I$(ROOTDIR)/$(QMI_DIR)
endif

ifneq ($(CONFIG_MSM_SUBSYSTEM_RESTART), )
	QMI_OBJS += $(QMI_DIR)/subsystem_notif.o        \
		    $(QMI_DIR)/subsystem_restart.o \
		    $(QMI_DIR)/ramdump.o 
	QMI_INC := -I$(ROOTDIR)/$(QMI_DIR)
endif

ifneq ($(CONFIG_MHI_BUS),)
	MHI_OBJS := $(MHI_DIR)/core/mhi_main.o           \
				$(MHI_DIR)/core/mhi_init.o           \
				$(MHI_DIR)/core/mhi_pm.o            \
				$(MHI_DIR)/core/mhi_boot.o       \
				$(MHI_DIR)/core/mhi_dtr.o       
ifeq ($(CONFIG_MHI_QCOM),y)
	MHI_OBJS += $(MHI_DIR)/controllers/mhi_qcom.o   
endif
	MHI_INC := -I$(ROOTDIR)/$(MHI_DIR)/core \
			   -I$(ROOTDIR)/$(MHI_DIR)/controllers 
endif

ifneq ($(CONFIG_QRTR), )
	QRTR_OBJS := $(QRTR_DIR)/qrtr.o          
ifeq ($(CONFIG_QRTR_MHI),y)
	QRTR_OBJS += $(QRTR_DIR)/mhi.o   
endif	
	QRTR_INC := -I$(ROOTDIR)/$(QRTR_DIR)
endif

ifneq ($(CONFIG_DIAG_CHAR),)
	DIAG_OBJS :=  $(DIAG_DIR)/diagchar_core.o                \
		     $(DIAG_DIR)/diagchar_hdlc.o                   \
		     $(DIAG_DIR)/diagfwd.o            \
		     $(DIAG_DIR)/diagfwd_peripheral.o            \
		     $(DIAG_DIR)/diagfwd_socket.o \
		     $(DIAG_DIR)/diagfwd_rpmsg.o \
		     $(DIAG_DIR)/diag_mux.o \
		     $(DIAG_DIR)/diag_memorydevice.o \
		     $(DIAG_DIR)/diagmem.o \
		     $(DIAG_DIR)/diagfwd_cntl.o \
		     $(DIAG_DIR)/diag_dci.o \
		     $(DIAG_DIR)/diag_masks.o \
		     $(DIAG_DIR)/diag_debugfs.o \
		     $(DIAG_DIR)/diag_pcie.o \
		     $(DIAG_DIR)/diag_local.o \
		     $(DIAG_DIR)/diag_nl.o 
ifeq ($(CONFIG_MHI_BUS),y)
	DIAG_OBJS += $(DIAG_DIR)/diagfwd_mhi.o
endif
ifeq ($(CONFIG_DIAGFWD_BRIDGE_CODE),y)
	DIAG_OBJS += $(DIAG_DIR)/diagfwd_bridge.o
endif
	DIAG_INC := -I$(ROOTDIR)/$(DIAG_DIR)
endif


OBJS := $(INIT_OBJS)                      \
	$(CNSS_OBJS)                       \
	$(CNSS_GENL_OBJS)                       \
	$(MHI_OBJS)                        \
	$(QRTR_OBJS)                       \
	$(QMI_OBJS)                        \
	$(DIAG_OBJS)                       \


INCS := $(INIT_INC)                     \
        $(CNSS_INC)                     \
        $(MHI_INC)                      \
        $(DIAG_INC)                     \
        $(QMI_INC)                      \
        $(QRTR_INC)                      \

cflags-y += $(INCS)
ccflags-y += -Os -I$(src)/inc -I$(src)/inc/uapi

obj-$(WLAN_CNSSCORE) +=$(MODNAME).o
$(MODNAME)-y := $(OBJS)
