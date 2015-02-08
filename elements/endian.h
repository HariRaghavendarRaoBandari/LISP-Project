#include <stdint.h>

void convert(uint32_t *p, int nb){
	int i;

	for (i=0;i<nb;i++){
		*p = ((*p>>24)&0xff) | // move byte 3 to byte 0
		      ((*p<<8)&0xff0000) | // move byte 1 to byte 2
		      ((*p>>8)&0xff00) | // move byte 2 to byte 1
		      ((*p<<24)&0xff000000); // byte 0 to byte 3
		}
}