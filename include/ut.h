#ifndef _UT_
#define _UT_
#include "def.h"

namespace conhash
{
	void test_lookup(
		const string& key,
		CConhash &hashctrller,
		map<string, uint32_t>& statics,
		map<string, string>& mobility
	)
	{
		string hash_seed = key;
		HashNode ret_node = hashctrller.conhash(hash_seed);
		//printf("ccc in %s\n", ret_node.node_name.c_str());
		string& nodekey = ret_node.node_name;
		if (statics.find(nodekey) != statics.end())
		{
			statics[nodekey]++;
		}
		else
		{
			statics.insert(make_pair(nodekey, 1));
		}

		mobility[hash_seed] = nodekey;
	}
}
#endif
