#ifndef STREAM_ABS_HEADER_H
#define STREAM_ABS_HEADER_H

#include "base_definition.h"

namespace vogg
{
	class Stream
	{

	protected:
		Stream();
		~Stream();

	public:
		/*  */
		virtual size_t stream_read(void* tobuffer, size_t buffersize, size_t readamount) = 0;

		/*  */
		virtual size_t stream_write(const void* frombuffer, size_t writesize) = 0;


	};
}

#endif
