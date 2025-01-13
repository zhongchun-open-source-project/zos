# config.mk
#
# Product-specific compile-time definitions.
#

# The generic product target doesn't have any hardware-specific pieces.
TARGET_NO_BOOTLOADER := true
TARGET_NO_KERNEL := true
TARGET_CPU_ABI := armeabi
#HAVE_HTC_AUDIO_DRIVER := true
#For Audio
BOARD_USES_GENERIC_AUDIO := false
BUILD_WITH_ALSA_UTILS := true
BOARD_USES_ALSA_AUDIO := true

#For wifi
WPA_BUILD_SUPPLICANT := true
BOARD_WPA_SUPPLICANT_DRIVER := WEXT
CONFIG_CTRL_IFACE := y
DRIVER_BUILT_IN := true
