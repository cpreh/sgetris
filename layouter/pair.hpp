#ifndef SGETRIS_LAYOUTER_PAIR_HPP_HPP_INCLUDED
#define SGETRIS_LAYOUTER_PAIR_HPP_HPP_INCLUDED

namespace sgetris
{
namespace layouter
{
template
<
	typename T,
	typename U
>
struct pair
{
public:
	pair()
	:
		master(),
		slave()
	{
	}

	pair(
		T const &master,
		U const &slave)
	:
		master(
			master),
		slave(
			slave)
	{
	}

	T master;
	U slave;
};
}
}

#endif
