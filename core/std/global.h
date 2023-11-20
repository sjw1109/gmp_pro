
#ifndef _FILE_GLOBAL_H
#define _FILE_GLOBAL_H



#ifdef __cplusplus
extern "C"
{
#endif

	// These function will be implemented by GMP
	void gmp_entry(void);
	void gmp_init_peripheral_tree(void);
	void gmp_setup_peripheral(void);
	void gmp_loop(void);
	void gmp_init(void);

	// User should implement this two functions 
	void user_init(void);
	void user_loop(void);

	// For test user should enable and implement the following function
	void gmp_test_bench(void);
#ifdef __cplusplus
}
#endif


#endif // _FILE_GLOBAL_H
