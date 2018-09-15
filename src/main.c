#include <mach/mach_types.h>
#include <libkern/libkern.h>

#ifdef __kextname__
extern kern_return_t _start(kmod_info_t *, void *);
extern kern_return_t _stop(kmod_info_t *, void *);

/* Will expand name if it's a macro */
#define KMOD_EXPLICIT_DECL2(name, ver, start, stop) \
	KMOD_EXPLICIT_DECL(name, ver, start, stop)

KMOD_EXPLICIT_DECL2(BUNDLEID, KEXTBUILD_S, _start, _stop)
/* This kext have no standard entry/exitry  acts as kext library */
__private_extern__ kmod_start_func_t *_realmain = NULL;
__private_extern__ kmod_stop_func_t *_antimain = NULL;
__private_extern__ int _kext_apple_cc = __APPLE_CC__;
#endif

