#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <Carbon/Carbon.h>

#include "server.hpp"
#include "server_objc_part.h"
#include "Mutex.hpp"

bool
KeyRemap4MacBook_server::Server::initialize(const char* basedirectory)
{
  if (! basedirectory) return false;
  if (*basedirectory == '\0') return false;

  socketpath_ = std::string(basedirectory) + "/KeyRemap4MacBook_server";
  if (! makeSocket()) return false;
  if (listenSocket_ == -1) return false;

  int error = listen(listenSocket_, 128);
  if (error) return false;

  return true;
}

void
KeyRemap4MacBook_server::Server::sendReply(int sock, void* data, size_t size, uint32_t error)
{
  struct iovec iov[2];
  iov[0].iov_base = reinterpret_cast<caddr_t>(&error);
  iov[0].iov_len = sizeof(error);
  if (size > 0) {
    iov[1].iov_base = reinterpret_cast<caddr_t>(data);
    iov[1].iov_len = size;
  }

  struct msghdr msg;
  memset(&msg, 0, sizeof(msg));
  msg.msg_iov = iov;
  if (size > 0) {
    msg.msg_iovlen = 2;
  } else {
    msg.msg_iovlen = 1;
  }

  sendmsg(sock, &msg, 0);
}

void
KeyRemap4MacBook_server::Server::dispatchOperator(int sock)
{
  org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::RequestType operation;
  if (read(sock, &operation, sizeof(operation)) < 0) goto error;

  switch (operation) {
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_GET_CONFIG_COUNT:
    {
      if (! do_GetConfigCount(sock)) goto error;
      break;
    }

    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_GET_CONFIG_INFO:
    {
      if (! do_GetConfigInfo(sock)) goto error;
      break;
    }

    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_GET_CONFIG_INITIALIZE_VECTOR:
    {
      if (! do_GetConfigInitializeVector(sock)) goto error;
      break;
    }

    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_NONE:
    default:
      goto error;
  }

  return;

error:
  sendReply(sock, NULL, 0, org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ERROR);
}

bool
KeyRemap4MacBook_server::Server::process(void)
{
  if (listenSocket_ == -1) return false;

  int s = accept(listenSocket_, NULL, NULL);
  if (s < 0) return false;

  dispatchOperator(s);
  close(s);
  return true;
}

// --------------------------------------------------
bool
KeyRemap4MacBook_server::Server::makeSocket(void)
{
  listenSocket_ = socket(PF_LOCAL, SOCK_STREAM, 0);
  if (listenSocket_ < 0) return false;

  struct sockaddr_un listenSocketAddr;
  memset(&listenSocketAddr, 0, sizeof(listenSocketAddr));
  listenSocketAddr.sun_len = sizeof(listenSocketAddr);
  listenSocketAddr.sun_family = AF_UNIX;
  snprintf(listenSocketAddr.sun_path, sizeof(listenSocketAddr.sun_path), "%s", socketpath_.c_str());

  if (bind(listenSocket_, reinterpret_cast<struct sockaddr*>(&listenSocketAddr), sizeof(listenSocketAddr)) < 0) return false;

  return true;
}

// --------------------------------------------------
bool
KeyRemap4MacBook_server::Server::do_GetConfigCount(int sock)
{
  org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetConfigCount::Reply reply;

  reply.count = getConfigCount();

  sendReply(sock, &reply, sizeof(reply), org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::SUCCESS);
  return true;
}

bool
KeyRemap4MacBook_server::Server::do_GetConfigInfo(int sock)
{
  bool retval = false;
  uint32_t count = getConfigCount();

  org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetConfigInfo::Reply::Item* items = new org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetConfigInfo::Reply::Item[count];
  if (! items) goto finish;

  for (uint32_t configindex = 0; configindex < count; ++configindex) {
    items[configindex].initialize_vector_size = getConfigInitializeVectorSize(configindex);
    items[configindex].enabled = getConfigValue(configindex);
  }

  sendReply(sock, items, sizeof(items[0]) * count, org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::SUCCESS);
  retval = true;

finish:
  if (items) {
    delete[] items;
  }
  return retval;
}

bool
KeyRemap4MacBook_server::Server::do_GetConfigInitializeVector(int sock)
{
  bool retval = false;
  uint32_t* initialize_vector = NULL;
  uint32_t initialize_vector_size = 0;
  org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetConfigInitializeVector::Request request(0);
  uint32_t size = 0;
  int error = 0;

  if (read(sock, &size, sizeof(size)) < 0) goto finish;

  if (size != sizeof(request)) goto finish;
  if (read(sock, &request, sizeof(request)) < 0) goto finish;

  initialize_vector_size = getConfigInitializeVectorSize(request.configindex);
  if (initialize_vector_size == 0) goto finish;

  initialize_vector = new uint32_t[initialize_vector_size];

  error = getConfigInitializeVector(initialize_vector, initialize_vector_size, request.configindex);
  if (error) goto finish;

  sendReply(sock, initialize_vector, sizeof(initialize_vector[0]) * initialize_vector_size, org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::SUCCESS);
  retval = true;

finish:
  if (initialize_vector) {
    delete[] initialize_vector;
  }
  return retval;
}
