#include "config.h"
#include <stdio.h>

typedef struct mp {
    union {
	void *call_ptr;
	long vtindex;
    } u;
    int delta;	 /* offset from base, used with multiple inheritance */
    int vflag;   /* used on archs where call_ptr might be legitmately odd */
} *method_pointer;

extern
void *get_this_ptr( method_pointer m, void *object_ptr)
{
    void *th;
    th = ((char *)object_ptr) + (m->delta / 2);
    return th;
}

extern
void *get_xfer_ptr( method_pointer m, void *object_ptr)
{
    void *cptr;
    cptr = m->u.call_ptr;
    printf("M vflag == %d, m->delta = %d, m->u.vtindex = %lx\n", m->vflag, m->delta, m->u.vtindex);
#ifdef NOTDEF
    if (m->vflag != 0) {  /* vflag indicates virtual on some archs*/
#elseif NOTDEF
    if ((m->delta & 0x1) == 1) {  /* odd delta indicates virtual on some archs*/
#else
    if ((m->u.vtindex & 0x1) == 1) {   /* METHOD_PTR IS ODD - virtual */
#endif
	int vtableindex = m->u.vtindex & -4;
	void **vtable = *((void***)object_ptr);
	printf("Virtual, vtableindex = %d\n", vtableindex);

	vtable = (void **)(((char*)vtable) + vtableindex);
	cptr = *vtable;
	printf("Returning cptr %lx\n", cptr);
    }
    return cptr;
}
