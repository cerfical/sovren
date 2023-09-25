#include "Keys.hpp"

namespace RENI {
	std::ostream& operator<<(std::ostream& out, Keys key) {
#define RENI_KEY(k) case Keys::k: out << "<" #k ">"; break;
		switch(key) {
			RENI_KEY_LIST
		}
#undef RENI_KEY
		return out;
	}
}