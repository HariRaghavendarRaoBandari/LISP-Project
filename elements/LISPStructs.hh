#ifndef LISPSTRUCTS_H
#define LISPSTRUCTS_H

#include <stdint.h>

/*
 * Values for the Key_ID field.
 */

#define KEY_ID_NONE 			0
#define KEY_ID_HMAC_SHA_1_96 	1 // defined in [RFC2404]
#define KEY_ID_HMAC_SHA_256_128 2 // defined in [RFC4868]


/*
 *		 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |            Key ID             |  Authentication Data Length   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      ~                     Authentication Data                       ~
 *  	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct LISPMapRegisterInnerHeader {
	unsigned int Key_Id : 16; //  16 bits but we use a mask here
	unsigned int Authentication_Data_Length : 16; // number of bytes for Authentication_Data field -- 16 bits but we use a mask here 
	unsigned int Authentication_Data : 32;  // uncomment and modify if needed
};

/*
 *		 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   |                          Record TTL                           |
 *  |   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  R   | Locator Count | EID mask-len  | ACT |A|      Reserved         |
 *  e   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  c   | Rsvd  |  Map-Version Number   |       EID-Prefix-AFI          |
 *  o   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  r   |                          EID-Prefix                           |
 *  d   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |  /|    Priority   |    Weight     |  M Priority   |   M Weight    |
 *  | L +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | o |        Unused Flags     |L|p|R|           Loc-AFI             |
 *  | c +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |  \|                             Locator                           |
 *  +-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

struct LISPLocatorHeader {
	unsigned int Priority : 8;
	unsigned int Weight : 8;
	unsigned int M_Priority : 8;
	unsigned int M_Weight : 8;
	unsigned int Unused_Flags : 13;
	unsigned int L : 1;
	unsigned int P : 1;
	unsigned int R : 1;
	unsigned int Loc_AFI : 16;
	unsigned int Locator : 32;
};

struct LISPRecordLocatorHeader{
	unsigned int Record_TTL : 32;
	unsigned int Locator_count : 8;
	unsigned int EID_mask_len : 8;
	unsigned int ACT : 3;
	unsigned int A : 1;
	unsigned int Reserved: 16;
	unsigned int Map_Version_number : 12;
	unsigned int EID_Prefix_AFI : 16;
	unsigned int EID_Prefix : 32; //32 bits (4 octets) pour ipv4, 128 bits (16 octets) pour ipv6
	struct LISPLocatorHeader loc;
};

struct LISPMapRegister {
        //struct LISPMapRegisterOuterHeader headerOuter;
        //uint32_t nonce1;
        //uint32_t nonce2;
		struct LISPMapRegisterInnerHeader headerInner;
		struct LISPRecordLocatorHeader recLoc;
} CLICK_SIZE_PACKED_ATTRIBUTE;

#endif
