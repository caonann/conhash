#ifndef __DEFNODE__
#define __DEFNODE__
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <sstream>

namespace conhash
{
#define NODE_FLAG_INIT  0x01 /* node is initialized */
#define NODE_FLAG_IN    0x02 /* node is added in the server */

	typedef struct hash_node
	{
		std::string node_name;
		uint32_t replicas; /* number of replica virtual nodes */
		uint32_t flag;
		hash_node()
		{
			replicas = 1;
			flag = NODE_FLAG_INIT;
		}
	}HashNode;

	template <typename T>
	inline T from_str(const std::string &str)
	{
		std::istringstream is(str);
		T t;
		is >> t;
		return t;
	}

	template <typename T>
	inline std::string to_str(const T &t)
	{
		std::ostringstream os;
		os << t;
		return os.str();
	}
}
#endif

