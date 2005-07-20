#if !defined(KEYCONST_H)
#define KEYCONST_H

#define KEYFILE_MAXKEYLENGTH 512                /* maximum key length */
/* need machine specific values here */
/* win */
#if defined(WIN32) || defined(sparc)
#define min_fcb_lc 45872
#define buffer_lc 4144
#else /* linux */
#if __WORDSIZE == 64
/* linux 64 */
#define min_fcb_lc 45920
#define buffer_lc 4144
#else /* linux 32 */
#define min_fcb_lc 44192
#define buffer_lc 4128
#endif
#endif
#endif
