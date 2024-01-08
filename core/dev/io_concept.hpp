// This file provide a set of IO operation concept.
// User may use these concept of function to complete your operation.


#ifndef _FILE_IO_CONCEPT_HPP_
#define _FILE_IO_CONCEPT_HPP_

template<typename T>
class gmp_concept_traits
{
public:
	typedef typename T::data_type data_type;
};

//////////////////////////////////////////////////////////////////////////
// Direct read & write peripheral 
template<typename data_type>
class concept_write_direct
{
public:
	virtual gmp_size_t write(data_type* data, gmp_size_t length);
};

typedef concept_write_direct<gmp_data_t> gmp_concept_write_direct;

template<typename data_type>
class concept_read_direct
{
public:
	virtual gmp_size_t read(data_type* data, gmp_size_t length);
};

typedef concept_read_direct<gmp_data_t> gmp_concept_read_direct;

template<typename data_type>
class concept_rw_direct
	:public concept_read_direct<data_type>,
	public concept_write_direct<data_type>
{};

typedef concept_rw_direct<gmp_data_t> gmp_concept_rw_direct;

template<typename data_type>
class concept_readwrite_direct
{
public:
	virtual gmp_size_t readwrite(data_type* data_in, data_type* data_out, gmp_size_t length);
};

typedef concept_readwrite_direct <gmp_data_t> gmp_concept_readwrite_direct;

template<typename data_type>
class concept_rwd_direct
	:public concept_rw_direct<data_type>,
	public concept_readwrite_direct<data_type>
{};

typedef concept_rwd_direct<gmp_data_t> gmp_concept_rwd_direct;

//////////////////////////////////////////////////////////////////////////
template<typename addr_type, typename data_type>
class concept_read_with_addr
{
public:
	virtual gmp_size_t read(addr_type addr, data_type* data, gmp_size_t length);
};

typedef concept_read_with_addr<gmp_addr_t, gmp_data_t> gmp_concept_read_with_addr;

template<typename addr_type, typename data_type>
class concept_write_with_addr
{
public:
	virtual gmp_size_t write(addr_type addr, data_type* data, gmp_size_t length);
};

typedef concept_write_with_addr<gmp_addr_t, gmp_data_t> gmp_concept_write_with_addr;

template<typename addr_type, typename data_type>
class concept_rw_with_addr
	:public concept_read_with_addr<addr_type, data_type>,
	public concept_write_with_addr<addr_type, data_type>
{};

typedef concept_rw_with_addr<gmp_addr_t, gmp_data_t> gmp_concept_rw_with_addr;

//////////////////////////////////////////////////////////////////////////
template<typename addr1_type, typename addr2_type, typename data_type>
class concept_read_with_dualaddr
{
public:
	virtual gmp_size_t read(addr1_type addr1, addr2_type addr2, data_type* data, gmp_size_t length);
};

template<typename addr1_type, typename addr2_type, typename data_type>
class concept_write_with_dualaddr
{
public:
	virtual gmp_size_t write(addr1_type addr1, addr2_type addr2, data_type* data, gmp_size_t length);
};

template<typename addr1_type, typename addr2_type, typename data_type>
class concept_rw_with_dualaddr
	: public concept_read_with_dualaddr<addr1_type, addr2_type, data_type>,
	public concept_write_with_dualaddr <addr1_type, addr2_type, data_type>
{};

typedef concept_rw_with_dualaddr<gmp_addr_t, gmp_addr_t, gmp_data_t> gmp_concept_rw_with_dualaddr;

//////////////////////////////////////////////////////////////////////////
template<typename data_type>
class concept_read_single
{
public:
	virtual data_type read();
};

template<typename data_type>
class concept_write_single
{
public:
	virtual gmp_size_t write(data_type data);
};

template<typename data_type>
class concept_rw_single
	: public concept_read_single<data_type>,
	public concept_write_single<data_type>
{};

typedef concept_rw_single<gmp_data_t> gmp_concept_rw_single;

class gmp_concept_seek
{
public:
	virtual gmp_size_t seek(gmp_size_t pos);
};

template<typename data_type>
class concept_rws_single
	:public concept_rw_single<gmp_data_t>,
	public gmp_concept_seek
{};

typedef concept_rws_single<gmp_data_t> gmp_concept_rws_single;

template<typename data_type>
class concept_readwrite_single
{
public:
	virtual data_type readwrite(data_type data);
};

template<typename data_type>
class concept_rwd_single
	:public concept_rw_single<data_type>,
	public concept_readwrite_single<data_type>
{};

typedef concept_rwd_single<gmp_data_t> gmp_concept_rwd_single;

template<typename data_type>
class concept_rwds_single
	:public concept_rwd_single<data_type>,
	public gmp_concept_seek
{};

typedef concept_rwds_single<gmp_data_t> gmp_concept_rwds_single;

//////////////////////////////////////////////////////////////////////////
template<typename addr_type, typename data_type>
class concept_read_single_addr
{
public:
	virtual data_type read(addr_type addr);
};

template<typename addr_type, typename data_type>
class concept_write_single_addr
{
public:
	virtual gmp_size_t write(addr_type addr, data_type data);
};

template<typename addr_type, typename data_type>
class concept_wr_single_addr
	:public concept_write_single_addr<addr_type, data_type>,
	public concept_read_single_addr<addr_type, data_type>
{};

typedef concept_wr_single_addr<gmp_addr_t, gmp_data_t> gmp_concept_wr_single_addr;


//#include <functional>
//////////////////////////////////////////////////////////////////////////
// Adapter
// template<typename addr1_type, typename addr_type, typename data_type, addr1_type first_addr>
// class gmp_trait_dualaddr_reg
// 	:public concept_wr_single_addr<addr_type, data_type>
// {
// public:
// 	gmp_trait_dualaddr_reg(concept_rw_with_dualaddr<addr1_type, addr_type, data_type>&& client)
// 		:client(std::move(client))
// 	{
// 	}
// 
// protected:
// 	concept_rw_with_dualaddr<addr1_type, addr_type, data_type>& client;
// 
// public:
// 	data_type read(addr_type addr) override
// 	{
// 		data_type result;
// 		client.read(first_addr, addr, &result, 1);
// 		return result;
// 	}
// 
// 	gmp_size_t write(addr_type addr, data_type data) override
// 	{
// 		client.write(first_addr, addr, &data, 1);
// 		return 1;
// 	}
// 
// };



#endif // _FILE_IO_CONCEPT_HPP_
