extern void extend_dill_stream(dill_stream s);
extern void dump_cur_dill_insn(dill_stream s);
extern jmp_table alloc_dill_jump_table(void);

#define INIT_CODE_SIZE 64

typedef struct arg_info { 
    char type;
    char is_register;		/* true if parameter is in register */
    char is_immediate;		/* true if actual is an immediate */
    unsigned in_reg;		/* callee register it's in */
    unsigned out_reg;		/* caller register it's in */
    int offset;			/* otherwise at this offset from v_pp */
} *arg_info_list;

typedef struct reg_set {
    long init_avail[1];
    long members[1];
    long avail[1];
    long used[1];
    long mustsave[1];
} reg_set;

struct branch_location {
    int label;
    int loc;
};

struct data_mark {
    void **addr;
    int label;
};

struct branch_table {
    int next_label;
    int max_alloc;
    int *label_locs;
    int branch_count;
    int branch_alloc;
    struct branch_location *branch_locs;
    int data_mark_count;
    struct data_mark *data_marks;
};

typedef struct branch_table *branch_t;


struct call_location {
    int loc;
    void *xfer_addr;
    char *xfer_name;
    void *mach_info;
};

typedef struct call_table {
    int call_alloc;
    int call_count;
    struct call_location *call_locs;
} call_t;

typedef struct ret_table {
    int ret_alloc;
    int ret_count;
    int *ret_locs;
} ret_t;

typedef void (*mach_reset_func)(dill_stream s);

typedef struct vreg_info {
    int typ;
    int preg;
    int offset;
    int need_assign;
    struct {
	short use_count;
	short def_count;
    } use_info;

    int assign_loc;
    int in_reg;
    int last_use;
    int use_metric;
    int update_in_reg;
    int value_in_mem;
} vreg_info;

typedef struct preg_info {
    int holds;
} preg_info;

typedef struct saved {
    jmp_table mach_jump;
    mach_reset_func mach_reset;
    void *mach_info;
    void *code_base;
    void *cur_ip;
    void *code_limit;
} saved_insn_info;

struct dill_private_ctx {
    void *code_base;
    void *cur_ip;
    void *code_limit;
    void *fp;
    void *fpc;
    int ret_type;
    struct branch_table branch_table;
    struct call_table call_table;
    struct ret_table ret_table;
    mach_reset_func mach_reset;
    mach_reset_func native_mach_reset;
    saved_insn_info native;
    saved_insn_info virtual;
    void *mach_info;
    int machine_strr_tmp_reg;
    reg_set var_i;
    reg_set tmp_i;
    reg_set var_f;
    reg_set tmp_f;
    int c_param_count;
    int save_param_count;
    dill_reg **c_param_regs;
    arg_info_list c_param_args;
    dill_parameter_type **c_param_structs;
    int doing_reverse_push;
    int unavail_called;

    int vreg_count;
    vreg_info *vregs;
    int v_tmps[DILL_B][3];
};

struct reg_type {
    union {
	struct {
#ifdef WORDS_BIGENDIAN
#if SIZEOF_LONG == 4
	    char junk1, junk2, junk3;
#else
	    char junk1, junk2, junk3, junk4, junk5, junk6, junk7;
#endif
#endif
	    char c;
	} c;
	struct {
#ifdef WORDS_BIGENDIAN
#if SIZEOF_LONG == 4
	    char junk1, junk2, junk3;
#else
	    char junk1, junk2, junk3, junk4, junk5, junk6, junk7;
#endif
#endif
	    unsigned char uc;
	} uc;
	struct {
#ifdef WORDS_BIGENDIAN
#if SIZEOF_LONG == 4
	    char junk1, junk2;
#else
	    char junk1, junk2, junk3, junk4, junk5, junk6;
#endif
#endif
	    short s;
	} s;
	struct {
#ifdef WORDS_BIGENDIAN
#if SIZEOF_LONG == 4
	    char junk1, junk2;
#else
	    char junk1, junk2, junk3, junk4, junk5, junk6;
#endif
#endif
	    unsigned short us;
	} us;
	struct {
#ifdef WORDS_BIGENDIAN
#if SIZEOF_LONG == 8
	    char junk1, junk2, junk3, junk4;
#endif
#endif
	    int i;
	} i;
	struct {
#ifdef WORDS_BIGENDIAN
#if SIZEOF_LONG == 8
	    char junk1, junk2, junk3, junk4;
#endif
#endif
	    unsigned int u;
	} u;
	struct {
	    long l;
	} l;
	struct {
	    unsigned long ul;
	} ul;
	struct {
	    void * p;
	} p;
	struct {
	    float f;
	} f;
	struct {
	    double d;
	} d;
    } u;
};

struct calling_param {
    int typ;
    struct reg_type val;
};

struct client_data_struct {
    int key;
    long value;
};

struct dec {
    dill_stream dc;
    int ret_reg;
    int param_count;
    struct reg_type *r;
    struct reg_type *p;
    int out_param_count;
    struct calling_param *out_params;
    int client_data_count;
    struct client_data_struct *client_data;
};

extern int dill_mustsave(reg_set *regs, int reg);
extern int dill_wasused(reg_set *regs, int reg);
extern void dill_mark_branch_location(dill_stream s, int label);
extern void dill_mark_call_location(dill_stream s, char *xfer_name,
				 void *xfer_address);
extern void dill_mark_ret_location(dill_stream s);
extern void dill_end_vararg_push(dill_stream s);
EXTERN void dill_dump_reg(dill_stream s, int typ, int reg);
extern void setup_VM_proc(dill_stream s);