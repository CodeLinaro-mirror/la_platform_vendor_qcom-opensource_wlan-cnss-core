# Android makefile for the WLAN Module
LOCAL_PATH := $(call my-dir)

# Assume no targets will be supported
WLAN_CHIPSET := wlan_cnss_core_usb

ifeq ($(BOARD_HAS_QCOM_WLAN), true)

# Check if this driver needs be built for current target
ifneq ($(findstring qca_cld3,$(WIFI_DRIVER_BUILT)),)
	WLAN_CHIPSET := qca_cld3
	WLAN_SELECT  := WLAN_CNSSCORE=m
endif

# Build/Package only in case of supported target
ifneq ($(WLAN_CHIPSET),)

# This makefile is only for DLKM
ifneq ($(findstring vendor,$(LOCAL_PATH)),)

ifneq ($(findstring opensource,$(LOCAL_PATH)),)
	WLAN_BLD_DIR := vendor/qcom/opensource/wlan
endif # opensource
# Multi-ko check
LOCAL_DEV_NAME := $(lastword $(strip \
    $(subst ~, , \
    $(subst /, ,$(LOCAL_PATH)))))

ifeq (1, $(strip $(shell expr $(words $(strip $(TARGET_WLAN_CHIP))) \>= 2)))

ifeq ($(LOCAL_DEV_NAME), wlan-cnss-core)
LOCAL_MULTI_KO := true
else
LOCAL_MULTI_KO := false
endif

endif

ifeq ($(LOCAL_DEV_NAME), wlan-cnss-core)

LOCAL_DEV_NAME := wlan_cnss_core_usb
LOCAL_MOD_NAME := wlan_cnss_core_usb
LOCAL_SRC_DIR :=
else

LOCAL_SRC_DIR := ~$(LOCAL_DEV_NAME)
endif


#LOCAL_DEV_NAME := wlan-cnss-usb
#OCAL_MOD_NAME := wlan-cnss-usb
CMN_OFFSET := ..
LOCAL_SRC_DIR :=

# DLKM_DIR was moved for JELLY_BEAN (PLATFORM_SDK 16)
ifeq ($(call is-platform-sdk-version-at-least,16),true)
    DLKM_DIR := $(TOP)/device/qcom/common/dlkm
else
    DLKM_DIR := build/dlkm
endif # platform-sdk-version

# Build wlan.ko as $(WLAN_CHIPSET)_wlan.ko
###########################################################
# This is set once per LOCAL_PATH, not per (kernel) module
KBUILD_OPTIONS := WLAN_ROOT=$(WLAN_BLD_DIR)/wlan-cnss-core/$(LOCAL_SRC_DIR)
KBUILD_OPTIONS += CONFIG_WLAN_CNSS_CORE=y
KBUILD_OPTIONS += CONFIG_MSM_DIAG_INTERFACE=y CONFIG_MSM_QMI_INTERFACE=y CONFIG_IPC_ROUTER=y CONFIG_IPC_ROUTER_SECURITY=y \
                  CONFIG_QMI_ENCDEC=y CONFIG_QMI_ENCDEC_DEBUG=y CONFIG_CNSS2=y CONFIG_CNSS2_DEBUG=y CONFIG_NAPIER_X86=y   \
                  CONFIG_CNSS_UTILS=y
KBUILD_OPTIONS += CONFIG_HSIC_XPRT=y CONFIG_USB_QTI_KS_BRIDGE=y CONFIG_DIAG_HSIC=y CONFIG_DIAG_IPC_BRIDGE=y CONFIG_CNSS2_USB=y
# We are actually building wlan.ko here, as per the
# requirement we are specifying <chipset>_wlan.ko as LOCAL_MODULE.
# This means we need to rename the module to <chipset>_wlan.ko
# after wlan.ko is built.
KBUILD_OPTIONS += MODNAME=$(LOCAL_MOD_NAME)
KBUILD_OPTIONS += BOARD_PLATFORM=$(TARGET_BOARD_PLATFORM)
KBUILD_OPTIONS += $(WLAN_SELECT)

include $(CLEAR_VARS)
LOCAL_MODULE              := wlan_cnss_core_usb.ko
LOCAL_MODULE_KBUILD_NAME  := wlan_cnss_core_usb.ko
LOCAL_MODULE_DEBUG_ENABLE := true
ifeq ($(PRODUCT_VENDOR_MOVE_ENABLED),true)
    ifeq ($(WIFI_DRIVER_INSTALL_TO_KERNEL_OUT),true)
        LOCAL_MODULE_PATH := $(KERNEL_MODULES_OUT)
    else
        LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/lib/modules/$(WLAN_CHIPSET)
    endif
else
    LOCAL_MODULE_PATH := $(TARGET_OUT)/lib/modules/$(WLAN_CHIPSET)
endif

include $(DLKM_DIR)/AndroidKernelModule.mk
###########################################################

# Create Symbolic link
ifneq ($(findstring $(WLAN_CHIPSET),$(WIFI_DRIVER_DEFAULT)),)
ifeq ($(PRODUCT_VENDOR_MOVE_ENABLED),true)
ifneq ($(WIFI_DRIVER_INSTALL_TO_KERNEL_OUT),)
$(shell mkdir -p $(TARGET_OUT_VENDOR)/lib/modules; \
	ln -sf /$(TARGET_COPY_OUT_VENDOR)/lib/modules/$(WLAN_CHIPSET)/$(LOCAL_MODULE) $(TARGET_OUT_VENDOR)/lib/modules/$(LOCAL_MODULE))
endif
else
$(shell mkdir -p $(TARGET_OUT)/lib/modules; \
	ln -sf /system/lib/modules/$(WLAN_CHIPSET)/$(LOCAL_MODULE) $(TARGET_OUT)/lib/modules/$(LOCAL_MODULE))
endif
endif


endif # supported target check
endif # WLAN enabled check
endif # BOARD WLAN
