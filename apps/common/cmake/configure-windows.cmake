SET(CONFIG_openPOWERLINK_BUILD_TYPE ON CACHE INTERNAL
    "Link kernel stack directly into application (Single process solution)")
UNSET(CFG_KERNEL_STACK_USERSPACE_DAEMON CACHE)
