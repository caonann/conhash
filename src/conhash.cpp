#include "conhash.h"
using namespace conhash;

bool CConhash::add_node(HashNode& node)
{
	if (!(node.flag&NODE_FLAG_INIT) || (node.flag&NODE_FLAG_IN))
	{
		printf("node init failed\n");
		return false;
	}
	node.flag |= NODE_FLAG_IN;

	//calc virtual nodes
	string& key = node.node_name;
	const uint32_t replicas = node.replicas;

	map<string, HashNode>::iterator it = _real_nodes.find(key);
	if (it != _real_nodes.end())
	{
		printf("node already existed %s\n", key.c_str());
		return false;
	}
	else
	{
		_real_nodes.insert(make_pair(key, node));
	}

	vector<string> vec_virtual_nodes;
	for (int i=0;i<replicas;i++)
	{
		string virtual_key = key + to_str(i);
		string str_hash_val = _phash_fun(virtual_key);

		//printf("nodename %s hash_val %llu %s\n", key.c_str(), hash_val, virtual_key.c_str());
		if (_virtual_nodes.find(str_hash_val) != _virtual_nodes.end())
		{
			printf("not null\n");
		}
		_virtual_nodes.insert(make_pair(str_hash_val, node));
		vec_virtual_nodes.push_back(str_hash_val);
		_virtual_num++;
	}
	
	//create real node mapping virtual nodes
	_real_map_virtal.insert(make_pair(key, vec_virtual_nodes));
	return true;
}

bool CConhash::del_node(HashNode& node)
{
	if (_real_nodes.empty() || !(node.flag&NODE_FLAG_INIT) || !(node.flag&NODE_FLAG_IN))
	{
		printf("del node failed \n");
		return false;
	}
	node.flag &= (~NODE_FLAG_IN);

	//del real nodes
	const string& key = node.node_name;
	uint32_t replicas = node.replicas;
	map<string, HashNode>::iterator it = _real_nodes.find(key);
	if ( it == _real_nodes.end() || it->second.replicas != replicas)
	{
		printf("del node failed not find key %s", key.c_str());
		return false;
	}
	_real_nodes.erase(it);

	//del virtual nodes
	for (int i = 0; i < replicas; i++)
	{
		string virtual_key = key + to_str(i);
		string str_hash_val = _phash_fun(virtual_key);

		//printf("nodename %s del hash_val %llu\n", key.c_str(), hash_val);
		_virtual_nodes.erase(str_hash_val);
		_virtual_num--;
	}

	return true;
}

HashNode CConhash::conhash(const string& hashkey)
{
	string hash = _phash_fun(hashkey);
	FILE *out = fopen("./conhash.txt", "a+");
	if (NULL == out)
	{
		printf("openfile failed \n");
		return HashNode();
	}
	fprintf(out, "%s\n", hash.c_str());
	fclose(out);
	out = NULL;
	map<string, HashNode>::iterator it = _virtual_nodes.lower_bound(to_str(hash));
	if (it != _virtual_nodes.end())
	{
		return it->second;
	}
	else
	{
		--it;
		return it->second;
	}
}

string CConhash::def_hash_fun(const string& key)
{
	int i;
	long hash = 0;
	char digest[16];
	conhash_md5_digest(key.c_str(), digest);

	/* use successive 4-bytes from hash as numbers */
	for (i = 0; i < 4; i++)
	{
		hash += ((long)(digest[i * 4 + 3] & 0xFF) << 24)
			| ((long)(digest[i * 4 + 2] & 0xFF) << 16)
			| ((long)(digest[i * 4 + 1] & 0xFF) << 8)
			| ((long)(digest[i * 4 + 0] & 0xFF));
	}

	return to_str(hash);
}
