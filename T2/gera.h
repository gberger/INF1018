#define DEBUG_GERA 1

typedef int (*funcp) ();
void gera(FILE *f, void **code, funcp *entry);
void libera(void *code);
