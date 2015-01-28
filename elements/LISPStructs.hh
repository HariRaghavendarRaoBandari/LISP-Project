#ifndef LISPSTRUCTS_H
#define LISPSTRUCTS_H

/*
 * Let's use bit-fields for specific lenght data:
 * http://www.tutorialspoint.com/cprogramming/c_bit_fields.htm
 */

/*
 * Values for Type field.
 */
#define LISP_H_TYPE_0 0	// Reserved
#define LISP_H_TYPE_1 1	// Map-Request
#define LISP_H_TYPE_2 2	// Map-Reply
#define LISP_H_TYPE_3 3	// Map-Register
#define LISP_H_TYPE_4 4	// Map-Notify
#define LISP_H_TYPE_8 8	// Encapsulated Control Message

/*
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Type=3 |P|            Reserved               |M| Record Count  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct LISPMapRegisterOuterHeader {
	struct { unsigned value : 4; } Type;
	bool P;
	struct { unsigned value : 18; } Reserved;
	bool M;
	uint8_t Record_Count;
};

#endif
