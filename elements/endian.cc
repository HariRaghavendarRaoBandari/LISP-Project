#include <stdint.h>
#include <click/config.h>
#include "endian.hh"

CLICK_DECLS

void convert(void *p, int nb){
	int i;
	uint32_t *pl = (uint32_t *)p;

	for (i=0;i<nb;i++){
		*(pl+i) = ((*(pl+i)>>24)&0xff) | // move byte 3 to byte 0
		      ((*(pl+i)<<8)&0xff0000) | // move byte 1 to byte 2
		      ((*(pl+i)>>8)&0xff00) | // move byte 2 to byte 1
		      ((*(pl+i)<<24)&0xff000000); // byte 0 to byte 3
		}
}

CLICK_ENDDECLS
ELEMENT_PROVIDES(Convert)
ELEMENT_REQUIRES(userlevel)