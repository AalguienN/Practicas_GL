#include <codebase.h>
#include <iostream>

using namespace cb;
namespace aux {
	Vec3 randomVec() {

		return Vec3(random(-1, 1), random(-1, 1), random(-1, 1));
	}
}