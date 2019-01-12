/**
 * Minimalist's kernel extension library
 *
 * Created 18H26  lynnl
 */

#ifndef LIBKEXT_H
#define LIBKEXT_H

#include <sys/types.h>

#ifndef __kext_makefile__
#define KEXTNAME_S		"TOFILL"
#endif

#define readonly_extern     extern  /* Cheap annotation */

/*
 * Used to indicate unused function parameters
 * see: <sys/cdefs.h>#__unused
 */
#define UNUSED(arg0, ...)   (void) ((void) arg0, ##__VA_ARGS__)

/*
 * G for GCC-specific
 * see:
 *  <sys/param.h>#MIN, MAX
 *  <libkern/libkern.h>#min, max
 */
#define GMIN(a, b) ({       \
    __typeof(a) _a = (a);   \
    __typeof(b) _b = (b);   \
    _a < _b ? _a : _b;      \
})

#define GMAX(a, b) ({       \
    __typeof(a) _a = (a);   \
    __typeof(b) _b = (b);   \
    _a > _b ? _a : _b;      \
})

#define ARRAY_SIZE(a)       (sizeof(a) / sizeof(*a))

/**
 * os_log() is only available on macOS 10.12 or newer
 *  thus os_log do have compatibility issue  use printf instead
 *
 * XNU kernel version of printf() don't recognize some rarely used specifiers
 *  like h, i, j, t  use unrecognized spcifier may raise kernel panic
 *
 * Feel free to print NULL as %s  it checked explicitly by kernel-printf
 *
 * see: xnu/osfmk/kern/printf.c#printf
 */
#define LOG(fmt, ...)        printf(KEXTNAME_S ": " fmt "\n", ##__VA_ARGS__)

#define LOG_INF(fmt, ...)    LOG("INF " fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...)    LOG("ERR " fmt, ##__VA_ARGS__)
#define LOG_BUG(fmt, ...)    LOG("BUG " fmt, ##__VA_ARGS__)
#define LOG_OFF(fmt, ...)    (void) (0, ##__VA_ARGS__)
#ifdef DEBUG
#define LOG_DBG(fmt, ...)    LOG("DBG " fmt, ##__VA_ARGS__)
#else
#define LOG_DBG(fmt, ...)    LOG_OFF(fmt, ##__VA_ARGS__)
#endif

#define panicf(fmt, ...)                \
    panic("\n" fmt "\n%s@%s#L%d\n\n",   \
            ##__VA_ARGS__, __BASE_FILE__, __FUNCTION__, __LINE__)

#ifdef DEBUG
/*
 * NOTE: Do NOT use any multi-nary conditional/logical operator inside assertion
 *       like operators && || ?:  it's extremely EVIL
 *       Separate them  each statement per line
 */
#define kassert(ex) (ex) ? (void) 0 : panicf("Assert `%s' failed", #ex)

/**
 * @ex      the expression
 * @fmt     panic message format
 *
 * Example: kassertf(sz > 0, "Why size %zd nonpositive?", sz);
 */
#define kassertf(ex, fmt, ...) \
    (ex) ? (void) 0 : panicf("Assert `%s' failed: " fmt, #ex, ##__VA_ARGS__)
#else
#define kassert(ex) (ex) ? (void) 0 : LOG_BUG("Assert `%s' failed", #ex)

#define kassertf(ex, fmt, ...) \
    (ex) ? (void) 0 : LOG_BUG("Assert `%s' failed: " fmt, #ex, ##__VA_ARGS__)
#endif

#define kassert_nonnull(ptr) kassert(((void *) ptr) != NULL)

/**
 * Branch predictions
 * see: linux/include/linux/compiler.h
 */
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

/**
 * Compile-time assertion  see: linux/arch/x86/boot/boot.h
 */
#ifdef DEBUG
#define BUILD_BUG_ON(cond)      UNUSED(sizeof(char[-!(cond)]))
#else
#define BUILD_BUG_ON(cond)      UNUSED(cond)
#endif

/**
 * Capital Q stands for quick
 * XXX: should only used for `char[]'  NOT `char *'
 */
#define QSTRLEN(s)          (sizeof(s) - 1)

void *libkext_malloc(size_t, int);
void *libkext_realloc(void *, size_t, size_t, int);
void libkext_mfree(void *);
void libkext_massert(void);

int libkext_get_kcb(void);
int libkext_put_kcb(void);
int libkext_read_kcb(void);
void libkext_invalidate_kcb(void);

int libkext_vma_uuid(vm_address_t, uuid_string_t);
void libkext_format_uuid_string(const uuid_t, uuid_string_t);

int libkext_file_read(const char *, unsigned char *, size_t, off_t, size_t *);

/**
 * kern_os_* family provides zero-out memory allocation
 * see: xnu/libkern/c++/OSRuntime.cpp
 */
extern void *kern_os_malloc(size_t);
extern void kern_os_free(void *);
extern void *kern_os_realloc(void *, size_t);

#endif /* LIBKEXT_H */

