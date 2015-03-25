#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include "LISPGenMapRegister.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapRegister::LISPGenMapRegister() { }

LISPGenMapRegister::~LISPGenMapRegister() { }

int LISPGenMapRegister::configure(Vector<String>& conf, ErrorHandler *errh) {
	Vector<IPAddress> ips;

	if (Args(conf, this, errh).read_all("EID", IPAddressArg(), ips).complete() < 0)
		return -1;

	for (Vector<IPAddress>::iterator it = ips.begin(); it != ips.end(); it++) {

		if (it->empty()) // not a valid ip
			continue;

		_ip_vector.push_back(it->addr());
	}

	return _ip_vector.size();
}

Packet* LISPGenMapRegister::pull(int) {

	if (!_ip_vector.empty()) {
		/*
		 * If we have something in our vector, we create a packet for each EID in the vector.
		 * The EID is in the dst_ip_anno for the "next" elements.
		 */

		struct LISPMapRegister data;
		data.Type = LISP_H_TYPE_3;
		data.P = 0;
		data.Reserved_1 = 0;
		data.Reserved_2 = 0;
		data.Reserved_3 = 0;
		data.M = 0;
		data.Record_Count = 1;
		data.nonce1 = click_random();
		data.nonce2 = click_random();
		data.Key_Id = KEY_ID_NONE;
		data.Authentication_Data_Length = 0;


		WritablePacket *p = Packet::make(headroom, &data, sizeof(LISPMapRegister), 0);

		if (p == NULL) {
			click_chatter("[-] Packet creation failed!");
			return NULL;
		}

		/*
		 * When the annotation for the packet is set, the EID in
		 * _ip_vector is removed.
		 */
		p->set_anno_u32(USER_ANNO_EID, IPAddress(*_ip_vector.begin()));
		_ip_vector.erase(_ip_vector.begin());

		return p;

	} else // If the EID vector is empty our job is done
		return NULL;
}

enum { H_EID_VECTOR }; // This will be vparam in handler's callbacks

String LISPGenMapRegister::read_callback(Element *e, void *vparam) {
	LISPGenMapRegister *mr = static_cast<LISPGenMapRegister *>(e);

	if ((intptr_t) vparam != H_EID_VECTOR)
		return "";

	if (mr->_ip_vector.empty())
		return String("<empty>");

	String res("");
	int i = 0;

	for (Vector<int>::iterator it = mr->_ip_vector.begin(); it; it++, i++) {
		res += "\n[";
		res += i;
		res += "] = ";
		res += IPAddress(*it).addr();
	}

	return res;
}

int LISPGenMapRegister::write_callback(const String &s, Element *e, void *vparam, ErrorHandler *errh) {
	LISPGenMapRegister *mr = static_cast<LISPGenMapRegister *>(e);

	if ((intptr_t) vparam != H_EID_VECTOR)
		return 0;

	IPAddress new_eid(s);

	if (new_eid.empty()) // if s isn't an ip, we fail gracefully
		return errh->error("Bad ip! Only one ip at the time is allowed");

	mr->_ip_vector.push_back(new_eid.addr());
	return 0;
}

void LISPGenMapRegister::add_handlers() {
	add_read_handler("eid_vector", read_callback, H_EID_VECTOR, Handler::CALM);
	add_write_handler("eid_vector", write_callback, H_EID_VECTOR);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegister)
