/* bb_aabb.h */

#ifndef BB_AABB_H
#define BB_AABB_H

#include "bb_param.h"
#include "core/object/object.h"

class BBAabb : public BBParam {
	GDCLASS(BBAabb, BBParam);

protected:
	virtual Variant::Type get_type() const override { return Variant::AABB; }
};

#endif // BB_AABB_H