

#ifndef __LIBGSOCKET_SELECT_H__
#define __LIBGSOCKET_SELECT_H__ 1


struct _gs_sel_item
{
	int (*func)(void *ctx, int fd, void *cb_arg, int cb_val);
	void *cb_arg;
	int cb_val;
};

typedef struct _gs_select_ctx
{
	int max_fd;
	fd_set *rfd;
	fd_set *wfd;
	fd_set *r;
	fd_set *w;
	struct timeval tv;
	struct timeval *tv_now;

	struct _gs_sel_item mgr_r[FD_SETSIZE];
	struct _gs_sel_item mgr_w[FD_SETSIZE];
	
	int blocking_func[FD_SETSIZE];		/* GS_CALLREAD || GS_CALLWRITE */
	int saved_rw_state[FD_SETSIZE];		/* 0 == not saved. 1 = READ, 2 = WRITE, 3 = R&W */
	int is_rw_state_saved[FD_SETSIZE];
	int want_io_write[FD_SETSIZE];
	int want_io_read[FD_SETSIZE];

	int rdata_pending[FD_SETSIZE];
	int rdata_pending_count;


	/* Heartbeat / Timeout control */
	int hb_freq;		/* Every hb_freq usec return to caller */
	uint64_t hb_init;	/* Initial time when hb was set */
	uint64_t hb_next;	/* Next time fo hb */
} GS_SELECT_CTX;

// #define GS_CALLDEFAULT		(0x00)
#define GS_CALLREAD			(0x01)
#define GS_CALLWRITE		(0x02)

typedef int (*gselect_cb_t)(GS_SELECT_CTX *ctx, int fd, void *arg, int val);

int GS_SELECT_CTX_init(GS_SELECT_CTX *ctx, fd_set *rfd, fd_set *wfd, fd_set *r, fd_set *w, struct timeval *tv_now, int frequency);
int GS_select(GS_SELECT_CTX *ctx);
void GS_SELECT_add_cb_r(GS_SELECT_CTX *ctx, gselect_cb_t func, int fd, void *arg, int val);
void GS_SELECT_add_cb_w(GS_SELECT_CTX *ctx, gselect_cb_t func, int fd, void *arg, int val);
void GS_SELECT_add_cb(GS_SELECT_CTX *ctx, gselect_cb_t func_r, gselect_cb_t func_w, int fd, void *arg, int val);
void GS_SELECT_add_cb_callagain(GS_SELECT_CTX *ctx, gselect_cb_t func_r, gselect_cb_t func_w, int fd, void *arg, int val);
void GS_SELECT_del_cb(GS_SELECT_CTX *ctx, int fd);
void GS_SELECT_del_cb_callagain(GS_SELECT_CTX *ctx, int fd);


#define GS_SUCCESS			(0x00)
#define GS_ECALLAGAIN		(0x01)		/* Return Error Likes to be calleda gain */
#define GS_ERROR			(0x02)
#define GS_ERR_WAITING		-1		/* Waiting for I/O */
#define GS_ERR_FATAL		-2
#define GS_ERR_EOF			-3

#endif /* !__LIBGSOCKET_SELECT_H__ */
