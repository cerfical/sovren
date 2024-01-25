#include "Keys.hpp"

namespace RENI {
	std::ostream& operator<<(std::ostream& out, Keys k) {
#define RENI_KEY(k) case Keys::k: out << "<" #k ">"; break;
		switch(k) {
			RENI_KEY_LIST
		}
#undef RENI_KEY
		return out;
	}
}