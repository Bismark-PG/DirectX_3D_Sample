/*==============================================================================

   Make Debug Ostream [debug_ostream.h]

==============================================================================*/
#ifndef DEBUG_OSTREAM_H
#define DEBUG_OSTREAM_H

#include <Windows.h>
#include <sstream>

using namespace std;

namespace hal
{
	class debugbuf : public basic_stringbuf<char, char_traits<char>>
	{
	public:
		virtual ~debugbuf()
		{
			sync();
		}
	protected:
		int sync()
		{
			OutputDebugStringA(str().c_str());
			str(basic_string<char>());
			return 0;
		}
	};

	class debug_ostream : public basic_ostream<char, char_traits<char>>
	{
	public:
		debug_ostream()
			: basic_ostream<char, char_traits<char>>(new debugbuf()) {}
		~debug_ostream() { delete rdbuf(); }
	};

	extern debug_ostream dout;
}
#endif // BASIC_DEBUG_OSTREAM_H