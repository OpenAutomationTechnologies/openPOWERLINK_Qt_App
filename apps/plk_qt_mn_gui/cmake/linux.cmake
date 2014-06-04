################################################################################
# libPCAP specific configurations
################################################################################
IF(CFG_KERNEL_STACK_DIRECTLINK OR CFG_KERNEL_STACK_USERSPACE_DAEMON)
    SET(PCAP_LIBRARIES pcap)
ENDIF()

SET(OTHER_DEPENDENT_LIBS pthread rt)

IF (CFG_KERNEL_STACK_DIRECTLINK OR CFG_KERNEL_STACK_USERSPACE_DAEMON)
    ADD_DEFINITIONS (-DCONFIG_USE_PCAP)
ENDIF (CFG_KERNEL_STACK_DIRECTLINK OR CFG_KERNEL_STACK_USERSPACE_DAEMON)