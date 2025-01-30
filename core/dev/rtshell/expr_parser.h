
#ifndef _FILE_RTSH_EXPR_PARSER_H_
#define _FILE_RTSH_EXPR_PARSER_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define RTSH_BASIC_TYPE float

#ifndef NAN
#define NAN (0.0 / 0.0)
#endif // NAN

#ifndef INFINITY
#define INFINITY (1.0 / 0.0)
#endif // INFINITY

    typedef enum _tag_rtsh_expr_type
    {
        RTSH_SYMBOL_VARIABLE = 0,
        RTSH_SYMBOL_NUMBER = 1,

        RTSH_SYMBOL_FUNCTION = 2,

        RTSH_SYMBOL_CLOSURE = 3

    } rtsh_expr_type_t;

    typedef struct _tag_rtsh_expr
    {
        // type of expr
        rtsh_expr_type_t type;

    } rtsh_expr_t;

    typedef enum _tag_rtsh_expr_state_t
    {
        RTSH_STATE_NULL = 0,
        RTSH_STATE_END = 1,

        RTSH_STATE_NUMBER = 2,
        RTSH_STATE_VARIABLE = 3
    } rtsh_state_t;

    typedef struct _tag_rtsh_state_machine
    {
        rtsh_state_t state;

    } rtsh_state_machine_t;



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_RTSH_EXPR_PARSER_H_
