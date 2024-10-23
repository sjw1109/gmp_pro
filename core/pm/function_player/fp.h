
// 思路简记

// EXT 模块的设计可以基于这一类functional进行派生。

typedef(time_gt) (*tick_fn)(void);


typedef(gmp_error_t)(functional_gt)(gmp_fp_functional *param);


typedef struct _tag_gmp_fp_functional
{
    // 这个函数一定是非阻塞的
    // Non-blocking function
    functional_gt callback;

    // parameter for callback
    // 可以通过继承关系获得灵活性
    //void *param;

    // last invoke time
    // for time counting and delta-time mode programming
    time_gt last_tick;

    // 父节点
    gmp_function_player *parent;

    // 形成链表，具体链表应当是什么形式，未知，目的在于做调度
    gmp_fp_functional *next;

}gmp_fp_functional;

// utilities for fp_functional type
// 

// 这个函数将会被player的响应函数调用
gmp_error_t invoke_fp_functional(gmp_fp_functional *fp_obj)
{
    fp_obj->callback();

    //更新结构体中其他数据项
}

typedef struct _tag_function_player
{
    // function point to tick get
    tick_fn tick;

    // last invoke time
    time_gt last_tick;

    // list to functional
    vector<gmp_fp_functional> function_pointer_list;

}gmp_function_player;

// 注册一个新的functional
