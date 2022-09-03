/* bt_wait.cpp */

#include "bt_wait.h"
#include "core/class_db.h"
#include "core/object.h"
#include "core/variant.h"

String BTWait::_generate_name() const {
	return vformat("Wait %ss", duration);
}

void BTWait::_enter() {
	_time_passed = 0.0;
}

int BTWait::_tick(float p_delta) {
	_time_passed += p_delta;
	if (_time_passed < duration) {
		return RUNNING;
	} else {
		return SUCCESS;
	}
}

void BTWait::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_duration", "p_value"), &BTWait::set_duration);
	ClassDB::bind_method(D_METHOD("get_duration"), &BTWait::get_duration);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "duration"), "set_duration", "get_duration");
}