//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

//..............................................................................

// pthread_cond_clockwait introduced in glibc-2.30
// fallback: convert deadline to CLOCK_REALTIME, then wait using pthread_cond_timedwait

int
__wrap_pthread_cond_clockwait(
	pthread_cond_t* cond,
	pthread_mutex_t* mutex,
	clockid_t clockId,
	const struct timespec* deadline
) {
	if (clockId == CLOCK_REALTIME)
		return pthread_cond_timedwait(cond, mutex, deadline);

	if (clockId != CLOCK_MONOTONIC)
		return EINVAL;

	struct timespec monotonicNow;
	struct timespec realtimeDeadline;
	clock_gettime(CLOCK_MONOTONIC, &monotonicNow);
	clock_gettime(CLOCK_REALTIME, &realtimeDeadline);

	int64_t delta =
		(int64_t)(deadline->tv_sec - monotonicNow.tv_sec) * 1000000000 +
		(deadline->tv_nsec - monotonicNow.tv_nsec);

	if (delta > 0) {
		delta += realtimeDeadline.tv_nsec;
		realtimeDeadline.tv_sec += delta / 1000000000;
		realtimeDeadline.tv_nsec = delta % 1000000000;
	}

	return pthread_cond_timedwait(cond, mutex, &realtimeDeadline);
}

//..............................................................................
