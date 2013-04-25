// Copyright (c) 2001-2010 quickfixengine.org  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
//
// 3. The end-user documentation included with the redistribution,
//    if any, must include the following acknowledgment:
//      "This product includes software developed by
//       quickfixengine.org (http://www.quickfixengine.org/)."
//   Alternately, this acknowledgment may appear in the software itself,
//   if and wherever such third-party acknowledgments normally appear.
//
// 4. The names "QuickFIX" and "quickfixengine.org" must
//    not be used to endorse or promote products derived from this
//    software without prior written permission. For written
//    permission, please contact ask@quickfixengine.org
//
// 5. Products derived from this software may not be called "QuickFIX",
//    nor may "QuickFIX" appear in their name, without prior written
//    permission of quickfixengine.org
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL QUICKFIXENGINE.ORG OR
// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#ifndef FIX_THREADEDSOCKETCONNECTION_H
#define FIX_THREADEDSOCKETCONNECTION_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Parser.h"
#include "Responder.h"
#include "SessionID.h"
#include <set>
#include <map>

namespace FIX
{
class ThreadedSocketAcceptor;
class ThreadedSocketInitiator;
class Session;
class Application;
class Log;

/// Encapsulates a socket file descriptor (multi-threaded).
class ThreadedSocketConnection : Responder
{
public:
  typedef std::set<SessionID> Sessions;

  ThreadedSocketConnection( int s, Sessions sessions, Application& application, Log* pLog );
  ThreadedSocketConnection( const SessionID&, int s, 
                            const std::string& address, short port, 
                            Application&, Log* pLog );
  virtual ~ThreadedSocketConnection() ;

  Session* getSession() const { return m_pSession; }
  int getSocket() const { return m_socket; }
  bool connect();
  void disconnect();
  bool read();

private:
  bool readMessage( std::string& msg ) throw( SocketRecvFailed );
  void processStream();
  bool send( const std::string& );
  bool setSession( const std::string& msg );

  int m_socket;
  char m_buffer[BUFSIZ];

  std::string m_address;
  int m_port;

  Application& m_application;
  Log* m_pLog;
  Parser m_parser;
  Sessions m_sessions;
  Session* m_pSession;
  bool m_disconnect;
  fd_set m_fds;
};
}

#endif //FIX_THREADEDSOCKETCONNECTION_H
