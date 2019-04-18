#ifndef __CONHASH__
#define __CONHASH__
#include <map>

#include "def.h"
#include "CNonCopy.h"
#include "md5.h"
#include <vector>

using namespace std;

namespace conhash
{
	typedef string (*PHASH_FUN)(const string& key) ;

	class CConhash:public Noncopyable
	{
	public:
		explicit CConhash(PHASH_FUN pfun = nullptr) :_virtual_num(0), _phash_fun(nullptr)
		{
			if (pfun == nullptr)
			{
				printf("use def hash func\n");
				_phash_fun = def_hash_fun;
			}
			else
			{
				printf("use usr hash func\n");
				_phash_fun = pfun;
			}
		}

		bool add_node(HashNode& node);
		bool del_node(HashNode& node);
		HashNode conhash(const string& hashkey);

		inline uint32_t virtual_node_num()
		{
			return _virtual_num;
		}

		//for debug
		void show_virtual_nodes()
		{
			for(auto &it:_virtual_nodes)
			{
				HashNode& node = it.second;
				string virtualkey = it.first;
				printf("node name %s,virtualkey %s\n", node.node_name.c_str(), virtualkey.c_str());
			}			

			int i = 0;
			for (auto& it :_real_map_virtal)
			{
				string filename = it.first;
				vector<string>& veclist = it.second;
				FILE* out = fopen(filename.c_str(), "w+");
				for (auto& item :veclist)
				{
					//fprintf(out, "[%d,%s,%d],", i, it->c_str(), 1);
					fprintf(out, "%s\n", item.c_str());
				}
				fclose(out);
				i++;
			}
		}
	private:
		static string def_hash_fun(const string& key);
		static void conhash_md5_digest(const char *instr, char digest[16])
		{
			md5_state_t md5state;

			md5_init(&md5state);
			md5_append(&md5state, reinterpret_cast<const md5_byte_t *>(instr), strlen(instr));
			md5_finish(&md5state, reinterpret_cast<md5_byte_t *>(digest));
		}

	private:
		map<string, HashNode> _virtual_nodes;
		map<string,HashNode> _real_nodes;
		map<string, vector<string> > _real_map_virtal;
		uint32_t _virtual_num;
		PHASH_FUN _phash_fun;
	};
}
#endif
