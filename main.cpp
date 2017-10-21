#include <stdlib.h>
#include "conhash.h"
#include "def.h"
#define DATA_NUM 10000
#define REPLICAS 1000

using namespace conhash;

uint64_t BKDRHash(const string& str)
{
	uint64_t seed = 131; // 31 131 1313 13131 131313 etc..
	uint64_t hash = 0;
	for (size_t i = 0; i < str.length(); i++)
	{
		hash = (hash * seed) + str.at(i);
	}
	return hash;
}


int main()
{
	HashNode node1,node2,node3;
	node1.node_name = "ip1";
	node1.replicas = REPLICAS;
	node2.node_name = "ip2";
	node2.replicas = REPLICAS;
	node3.node_name = "ip3";
	node3.replicas = REPLICAS;

	CConhash hashctrller(&BKDRHash);
	//CConhash hashctrller;
	hashctrller.add_node(node1);
	hashctrller.add_node(node2);
	hashctrller.add_node(node3);
	
	map<string, uint32_t> statics;
	map<string, string> mobility;
	map<string, string> mobility2;
	//hashctrller.show_virtual_nodes();
	for (int i=0;i<DATA_NUM;i++)
	{
		string hash_seed = "James.km" + to_str(i);
		HashNode ret_node = hashctrller.conhash(hash_seed);
		//printf("ccc in %s\n", ret_node.node_name.c_str());
		string& nodekey = ret_node.node_name;
		if (statics.find(nodekey) != statics.end())
		{
			statics[nodekey]++;
		}
		else
		{
			statics.insert(make_pair(nodekey, 0));
		}

		mobility[hash_seed] = nodekey;
	}
	
	printf("virtal node count %d\n", hashctrller.virtual_node_num());
	for (auto &node:statics)
	{
		printf("node %s,count %u\n",node.first.c_str(), node.second);
	}

	//printf("del one node\n");
	printf("add one node\n");
	statics.clear();
	//hashctrller.del_node(node1);
	HashNode node4;
	node4.node_name = "ip4";
	node4.replicas = REPLICAS;
	hashctrller.add_node(node4);
	for (int i = 0; i < DATA_NUM; i++)
	{
		string hash_seed = "James.km" + to_str(i);
		HashNode ret_node = hashctrller.conhash(hash_seed);
		//printf("ccc in %s\n", ret_node.node_name.c_str());
		string& nodekey = ret_node.node_name;
		if (statics.find(nodekey) != statics.end())
		{
			statics[nodekey]++;
		}
		else
		{
			statics.insert(make_pair(nodekey, 0));
		}
		mobility2[hash_seed] = nodekey;
	}

	for (auto &node : statics)
	{
		printf("node %s,count %u\n", node.first.c_str(), node.second);
	}
	printf("virtal node count %d\n", hashctrller.virtual_node_num());

	uint32_t total = 0;
	uint32_t succ = 0;
	for (auto & seed1: mobility)
	{
		string hash_seed = seed1.first;
		string nodekey = seed1.second;
		if (mobility2[hash_seed]==nodekey)
		{
			succ++;
		}
		total++;
	}
	printf("succ %u,total %u\n", succ, total);
	//hashctrller.show_virtual_nodes();
	system("pause");
	return 0;
}