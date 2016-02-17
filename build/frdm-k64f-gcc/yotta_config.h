#ifndef __YOTTA_CONFIG_H__
#define __YOTTA_CONFIG_H__
// yotta config data (including backwards-compatible definitions)
#define TARGET FRDM_K64F_GCC
#define TARGET_LIKE_FRDM_K64F_GCC
#define YOTTA_BUILD_INFO_HEADER "/Users/douans01/Documents/workspace/GitHub/yotta_projects/mbed-ethernet-sample/build/frdm-k64f-gcc/yotta_build_info.h"
#define TARGET_LIKE_FRDM_K64F_GCC
#define TARGET_LIKE_FRDM_K64F
#define TARGET_LIKE_LWIP
#define TARGET_LIKE_LWIP_K64F
#define TARGET_LIKE_ETH_K64F
#define TARGET_LIKE_KINETIS_K64_GCC
#define TARGET_LIKE_K64F
#define TARGET_LIKE_KSDK_MCU
#define TARGET_LIKE_MK64FN1M0VMD12
#define TARGET_LIKE_MK64FN1M0
#define TARGET_LIKE_MK64FN
#define TARGET_LIKE_FREESCALE
#define TARGET_LIKE_CORTEX_M4
#define TARGET_LIKE_ARMV7_M
#define TARGET_LIKE_ARM
#define TARGET_LIKE_GCC
#define TARGET_LIKE_MBED
#define TARGET_LIKE_MBED_GCC
#define TARGET_LIKE_MBED
#define TARGET_LIKE_ARM
#define TARGET_LIKE_GCC
#define YOTTA_CFG
#define YOTTA_CFG_MINAR
#define YOTTA_CFG_MINAR_INITIAL_EVENT_POOL_SIZE 50
#define YOTTA_CFG_MINAR_ADDITIONAL_EVENT_POOLS_SIZE 100
#define YOTTA_CFG_MBED_OS
#define YOTTA_CFG_MBED_OS_NET
#define YOTTA_CFG_MBED_OS_NET_STACKS
#define YOTTA_CFG_MBED_OS_NET_STACKS_LWIP 1
#define YOTTA_CFG_MBED_OS_STDIO
#define YOTTA_CFG_MBED_OS_STDIO_DEFAULT_BAUD 9600
#define YOTTA_CFG_CMSIS
#define YOTTA_CFG_CMSIS_NVIC
#define YOTTA_CFG_CMSIS_NVIC_RAM_VECTOR_ADDRESS 0x1FFF0000
#define YOTTA_CFG_CMSIS_NVIC_FLASH_VECTOR_ADDRESS 0x0
#define YOTTA_CFG_CMSIS_NVIC_USER_IRQ_OFFSET 16
#define YOTTA_CFG_CMSIS_NVIC_USER_IRQ_NUMBER 86
#define YOTTA_CFG_HARDWARE
#define YOTTA_CFG_HARDWARE_PINS
#define YOTTA_CFG_HARDWARE_PINS_LED_RED PTB22
#define YOTTA_CFG_HARDWARE_PINS_LED_GREEN PTE26
#define YOTTA_CFG_HARDWARE_PINS_LED_BLUE PTB21
#define YOTTA_CFG_HARDWARE_PINS_LED1 LED_RED
#define YOTTA_CFG_HARDWARE_PINS_LED2 LED_GREEN
#define YOTTA_CFG_HARDWARE_PINS_LED3 LED_BLUE
#define YOTTA_CFG_HARDWARE_PINS_LED4 LED_RED
#define YOTTA_CFG_HARDWARE_PINS_SW2 PTC6
#define YOTTA_CFG_HARDWARE_PINS_SW3 PTA4
#define YOTTA_CFG_HARDWARE_PINS_USBTX PTB17
#define YOTTA_CFG_HARDWARE_PINS_USBRX PTB16
#define YOTTA_CFG_HARDWARE_PINS_D0 PTC16
#define YOTTA_CFG_HARDWARE_PINS_D1 PTC17
#define YOTTA_CFG_HARDWARE_PINS_D2 PTB9
#define YOTTA_CFG_HARDWARE_PINS_D3 PTA1
#define YOTTA_CFG_HARDWARE_PINS_D4 PTB23
#define YOTTA_CFG_HARDWARE_PINS_D5 PTA2
#define YOTTA_CFG_HARDWARE_PINS_D6 PTC2
#define YOTTA_CFG_HARDWARE_PINS_D7 PTC3
#define YOTTA_CFG_HARDWARE_PINS_D8 PTA0
#define YOTTA_CFG_HARDWARE_PINS_D9 PTC4
#define YOTTA_CFG_HARDWARE_PINS_D10 PTD0
#define YOTTA_CFG_HARDWARE_PINS_D11 PTD2
#define YOTTA_CFG_HARDWARE_PINS_D12 PTD3
#define YOTTA_CFG_HARDWARE_PINS_D13 PTD1
#define YOTTA_CFG_HARDWARE_PINS_D14 PTE25
#define YOTTA_CFG_HARDWARE_PINS_D15 PTE24
#define YOTTA_CFG_HARDWARE_PINS_I2C_SCL D15
#define YOTTA_CFG_HARDWARE_PINS_I2C_SDA D14
#define YOTTA_CFG_HARDWARE_PINS_A0 PTB2
#define YOTTA_CFG_HARDWARE_PINS_A1 PTB3
#define YOTTA_CFG_HARDWARE_PINS_A2 PTB10
#define YOTTA_CFG_HARDWARE_PINS_A3 PTB11
#define YOTTA_CFG_HARDWARE_PINS_A4 PTC10
#define YOTTA_CFG_HARDWARE_PINS_A5 PTC11
#define YOTTA_CFG_HARDWARE_PINS_DAC0_OUT 0xFEFE
#define YOTTA_CFG_HARDWARE_TEST_PINS
#define YOTTA_CFG_HARDWARE_TEST_PINS_SPI
#define YOTTA_CFG_HARDWARE_TEST_PINS_SPI_MOSI PTD2
#define YOTTA_CFG_HARDWARE_TEST_PINS_SPI_MISO PTD3
#define YOTTA_CFG_HARDWARE_TEST_PINS_SPI_SCLK PTD1
#define YOTTA_CFG_HARDWARE_TEST_PINS_SPI_SSEL PTD0
#define YOTTA_CFG_HARDWARE_TEST_PINS_I2C
#define YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SDA PTE25
#define YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SCL PTE24
#define YOTTA_CFG_HARDWARE_TEST_PINS_SERIAL
#define YOTTA_CFG_HARDWARE_TEST_PINS_SERIAL_TX PTC17
#define YOTTA_CFG_HARDWARE_TEST_PINS_SERIAL_RX PTD2
#define YOTTA_CFG_UVISOR
#define YOTTA_CFG_UVISOR_PRESENT 1
#define YOTTA_CFG_ARCH
#define YOTTA_CFG_ARCH_ARM
#define YOTTA_CFG_MBED

// version definitions
#define YOTTA_MBED_CLIENT_VERSION_STRING "1.2.16"
#define YOTTA_MBED_CLIENT_VERSION_MAJOR 1
#define YOTTA_MBED_CLIENT_VERSION_MINOR 2
#define YOTTA_MBED_CLIENT_VERSION_PATCH 16
#define YOTTA_MBED_CONNECTOR_INTERFACE_VERSION_STRING "1.0.0"
#define YOTTA_MBED_CONNECTOR_INTERFACE_VERSION_MAJOR 1
#define YOTTA_MBED_CONNECTOR_INTERFACE_VERSION_MINOR 0
#define YOTTA_MBED_CONNECTOR_INTERFACE_VERSION_PATCH 0
#define YOTTA_MBED_ENDPOINT_NETWORK_ETH_VERSION_STRING "0.1.0"
#define YOTTA_MBED_ENDPOINT_NETWORK_ETH_VERSION_MAJOR 0
#define YOTTA_MBED_ENDPOINT_NETWORK_ETH_VERSION_MINOR 1
#define YOTTA_MBED_ENDPOINT_NETWORK_ETH_VERSION_PATCH 0
#define YOTTA_MBED_CLIENT_C_VERSION_STRING "1.1.2"
#define YOTTA_MBED_CLIENT_C_VERSION_MAJOR 1
#define YOTTA_MBED_CLIENT_C_VERSION_MINOR 1
#define YOTTA_MBED_CLIENT_C_VERSION_PATCH 2
#define YOTTA_MBED_CLIENT_MBED_OS_VERSION_STRING "1.1.1"
#define YOTTA_MBED_CLIENT_MBED_OS_VERSION_MAJOR 1
#define YOTTA_MBED_CLIENT_MBED_OS_VERSION_MINOR 1
#define YOTTA_MBED_CLIENT_MBED_OS_VERSION_PATCH 1
#define YOTTA_MBED_CLIENT_MBEDTLS_VERSION_STRING "1.0.11"
#define YOTTA_MBED_CLIENT_MBEDTLS_VERSION_MAJOR 1
#define YOTTA_MBED_CLIENT_MBEDTLS_VERSION_MINOR 0
#define YOTTA_MBED_CLIENT_MBEDTLS_VERSION_PATCH 11
#define YOTTA_NANOSTACK_LIBSERVICE_VERSION_STRING "3.0.9"
#define YOTTA_NANOSTACK_LIBSERVICE_VERSION_MAJOR 3
#define YOTTA_NANOSTACK_LIBSERVICE_VERSION_MINOR 0
#define YOTTA_NANOSTACK_LIBSERVICE_VERSION_PATCH 9
#define YOTTA_MBED_DRIVERS_VERSION_STRING "0.11.6"
#define YOTTA_MBED_DRIVERS_VERSION_MAJOR 0
#define YOTTA_MBED_DRIVERS_VERSION_MINOR 11
#define YOTTA_MBED_DRIVERS_VERSION_PATCH 6
#define YOTTA_SOCKETS_VERSION_STRING "1.1.0"
#define YOTTA_SOCKETS_VERSION_MAJOR 1
#define YOTTA_SOCKETS_VERSION_MINOR 1
#define YOTTA_SOCKETS_VERSION_PATCH 0
#define YOTTA_MBED_HAL_VERSION_STRING "1.2.0"
#define YOTTA_MBED_HAL_VERSION_MAJOR 1
#define YOTTA_MBED_HAL_VERSION_MINOR 2
#define YOTTA_MBED_HAL_VERSION_PATCH 0
#define YOTTA_CMSIS_CORE_VERSION_STRING "1.1.0"
#define YOTTA_CMSIS_CORE_VERSION_MAJOR 1
#define YOTTA_CMSIS_CORE_VERSION_MINOR 1
#define YOTTA_CMSIS_CORE_VERSION_PATCH 0
#define YOTTA_UALLOC_VERSION_STRING "1.0.3"
#define YOTTA_UALLOC_VERSION_MAJOR 1
#define YOTTA_UALLOC_VERSION_MINOR 0
#define YOTTA_UALLOC_VERSION_PATCH 3
#define YOTTA_MINAR_VERSION_STRING "1.0.1"
#define YOTTA_MINAR_VERSION_MAJOR 1
#define YOTTA_MINAR_VERSION_MINOR 0
#define YOTTA_MINAR_VERSION_PATCH 1
#define YOTTA_CORE_UTIL_VERSION_STRING "1.0.1"
#define YOTTA_CORE_UTIL_VERSION_MAJOR 1
#define YOTTA_CORE_UTIL_VERSION_MINOR 0
#define YOTTA_CORE_UTIL_VERSION_PATCH 1
#define YOTTA_COMPILER_POLYFILL_VERSION_STRING "1.2.1"
#define YOTTA_COMPILER_POLYFILL_VERSION_MAJOR 1
#define YOTTA_COMPILER_POLYFILL_VERSION_MINOR 2
#define YOTTA_COMPILER_POLYFILL_VERSION_PATCH 1
#define YOTTA_MBED_HAL_FREESCALE_VERSION_STRING "1.0.0"
#define YOTTA_MBED_HAL_FREESCALE_VERSION_MAJOR 1
#define YOTTA_MBED_HAL_FREESCALE_VERSION_MINOR 0
#define YOTTA_MBED_HAL_FREESCALE_VERSION_PATCH 0
#define YOTTA_MBED_HAL_KSDK_MCU_VERSION_STRING "1.0.6"
#define YOTTA_MBED_HAL_KSDK_MCU_VERSION_MAJOR 1
#define YOTTA_MBED_HAL_KSDK_MCU_VERSION_MINOR 0
#define YOTTA_MBED_HAL_KSDK_MCU_VERSION_PATCH 6
#define YOTTA_UVISOR_LIB_VERSION_STRING "1.0.11"
#define YOTTA_UVISOR_LIB_VERSION_MAJOR 1
#define YOTTA_UVISOR_LIB_VERSION_MINOR 0
#define YOTTA_UVISOR_LIB_VERSION_PATCH 11
#define YOTTA_MBED_HAL_K64F_VERSION_STRING "1.0.1"
#define YOTTA_MBED_HAL_K64F_VERSION_MAJOR 1
#define YOTTA_MBED_HAL_K64F_VERSION_MINOR 0
#define YOTTA_MBED_HAL_K64F_VERSION_PATCH 1
#define YOTTA_CMSIS_CORE_FREESCALE_VERSION_STRING "1.0.0"
#define YOTTA_CMSIS_CORE_FREESCALE_VERSION_MAJOR 1
#define YOTTA_CMSIS_CORE_FREESCALE_VERSION_MINOR 0
#define YOTTA_CMSIS_CORE_FREESCALE_VERSION_PATCH 0
#define YOTTA_CMSIS_CORE_K64F_VERSION_STRING "1.0.0"
#define YOTTA_CMSIS_CORE_K64F_VERSION_MAJOR 1
#define YOTTA_CMSIS_CORE_K64F_VERSION_MINOR 0
#define YOTTA_CMSIS_CORE_K64F_VERSION_PATCH 0
#define YOTTA_MBED_HAL_FRDM_K64F_VERSION_STRING "1.0.0"
#define YOTTA_MBED_HAL_FRDM_K64F_VERSION_MAJOR 1
#define YOTTA_MBED_HAL_FRDM_K64F_VERSION_MINOR 0
#define YOTTA_MBED_HAL_FRDM_K64F_VERSION_PATCH 0
#define YOTTA_DLMALLOC_VERSION_STRING "1.0.0"
#define YOTTA_DLMALLOC_VERSION_MAJOR 1
#define YOTTA_DLMALLOC_VERSION_MINOR 0
#define YOTTA_DLMALLOC_VERSION_PATCH 0
#define YOTTA_MINAR_PLATFORM_VERSION_STRING "1.0.0"
#define YOTTA_MINAR_PLATFORM_VERSION_MAJOR 1
#define YOTTA_MINAR_PLATFORM_VERSION_MINOR 0
#define YOTTA_MINAR_PLATFORM_VERSION_PATCH 0
#define YOTTA_MINAR_PLATFORM_MBED_VERSION_STRING "1.0.0"
#define YOTTA_MINAR_PLATFORM_MBED_VERSION_MAJOR 1
#define YOTTA_MINAR_PLATFORM_MBED_VERSION_MINOR 0
#define YOTTA_MINAR_PLATFORM_MBED_VERSION_PATCH 0
#define YOTTA_SAL_VERSION_STRING "1.1.1"
#define YOTTA_SAL_VERSION_MAJOR 1
#define YOTTA_SAL_VERSION_MINOR 1
#define YOTTA_SAL_VERSION_PATCH 1
#define YOTTA_SAL_STACK_LWIP_VERSION_STRING "1.1.0"
#define YOTTA_SAL_STACK_LWIP_VERSION_MAJOR 1
#define YOTTA_SAL_STACK_LWIP_VERSION_MINOR 1
#define YOTTA_SAL_STACK_LWIP_VERSION_PATCH 0
#define YOTTA_SAL_DRIVER_LWIP_K64F_ETH_VERSION_STRING "1.0.2"
#define YOTTA_SAL_DRIVER_LWIP_K64F_ETH_VERSION_MAJOR 1
#define YOTTA_SAL_DRIVER_LWIP_K64F_ETH_VERSION_MINOR 0
#define YOTTA_SAL_DRIVER_LWIP_K64F_ETH_VERSION_PATCH 2
#define YOTTA_SAL_IFACE_ETH_VERSION_STRING "1.0.1"
#define YOTTA_SAL_IFACE_ETH_VERSION_MAJOR 1
#define YOTTA_SAL_IFACE_ETH_VERSION_MINOR 0
#define YOTTA_SAL_IFACE_ETH_VERSION_PATCH 1
#define YOTTA_MBEDTLS_VERSION_STRING "2.2.0"
#define YOTTA_MBEDTLS_VERSION_MAJOR 2
#define YOTTA_MBEDTLS_VERSION_MINOR 2
#define YOTTA_MBEDTLS_VERSION_PATCH 0
#define YOTTA_MBED_ETHERNET_SAMPLE_VERSION_STRING "0.1.0"
#define YOTTA_MBED_ETHERNET_SAMPLE_VERSION_MAJOR 0
#define YOTTA_MBED_ETHERNET_SAMPLE_VERSION_MINOR 1
#define YOTTA_MBED_ETHERNET_SAMPLE_VERSION_PATCH 0
#endif // ndef __YOTTA_CONFIG_H__
