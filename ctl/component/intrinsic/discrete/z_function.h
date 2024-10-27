// BAD FILE!
// This file should be update, and will support MATLAB export headers.

/**
 * @file        discrete_control_system.h
 * @author      Javnson
 * @email       javnson@zju.edu.cn
 * @date        2022.03.18(Create:2022.03.07)
 * @version     1.0.0:0000
 * @licence     Apache License, Version 2.0
 *
 * @brief
 *
 * Change history :
 *   <Date>   | <version>  | <author> | <Description> |
 * 2022.03.07 | 1.0.0:0000 | Javnson  | Create File   |
 * 2022.03.10 | 1.0.1:0001 | Javnson  | Add discrete time for discrete system. |
 * 2022.03.27 | 1.0.1:0002 | Javnson  | Add dcs_param_t, dcs_uint_t, dcs_difference_t |
 *
 */

#include <emlib_setting.h>

#ifndef _FILE_DISCRETE_CONTROL_SYSTEM_H_
#define _FILE_DISCRETE_CONTROL_SYSTEM_H_

#ifdef __cplusplus
extern "C"
{
#endif

    // specific parameter type for the whole discrete control system.
#if defined DCS_PARAM_USE_FLOAT
    typedef float dcs_param_t;
#elif defined DCS_PARAM_USE_UINT8
typedef uint8_t dcs_param_t;
#elif defined DCS_PARAM_USE_UINT16
typedef uint16_t dcs_param_t;
#elif defined DCS_PARAM_USE_UINT32
typedef uint32_t dcs_param_t;
#elif defined DCS_PARAM_USE_INT8
typedef int8_t dcs_param_t;
#elif defined DCS_PARAM_USE_INT16
typedef int16_t dcs_param_t;
#elif defined DCS_PARAM_USE_INT32
typedef int32_t dcs_param_t;
#elif defined DCS_PARAM_USE_DOUBLE
typedef double dcs_param_t;
#elif defined DCS_PARAM_USE_MANUAL
typedef PID_PARAM_USE_MANUAL dcs_param_t;
#else
#pragma message(                                                                                                       \
    "Now the dcs_param_t type is default which is float type, please use macro \"DCS_PARAM_USE_MANUAL\" to define a manual type for yourself.")
typedef float dcs_param_t;
#endif

    // specify parameter type for the unit
#if defined DCS_UNIT_USE_FLOAT
    typedef float dcs_unit_t;
#elif defined DCS_PARAM_USE_UINT8
typedef uint8_t dcs_unit_t;
#elif defined DCS_UNIT_USE_UIINT16
typedef uint16_t dcs_uint_t;
#elif defined DCS_UNIT_USE_UIINT32
typedef uint32_t dcs_uint_t;
#elif defined DCS_UINT_USE_INT8
typedef int8_t dcs_uint_t;
#elif defined DCS_UINT_USE_INT16
typedef int16_t dcs_uint_t;
#elif defined DCS_UINT_USE_INT32
typedef int32_t dcs_param_t;
#elif defined DCS_UNIT_USE_DOUBLE
typedef double dcs_param_t;
#elif defined DCS_UNIT_USE_MANUAL
typedef DCS_UNIT_USE_MANUAL dcs_uint_t;
#else
#pragma message(                                                                                                       \
    "Now the dcs_unit_t type is default type, which is float type. You may use macro \"DCS_UNIT_USE_MANUAL\" to specify the unit type clearly.\n")
typedef float dcs_uint_t;
#endif

    // specify Measurement type of memory length
#if defined DCS_DIFFERENCE_USE_UINT16
    typedef uint16_t dcs_difference_t;
#elif defined DCS_DIFFERENCE_USE_UINT32
typedef uint32_t dcs_difference_t;
#elif defined DCS_DIFFERENCE_USE_UINT64
typedef uint64_t dcs_difference_t;
#elif defined DCS_DIFFERENCE_USE_MANUAL
typedef DCS_DIFFERENCE_USE_MANUAL dcs_difference_t;
#else
#pragma message(                                                                                                       \
    "Now the dcs_difference type is default type, which is uint32_t. You may use maccro\"DCS_DIFFERENCE_USE_MANUAL\" to specify the difference type.\n")
typedef uint32_t dcs_difference_t;
#endif

    /**
     * @brief Describe a discrete control system.
     */
    typedef struct _entity_discrete_control_system
    {
        /**
         * @brief This function will prepare whole discrete control system. You may cover the function if necessary. Or
         * you may just cover only transactional function.
         *
         * @param obj: the handle of the discrete control system
         *
         * @retval None
         *
         */
        void (*fn_calc)(struct _entity_discrete_control_system *obj);

        dcs_param_t m_discrete_time; ///< The discrete time for the system.
                                     /// @note you may set the param to 1, if you need a pure discrete system.

        dcs_param_t m_zero; ///< The zero point for the discrete system.

    } discrete_control_system, *pdiscrete_control_system;

#ifdef __cplusplus
}
#endif

#endif


/**
 * @file        z_trnasfer_function.h
 * @author      Javnson
 * @email       javnson@zju.edu.cn
 * @date        2022.03.27(Create:2022.03.25)
 * @version     1.0.0:0000
 * @licence     Apache License, Version 2.0
 *
 * @brief
 *
 * Change history :
 *   <Date>   | <version>  | <author> | <Description> |
 * 2022.03.25 | 1.0.0:0000 | Javnson  | Create File   |
 * 2022.03.27 | 1.0.1:0002 | Javnson  | Complete basic z function |
 * 2022.03.28 | 1.0.2:0003 | Javnson  | init_z_function_struct_mm and deinit_z_function_struct_mm |
 *
 */

/* Application Notes
 * Application Note: 3/27/2022
 * when you use BUFFER_POINTER_ENDLESS mode to ask the `fn_clac` routine to move pointer. you must pay
 * attention to the origin for the array. The first `cm_num_order` items of `p_input_buffer` are occupied
 * due to calculation needs.so as `cm_den_order` items of `p_output_buffer`.
 * You can also give the Z transfer function the initial condition after init routine, by set the
 * `z_function::p_input_buffer` and `z_function::p_output_buffer`.
 *
 * Application Note: 3/28/2022
 * Please notice that the order of the denominator and numerator must be carefully confirm!
 * If you don't set the correct coefficient or order, the result may be divergent or shaking.
 */

/* Demo routine
* Demo 1 Trinomial mean filter
* transfer function: $T(z)=\frac{1+z^{-1}+z^{-2}}{3}$
    discrete_control_system dcs;
    z_function z1;

    dcs.m_discrete_time = 0.001; // 10 kHz
    dcs.m_zero = 0.0f;

    z1.cm_num_order = 2;
    z1.cm_den_order = 0;

    float num[3] = { 1.0f, 1.0f, 1.0f };
    float den[1] = { 3.0f };

    z1.cm_num_param = num;
    z1.cm_den_param = den;

    float* input_array = new float[2 << 13];
    float* output_array = new float[2 << 13];

    z1.p_input_buffer = input_array;
    z1.p_output_buffer = output_array;

    z1.m_buffer_state = z1.BUFFER_POINTER_ENDLESS;

    init_z_function_struct(&z1, &dcs);

    for (size_t i = 0; i < 10000; i++)
    {
        z1.m_input = i;
        z1.fn_calc(&z1);
    } // set break point here.
* You may notice the mean filter's result at the break point.
*/

#include <discrete_control_system.h>

#ifndef _FILE_Z_TRNASFER_FUNCTION_H_
#define _FILE_Z_TRANSFER_FUCNTION_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief This object implements a physically realizable z-domain function.
     *  You may call the fn_calc to calculate the result for the Z domain.
     *  In general you have to design the Z function firstly, and simplify the equation to the following basic form.
     $$
      \begin{aligned}
        H(z) &= \frac{R(z)}{X(z)}=\frac{a_0+\Sigma_{p=1}^{m}{a_pz^{-p}}}{1+\Sigma_{q=1}^n{b_qz^{-q}}} \\
        r_n &= a_0x_n+\Sigma_{p=1}^{m}{a_px_{n-p}}-\Sigma_{q=1}^n{b_qr_{n-q}}
      \end{aligned}
     $$
     * You may read it by LaTex compiler, if necessary.
     * Than, based on the equation, the user must pass two vectors as the numerator
     *  and the denominator.
     * the entity provide a calculate function, named @fn_calc user may call the function, at the right time.
     *
     */
    typedef struct _entity_z_transfer_function
    {
        /**
         * @brief This function specify the calculate function.
         *        You may provide the input param and call the function to get the result.
         *
         * @param obj: input the Z-domain function object.
         *
         * @retval None
         *
         */
        void (*fn_calc)(struct _entity_z_transfer_function *obj);

        dcs_param_t m_input;
        dcs_param_t m_output;

        dcs_difference_t cm_num_order; // const member numerator order, refer to p, this value may be set to 0, if none
                                       // history information are needed.
        dcs_difference_t cm_den_order; // const member denominator order, refer to q, this value may be set to 0, if
                                       // none history information are needed.

        // these params are stored in decreasing order
        dcs_param_t
            *cm_num_param; // const member numerator
                           // The pointer point to an array that contain more than `cm_num_order + 1` items, as the
                           // numerator param. In general, the first param should be 1, in order to match the equation.
        dcs_param_t *cm_den_param; // const member denominator
                                   // The pointer point to an array that contain more than `cm_den_order + 1` items.

        dcs_param_t *p_input_buffer;  // The buffer must point to an array that contain more than `cm_num_order` items,
                                      // and only cm_num_order element will be used.
        dcs_param_t *p_output_buffer; // The buffer must point to an array that contain more than `cm_den_order` items,
                                      // and only cm_den_order element will be used.
                                      //
        // An example for 3-order system fixed system mode or endless system mode.
        // index : [-1]      [0]       [1]       [2]
        //         x_(-4)    x_(-3)    x_(-2)    x_(-1)
        //         r_(-4)    r_(-3)    r_(-2)    r_(-1)
        // warning: the run function won't check the boundary conditions. User must ensure the boundary condition is
        // reasonable. An example for 3-order system circle mode. Every oldest data will be replaced by the newest data.
        // index : [0]       [1]       [2]       [3]
        //   1st   x_(-4)    x_(-3)    x_(-2)    x_(-1)
        //         r_(-4)    r_(-3)    r_(-2)    r_(-1)
        // index : [0]       [1]       [2]       [3]
        //   2nd   x_(0)     x_(-3)    x_(-2)    x_(-1)
        //         r_(0)     r_(-3)    r_(-2)    r_(-1)
        // index : [0]       [1]       [2]       [3]
        //   3rd   x_(0)     x_(1)     x_(-2)    x_(-1)
        //         r_(0)     r_(1)     r_(-2)    r_(-1)
        enum
        {
            BUFFER_POINTER_FIXED =
                1, // The buffer label is fixed, buffer[0] is the last item(previous item for output),
                   //  buffer[1] is the previous item for buffer[0].
                   // That is, a circular copy is executed every time calculation (@fn_calc) is completed.
            BUFFER_POINTER_CIRCLE, // The calculation results will be stored in the buffer in a circular manner.
                                   //  The last input param will replace the earliest stored data.
                                   //  And the function will automatically match the label and index.
            BUFFER_POINTER_ENDLESS // The calculation result will stored in an endless array.
                                   //  The pointer will move to next item every time calculation is completed,
                                   //  and user must ensure the buffer has enough length, or move the pointer address
                                   //  manually.
        } m_buffer_state;

        dcs_difference_t p_cir_pos; // This variable record the position of the oldest item.
        struct _entity_discrete_control_system *m_dcs;

    } z_function, *pz_function;

    /**
     * @brief init the z transfer function entity struct.
     *
     * @param obj: the struct to be init.
     *		 dcs: the discrete control system the z_function module based on.
     *
     * @retval None
     * @note The function mast be call after all pointer was set.
     *
     */
    void init_z_function_struct(struct _entity_z_transfer_function *obj, struct _entity_discrete_control_system *dcs);

    /**
     * @brief init z transfer function entity struct and alloc memory automatically.
     *
     * @param obj: the struct to be init.
     *		 dcs: the discrete control system the z_function module based on.
     *
     * @retval None
     * @note	This function will call @malloc. you must ensure the function is enable in your production environment.
     *  Otherwise, you have no choice to use @init_z_function_struct and alloc memory manually.
     *  This function will only allocate memory for p_input_buffer and p_output_buffer in minimum size.
     * @warning The time you use this function to construct a z_function entity. You need to call
     */
    void init_z_function_struct_mm(struct _entity_z_transfer_function *obj,
                                   struct _entity_discrete_control_system *dcs);

    /**
     * @brief init z transfer function entity struct and alloc memory automatically.
     *
     * @param obj: the struct to be init.
     *		 dcs: the discrete control system the z_function module based on.
     *
     * @retval None
     * @note	This function will call @malloc. you must ensure the function is enable in your production environment.
     *  Otherwise, you have no choice to use @init_z_function_struct and alloc memory manually.
     *  This function will only allocate memory for p_input_buffer and p_output_buffer in minimum size.
     * @warning The time you use this function to construct a z_function entity. You need to call
     */
    void deinit_z_function_struct_mm(struct _entity_z_transfer_function *obj);

    /**
     * @brief This function is default function for the z transfer.
     *
     * @param obj: the z_transfer_function object.
     *
     * @retval None
     *
     */
    void _z_func_calc_default(struct _entity_z_transfer_function *obj);

#ifdef __cplusplus
}
#endif

#endif



/**
 * @file        z_trnasfer_function.c
 * @author      Javnson
 * @email       javnson@zju.edu.cn
 * @date        2022.03.27(Create:2022.03.26)
 * @version     1.0.0:0000
 * @licence     Apache License, Version 2.0
 *
 * @brief
 *
 * Change history :
 *   <Date>   | <version>  | <author> | <Description> |
 * 2022.03.26 | 1.0.0:0000 | Javnson  | Create File   |
 * 2022.03.27 | 1.0.1:0002 | Javnson  | Complete basic z function |
 *
 */

#include <discrete_control_system.h>
#include <memory.h>
#include <z_transfer_function.h>

void _z_func_calc_default(pz_function obj)
{
    cs_assert_error(obj->cm_num_param, "num_param must be a array and not point to nullptr");
    cs_assert_error(obj->cm_den_param, "den_param must be a array and not point to nullptr");
    cs_assert_error(obj->cm_den_param[0] != (dcs_param_t)0, "den_param[0] must be an nonzero value.");

    dcs_param_t r_n = obj->cm_num_param[0] * obj->m_input;

    // this switch ensure the judgment will not happen usually.
    switch (obj->m_buffer_state)
    {
    case BUFFER_POINTER_FIXED:
        for (dcs_difference_t i = 1; i <= obj->cm_num_order; ++i)
            // r_n += obj->cm_num_param[i] * obj->p_input_buffer[obj->cm_num_order - i];
            r_n += *(obj->cm_num_param + i) * (*(obj->p_input_buffer + obj->cm_num_order - i));
        for (dcs_difference_t i = 1; i <= obj->cm_den_order; ++i)
            // r_n -= obj->cm_den_param[i] * obj->p_output_buffer[obj->cm_den_order - i];
            r_n -= *(obj->cm_den_param + i) * (*(obj->p_output_buffer + obj->cm_den_order - i));

        // get the result.
        obj->m_output = r_n / obj->cm_den_param[0];

        // memory move.
        for (dcs_difference_t i = 1; i < obj->cm_num_order; ++i)
            obj->p_input_buffer[i - 1] = obj->p_input_buffer[i];
        if (obj->cm_num_order)
            obj->p_input_buffer[obj->cm_num_order - 1] = obj->m_input;

        for (dcs_difference_t i = 1; i < obj->cm_den_order; ++i)
            obj->p_output_buffer[i - 1] = obj->p_output_buffer[i];
        if (obj->cm_den_order)
            obj->p_output_buffer[obj->cm_den_order - 1] = obj->m_output;

        return;
    case BUFFER_POINTER_CIRCLE:
        for (dcs_difference_t i = 1; i <= obj->cm_num_order; ++i)
            // r_n += obj->cm_num_param[i] * obj->p_input_buffer[(obj->cm_num_order - i + obj->p_cir_pos) %
            // obj->cm_num_order];
            r_n += *(obj->cm_num_param + i) *
                   (*(obj->p_input_buffer + (obj->cm_num_order - i + obj->p_cir_pos) % obj->cm_num_order));

        for (dcs_difference_t i = 1; i <= obj->cm_den_order; ++i)
            // r_n -= obj->cm_den_param[i] * obj->p_output_buffer[(obj->cm_den_order - i + obj->p_cir_pos) %
            // obj->cm_den_order];
            r_n -= *(obj->cm_den_param + i) *
                   (*(obj->p_output_buffer + (obj->cm_den_order - i + obj->p_cir_pos) % obj->cm_den_order));

        // get the result.
        obj->m_output = r_n / obj->cm_den_param[0];

        // replace the oldest data
        if (obj->cm_num_order)
            obj->p_input_buffer[obj->p_cir_pos % obj->cm_num_order] = obj->m_input;
        if (obj->cm_den_order)
            obj->p_output_buffer[obj->p_cir_pos % obj->cm_den_order] = obj->m_output;

        // Keep good circulation
        if (++obj->p_cir_pos == obj->cm_den_order * obj->cm_num_order)
            obj->p_cir_pos = 0;

        return;
    case BUFFER_POINTER_ENDLESS:
        for (dcs_difference_t i = 1; i <= obj->cm_num_order; ++i)
            // r_n += obj->cm_num_param[i] * obj->p_input_buffer[obj->cm_num_order - i];
            r_n += *(obj->cm_num_param + i) * (*(obj->p_input_buffer + obj->cm_num_order - i));
        for (dcs_difference_t i = 1; i <= obj->cm_den_order; ++i)
            // r_n -= obj->cm_den_param[i] * obj->p_output_buffer[obj->cm_den_order - i];
            r_n -= *(obj->cm_den_param + i) * (*(obj->p_output_buffer + obj->cm_den_order - i));

        // get the result.
        obj->m_output = r_n / obj->cm_den_param[0];

        // Move to net position.
        *(++obj->p_input_buffer + obj->cm_num_order - 1) = obj->m_input;
        *(++obj->p_output_buffer + obj->cm_den_order - 1) = obj->m_output;

        return;
    default:
        cs_assert_error(0, "Unknown buffer style.");
    }
    return;
}

void init_z_function_struct(struct _entity_z_transfer_function *obj, struct _entity_discrete_control_system *dcs)
{
    cs_assert_error(obj, "You must pass an obj pointer to the z_function struct.\n");

    cs_assert_error(obj->cm_den_param, "You must specify the denominator param firstly.\n");
    cs_assert_error(obj->cm_num_param, "You must specify the numerator param firstly.\n");

    cs_assert_error(obj->cm_num_order == 0 || obj->p_input_buffer, "You must specify the input buffer.\n");
    cs_assert_error(obj->cm_den_order == 0 || obj->p_output_buffer, "You must specifu the output buffer.\n");

    cs_assert_error(
        dcs, "The z_function module is based on discrete control system, you must define and pass a dcs object,\n");
    obj->m_dcs = dcs;

    for (dcs_difference_t i = 0; i < obj->cm_num_order; ++i)
        obj->p_input_buffer[i] = obj->m_dcs->m_zero;

    for (dcs_difference_t i = 0; i < obj->cm_den_order; ++i)
        obj->p_output_buffer[i] = obj->m_dcs->m_zero;

    obj->fn_calc = _z_func_calc_default;
    obj->p_cir_pos = 0;
}

void init_z_function_struct_mm(struct _entity_z_transfer_function *obj, struct _entity_discrete_control_system *dcs)
{
    obj->p_input_buffer = obj->cm_num_order == 0 ? 0 : (dcs_param_t *)malloc(sizeof(dcs_param_t) * obj->cm_num_order);
    obj->p_output_buffer = obj->cm_den_order == 0 ? 0 : (dcs_param_t *)malloc(sizeof(dcs_param_t) * obj->cm_den_order);
    init_z_function_struct(obj, dcs);
}

void deinit_z_function_struct_mm(struct _entity_z_transfer_function *obj)
{
    if (obj->cm_num_order != 0)
        free(obj->p_input_buffer);
    if (obj->cm_den_order != 0)
        free(obj->p_output_buffer);
}
