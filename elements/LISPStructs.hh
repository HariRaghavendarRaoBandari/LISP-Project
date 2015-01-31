#ifndef LISPSTRUCTS_H
#define LISPSTRUCTS_H

/*
 * Values for the Type field.
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
	unsigned short Type : 4; // 4 bits
	bool P;
	unsigned int Reserved; // 18 bits but we use a mask here
	bool M;
	unsigned int Record_Count; // 8 bits but we use a mask here
};

#endif
