// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_EVENTLOOPTHREADPOOL_H
#define MUDUO_NET_EVENTLOOPTHREADPOOL_H

#include <muduo/base/noncopyable.h>
#include <muduo/base/Types.h>

#include <functional>
#include <memory>
#include <vector>

namespace muduo
{

namespace net
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable
{
public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

private:
  ///> EventLoop object that hold the EventLoopThreadPool object.
  EventLoop* baseLoop_;
  string name_;
  bool started_;
  ///> counter of new EventLoopThread.
  int numThreads_;
  ///> combine EventLoopThread name string, and the next_ is mutable part.
  int next_;
  ///> EventLoopThread objects that represent new loop threas.
  std::vector<std::unique_ptr<EventLoopThread>> threads_;
  ///> EventLoop objects belong new loop threads.
  std::vector<EventLoop*> loops_;

public:
  EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg);
  ~EventLoopThreadPool();
  ///> set new EventLoopThread counter.
  void setThreadNum(int numThreads) { numThreads_ = numThreads; }
  ///> real new EventLoopThread, and initialization.
  void start(const ThreadInitCallback& cb = ThreadInitCallback());

  // valid after calling start()
  /// round-robin
  EventLoop* getNextLoop();

  /// with the same hash code, it will always return the same EventLoop
  EventLoop* getLoopForHash(size_t hashCode);

  ///> if has new loop threads, return; else return baseLoop_.
  std::vector<EventLoop*> getAllLoops();

  bool started() const { return started_; }

  const string& name() const { return name_; }
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_EVENTLOOPTHREADPOOL_H
