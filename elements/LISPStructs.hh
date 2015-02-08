#ifndef LISPSTRUCTS_H
#define LISPSTRUCTS_H

/*
 * Values for the Key_ID field.
 */

#define KEY_ID_NONE 			0x00000000
#define KEY_ID_HMAC_SHA_1_96 	0x00000100 // (00 01 00 00 => 00000100) defined in [RFC2404]
#define KEY_ID_HMAC_SHA_256_128 0x00000200 // (00 02 00 00 => 00000200) defined in [RFC4868]

/*
 *		 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |            Key ID             |  Authentication Data Length   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      ~                     Authentication Data                       ~
 *  	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct LISPMapRegisterInnerHeader {
	unsigned int Key_Id ; //  16 bits but we use a mask here
	unsigned int Authentication_Data_Length; // number of bytes for Authentication_Data field -- 16 bits but we use a mask here 
	unsigned int Authentication_Data;  // uncomment and modify if needed
};

struct LISPMapRegister {
        //struct LISPMapRegisterOuterHeader headerOuter;
        //uint32_t nonce1;
        //uint32_t nonce2;
		struct LISPMapRegisterInnerHeader headerInner;
} CLICK_SIZE_PACKED_ATTRIBUTE;

#endif
