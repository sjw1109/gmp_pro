

#ifndef _FILE_WINDOWS_SIMULINK_PERIPHERAL_H_
#define _FILE_WINDOWS_SIMULINK_PERIPHERAL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	typedef struct simulink_timer
	{
		// time from Simulink
		double simulink_time;

		// time from Windows
		SYSTEMTIME system_time;


	};






#ifndef __cplusplus
}
#endif // __cplusplus




#endif // _FILE_WINDOWS_SIMULINK_PERIPHERAL_H_


