#ifndef __NONCOPY__
#define __NONCOPY__
namespace conhash
{
	class Noncopyable {
	protected:
		Noncopyable() {}
		~Noncopyable() {}
	private:
		Noncopyable(const Noncopyable&);
		const Noncopyable& operator=(const Noncopyable&);
	};
}

#endif
