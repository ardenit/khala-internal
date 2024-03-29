/*  =========================================================================
    zloop - event-driven reactor

    Copyright (c) the Contributors as noted in the AUTHORS file.
    This file is part of CZMQ, the high-level C binding for 0MQ:
    http://czmq.zeromq.org.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef __ZLOOP_H_INCLUDED__
#define __ZLOOP_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

//  @warning THE FOLLOWING @INTERFACE BLOCK IS AUTO-GENERATED BY ZPROJECT
//  @warning Please edit the model at "api/zloop.api" to make changes.
//  @interface
//  This is a stable class, and may not change except for emergencies. It
//  is provided in stable builds.
// Callback function for reactor socket activity
//typedef int (zloop_reader_fn) (
//    zloop_t *loop, zsock_t *reader, void *arg);

// Callback function for reactor events (low-level)
//typedef int (zloop_fn) (
//    zloop_t *loop, zmq_pollitem_t *item, void *arg);

// Callback for reactor timer events
//typedef int (zloop_timer_fn) (
//    zloop_t *loop, int timer_id, void *arg);

//  Create a new zloop reactor
CZMQ_EXPORT zloop_t *
    zloop_new (void);

//  Destroy a reactor
CZMQ_EXPORT void
    zloop_destroy (zloop_t **self_p);

//  Register socket reader with the reactor. When the reader has messages,
//  the reactor will call the handler, passing the arg. Returns 0 if OK, -1
//  if there was an error. If you register the same socket more than once,
//  each instance will invoke its corresponding handler.
CZMQ_EXPORT int
    zloop_reader (zloop_t *self, zsock_t *sock, int (*handler) (zloop_t *, zsock_t *, void *), void *arg);

//  Cancel a socket reader from the reactor. If multiple readers exist for
//  same socket, cancels ALL of them.
CZMQ_EXPORT void
    zloop_reader_end (zloop_t *self, zsock_t *sock);

//  Configure a registered reader to ignore errors. If you do not set this,
//  then readers that have errors are removed from the reactor silently.
CZMQ_EXPORT void
    zloop_reader_set_tolerant (zloop_t *self, zsock_t *sock);

//  Register low-level libzmq pollitem with the reactor. When the pollitem
//  is ready, will call the handler, passing the arg. Returns 0 if OK, -1
//  if there was an error. If you register the pollitem more than once, each
//  instance will invoke its corresponding handler. A pollitem with
//  socket=NULL and fd=0 means 'poll on FD zero'.
CZMQ_EXPORT int
    zloop_poller (zloop_t *self, zmq_pollitem_t *item, int (*handler) (zloop_t *, zmq_pollitem_t *, void *), void *arg);

//  Cancel a pollitem from the reactor, specified by socket or FD. If both
//  are specified, uses only socket. If multiple poll items exist for same
//  socket/FD, cancels ALL of them.
CZMQ_EXPORT void
    zloop_poller_end (zloop_t *self, zmq_pollitem_t *item);

//  Configure a registered poller to ignore errors. If you do not set this,
//  then poller that have errors are removed from the reactor silently.
CZMQ_EXPORT void
    zloop_poller_set_tolerant (zloop_t *self, zmq_pollitem_t *item);

//  Register a timer that expires after some delay and repeats some number of
//  times. At each expiry, will call the handler, passing the arg. To run a
//  timer forever, use 0 times. Returns a timer_id that is used to cancel the
//  timer in the future. Returns -1 if there was an error.
CZMQ_EXPORT int
    zloop_timer (zloop_t *self, size_t delay, size_t times, int (*handler) (zloop_t *, int, void *), void *arg);

//  Cancel a specific timer identified by a specific timer_id (as returned by
//  zloop_timer).
CZMQ_EXPORT int
    zloop_timer_end (zloop_t *self, int timer_id);

//  Register a ticket timer. Ticket timers are very fast in the case where
//  you use a lot of timers (thousands), and frequently remove and add them.
//  The main use case is expiry timers for servers that handle many clients,
//  and which reset the expiry timer for each message received from a client.
//  Whereas normal timers perform poorly as the number of clients grows, the
//  cost of ticket timers is constant, no matter the number of clients. You
//  must set the ticket delay using zloop_set_ticket_delay before creating a
//  ticket. Returns a handle to the timer that you should use in
//  zloop_ticket_reset and zloop_ticket_delete.
CZMQ_EXPORT void *
    zloop_ticket (zloop_t *self, int (*handler) (zloop_t *, int, void *), void *arg);

//  Reset a ticket timer, which moves it to the end of the ticket list and
//  resets its execution time. This is a very fast operation.
CZMQ_EXPORT void
    zloop_ticket_reset (zloop_t *self, void *handle);

//  Delete a ticket timer. We do not actually delete the ticket here, as
//  other code may still refer to the ticket. We mark as deleted, and remove
//  later and safely.
CZMQ_EXPORT void
    zloop_ticket_delete (zloop_t *self, void *handle);

//  Set the ticket delay, which applies to all tickets. If you lower the
//  delay and there are already tickets created, the results are undefined.
CZMQ_EXPORT void
    zloop_set_ticket_delay (zloop_t *self, size_t ticket_delay);

//  Set hard limit on number of timers allowed. Setting more than a small
//  number of timers (10-100) can have a dramatic impact on the performance
//  of the reactor. For high-volume cases, use ticket timers. If the hard
//  limit is reached, the reactor stops creating new timers and logs an
//  error.
CZMQ_EXPORT void
    zloop_set_max_timers (zloop_t *self, size_t max_timers);

//  Set verbose tracing of reactor on/off. The default verbose setting is
//  off (false).
CZMQ_EXPORT void
    zloop_set_verbose (zloop_t *self, bool verbose);

//  By default the reactor stops if the process receives a SIGINT or SIGTERM
//  signal. This makes it impossible to shut-down message based architectures
//  like zactors. This method lets you switch off break handling. The default
//  nonstop setting is off (false).
CZMQ_EXPORT void
    zloop_set_nonstop (zloop_t *self, bool nonstop);

//  Start the reactor. Takes control of the thread and returns when the 0MQ
//  context is terminated or the process is interrupted, or any event handler
//  returns -1. Event handlers may register new sockets and timers, and
//  cancel sockets. Returns 0 if interrupted, -1 if canceled by a handler.
CZMQ_EXPORT int
    zloop_start (zloop_t *self);

//  Self test of this class.
CZMQ_EXPORT void
    zloop_test (bool verbose);

//  @end


//  Deprecated method aliases
#define zloop_set_tolerant(s,i) zloop_poller_set_tolerant(s,i)

#ifdef __cplusplus
}
#endif

#endif
