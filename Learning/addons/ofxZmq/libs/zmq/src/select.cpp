/*
    Copyright (c) 2007-2015 Contributors as noted in the AUTHORS file

    This file is part of libzmq, the ZeroMQ core engine in C++.

    libzmq is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    As a special exception, the Contributors give you permission to link
    this library with independent modules to produce an executable,
    regardless of the license terms of these independent modules, and to
    copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the
    terms and conditions of the license of that module. An independent
    module is a module which is not derived from or based on this library.
    If you modify this library, you must extend this exception to your
    version of the library.

    libzmq is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "select.hpp"
#if defined ZMQ_USE_SELECT

#include "platform.hpp"
#if defined ZMQ_HAVE_WINDOWS
#include "windows.hpp"
#elif defined ZMQ_HAVE_HPUX
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#elif defined ZMQ_HAVE_OPENVMS
#include <sys/types.h>
#include <sys/time.h>
#else
#include <sys/select.h>
#endif

#include <string.h>
#include <algorithm>

#include "err.hpp"
#include "config.hpp"
#include "i_poll_events.hpp"

zmq::select_t::select_t (const zmq::ctx_t &ctx_) :
    ctx(ctx_),
    maxfd (retired_fd),
    retired (false),
    stopping (false)
{
    //  Clear file descriptor sets.
    FD_ZERO (&source_set_in);
    FD_ZERO (&source_set_out);
    FD_ZERO (&source_set_err);
}

zmq::select_t::~select_t ()
{
    worker.stop ();
}

zmq::select_t::handle_t zmq::select_t::add_fd (fd_t fd_, i_poll_events *events_)
{
    //  Store the file descriptor.
    fd_entry_t entry = {fd_, events_};
    fds.push_back (entry);

    //  Ensure we do not attempt to select () on more than FD_SETSIZE
    //  file descriptors.
    zmq_assert (fds.size () <= FD_SETSIZE);

    //  Start polling on errors.
    FD_SET (fd_, &source_set_err);

    //  Adjust maxfd if necessary.
    if (fd_ > maxfd)
        maxfd = fd_;

    //  Increase the load metric of the thread.
    adjust_load (1);

    return fd_;
}

void zmq::select_t::rm_fd (handle_t handle_)
{
    //  Mark the descriptor as retired.
    fd_set_t::iterator it;
    for (it = fds.begin (); it != fds.end (); ++it)
        if (it->fd == handle_)
            break;
    zmq_assert (it != fds.end ());
    it->fd = retired_fd;
    retired = true;

    //  Stop polling on the descriptor.
    FD_CLR (handle_, &source_set_in);
    FD_CLR (handle_, &source_set_out);
    FD_CLR (handle_, &source_set_err);

    //  Discard all events generated on this file descriptor.
    FD_CLR (handle_, &readfds);
    FD_CLR (handle_, &writefds);
    FD_CLR (handle_, &exceptfds);

    //  Adjust the maxfd attribute if we have removed the
    //  highest-numbered file descriptor.
    if (handle_ == maxfd) {
        maxfd = retired_fd;
        for (fd_set_t::iterator it = fds.begin (); it != fds.end (); ++it)
            if (it->fd > maxfd)
                maxfd = it->fd;
    }

    //  Decrease the load metric of the thread.
    adjust_load (-1);
}

void zmq::select_t::set_pollin (handle_t handle_)
{
    FD_SET (handle_, &source_set_in);
}

void zmq::select_t::reset_pollin (handle_t handle_)
{
    FD_CLR (handle_, &source_set_in);
}

void zmq::select_t::set_pollout (handle_t handle_)
{
    FD_SET (handle_, &source_set_out);
}

void zmq::select_t::reset_pollout (handle_t handle_)
{
    FD_CLR (handle_, &source_set_out);
}

void zmq::select_t::start ()
{
    ctx.start_thread (worker, worker_routine, this);
}

void zmq::select_t::stop ()
{
    stopping = true;
}

int zmq::select_t::max_fds ()
{
    return FD_SETSIZE;
}

void zmq::select_t::loop ()
{
    while (!stopping) {

        //  Execute any due timers.
        int timeout = (int) execute_timers ();

        //  Intialise the pollsets.
        memcpy (&readfds, &source_set_in, sizeof source_set_in);
        memcpy (&writefds, &source_set_out, sizeof source_set_out);
        memcpy (&exceptfds, &source_set_err, sizeof source_set_err);

        //  Wait for events.
#ifdef ZMQ_HAVE_OSX
        struct timeval tv = {(long) (timeout / 1000), timeout % 1000 * 1000};
#else
        struct timeval tv = {(long) (timeout / 1000),
            (long) (timeout % 1000 * 1000)};
#endif
#ifdef ZMQ_HAVE_WINDOWS
        int rc = select (0, &readfds, &writefds, &exceptfds,
            timeout ? &tv : NULL);
        wsa_assert (rc != SOCKET_ERROR);
#else
        int rc = select (maxfd + 1, &readfds, &writefds, &exceptfds,
            timeout ? &tv : NULL);
        if (rc == -1) {
            errno_assert (errno == EINTR);
            continue;
        }
#endif

        //  If there are no events (i.e. it's a timeout) there's no point
        //  in checking the pollset.
        if (rc == 0)
            continue;

        for (fd_set_t::size_type i = 0; i < fds.size (); i ++) {
            if (fds [i].fd == retired_fd)
                continue;
            if (FD_ISSET (fds [i].fd, &exceptfds))
                fds [i].events->in_event ();
            if (fds [i].fd == retired_fd)
                continue;
            if (FD_ISSET (fds [i].fd, &writefds))
                fds [i].events->out_event ();
            if (fds [i].fd == retired_fd)
                continue;
            if (FD_ISSET (fds [i].fd, &readfds))
                fds [i].events->in_event ();
        }

        //  Destroy retired event sources.
        if (retired) {
            fds.erase (std::remove_if (fds.begin (), fds.end (),
                zmq::select_t::is_retired_fd), fds.end ());
            retired = false;
        }
    }
}

void zmq::select_t::worker_routine (void *arg_)
{
    ((select_t*) arg_)->loop ();
}

bool zmq::select_t::is_retired_fd (const fd_entry_t &entry)
{
    return (entry.fd == retired_fd);
}

#endif
