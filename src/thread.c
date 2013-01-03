#include <common.h>
#include <register.h>

#include <thread.h>

pthread_t thread_create
(	void *call,
		int id)
{
	o_thrd object = { {0} };
	pthread_t pt = {0};

	int r;

	if ( (r = pthread_create
			(&pt, NULL, (void*)(call), NULL)) )
		return r;
	
	object.pt = pt;
	object.id = id;

	if ( register_object
			((void*)&object, (void*)&threads,
				THREAD_MAX, sizeof (o_thrd))
				== NULL) {
		return -0xE;
	}
	
	return 0;
}

int thread_destroy
(pthread_t pt)
{
  	int s;
  	void *res;

	if ((s = pthread_cancel(pt)) != 0) 
		return s;  
	
	if ((s = pthread_join(pt, &res)) != 0)
		return s;

	if (res != PTHREAD_CANCELED)    
		return -0xF;

	po_gh r = (po_gh)search_object
			((unsigned char*)pt,
				sizeof(pthread_t),
				(unsigned char*)&threads,
				sizeof(o_thrd), 0);

	if ( !r ) 
		return -0x4;

	unregister_object
		(r, (p_goa) &threads);
	
	return 0;
}

po_thrd thread_get_from_id
(int id)
{	
	return (po_thrd)search_object
			((unsigned char*)&id,
				4,
				(unsigned char*)&threads,
				sizeof(o_thrd),
				sizeof(pthread_t));
}

po_thrd thread_get_from_tid
(pthread_t tid)
{	
	return (po_thrd)search_object
			((unsigned char*)tid,
					sizeof(pthread_t),
					(unsigned char*)&threads,
					sizeof(o_thrd),
					0);
}

int generic_worker
(void *arg)
{
	return 0;
}


