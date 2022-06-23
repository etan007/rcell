//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if defined(_MSC_VER)
#pragma warning(disable:4996)
#endif

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)
#	ifndef _WIN32_WINNT
#		if __has_include(<winsdkver.h>)
#			include <winsdkver.h>
#			define _WIN32_WINNT _WIN32_WINNT_WIN7
#		endif
#		if __has_include(<SDKDDKVer.h>)
#			include <SDKDDKVer.h>
#		endif
#	endif
#endif

#define ASIO_STANDALONE
#include "asio.hpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include "handler_allocator.hpp"

class stats
{
public:
  stats()
    : mutex_(),
      total_bytes_written_(0),
      total_bytes_read_(0)
  {
  }

  void add(size_t bytes_written, size_t bytes_read)
  {
    asio::detail::mutex::scoped_lock lock(mutex_);
    total_bytes_written_ += bytes_written;
    total_bytes_read_ += bytes_read;
  }

  void print()
  {
    asio::detail::mutex::scoped_lock lock(mutex_);
    std::cout << total_bytes_written_ << " total bytes written\n";
    std::cout << total_bytes_read_ << " total bytes read\n";
  }

private:
  asio::detail::mutex mutex_;
  size_t total_bytes_written_;
  size_t total_bytes_read_;
};

class session
{
public:
  session(asio::io_context& ioc, size_t block_size, stats& s)
    : strand_(ioc.get_executor()),
      socket_(ioc),
      block_size_(block_size),
      read_data_(new char[block_size]),
      read_data_length_(0),
      write_data_(new char[block_size]),
      unwritten_count_(0),
      bytes_written_(0),
      bytes_read_(0),
      stats_(s)
  {
    for (size_t i = 0; i < block_size_; ++i)
      write_data_[i] = static_cast<char>(i % 128);
  }

  ~session()
  {
    stats_.add(bytes_written_, bytes_read_);

    delete[] read_data_;
    delete[] write_data_;
  }

  void start(asio::ip::tcp::resolver::results_type endpoints)
  {
    asio::async_connect(socket_, endpoints,
        asio::bind_executor(strand_,
          std::bind(&session::handle_connect, this,
            std::placeholders::_1)));
  }

  void stop()
  {
    asio::post(strand_, std::bind(&session::close_socket, this));
  }

private:
  void handle_connect(const asio::error_code& err)
  {
    if (!err)
    {
      asio::error_code set_option_err;
      asio::ip::tcp::no_delay no_delay(true);
      socket_.set_option(no_delay, set_option_err);
      if (!set_option_err)
      {
        ++unwritten_count_;
        async_write(socket_, asio::buffer(write_data_, block_size_),
            asio::bind_executor(strand_,
              make_custom_alloc_handler(write_allocator_,
                std::bind(&session::handle_write, this,
					std::placeholders::_1,
					std::placeholders::_2))));
        socket_.async_read_some(asio::buffer(read_data_, block_size_),
            asio::bind_executor(strand_,
              make_custom_alloc_handler(read_allocator_,
                std::bind(&session::handle_read, this,
					std::placeholders::_1,
					std::placeholders::_2))));
      }
    }
  }

  void handle_read(const asio::error_code& err, size_t length)
  {
    if (!err)
    {
      bytes_read_ += length;

      read_data_length_ = length;
      ++unwritten_count_;
      if (unwritten_count_ == 1)
      {
        std::swap(read_data_, write_data_);
        async_write(socket_, asio::buffer(write_data_, read_data_length_),
            asio::bind_executor(strand_,
              make_custom_alloc_handler(write_allocator_,
                std::bind(&session::handle_write, this,
					std::placeholders::_1,
					std::placeholders::_2))));
        socket_.async_read_some(asio::buffer(read_data_, block_size_),
            asio::bind_executor(strand_,
              make_custom_alloc_handler(read_allocator_,
                std::bind(&session::handle_read, this,
					std::placeholders::_1,
					std::placeholders::_2))));
      }
    }
  }

  void handle_write(const asio::error_code& err, size_t length)
  {
    if (!err && length > 0)
    {
      bytes_written_ += length;

      --unwritten_count_;
      if (unwritten_count_ == 1)
      {
        std::swap(read_data_, write_data_);
        async_write(socket_, asio::buffer(write_data_, read_data_length_),
            asio::bind_executor(strand_,
              make_custom_alloc_handler(write_allocator_,
                std::bind(&session::handle_write, this,
					std::placeholders::_1,
					std::placeholders::_2))));
        socket_.async_read_some(asio::buffer(read_data_, block_size_),
            asio::bind_executor(strand_,
              make_custom_alloc_handler(read_allocator_,
                std::bind(&session::handle_read, this,
					std::placeholders::_1,
					std::placeholders::_2))));
      }
    }
  }

  void close_socket()
  {
    socket_.close();
  }

private:
  asio::strand<asio::io_context::executor_type> strand_;
  asio::ip::tcp::socket socket_;
  size_t block_size_;
  char* read_data_;
  size_t read_data_length_;
  char* write_data_;
  int unwritten_count_;
  size_t bytes_written_;
  size_t bytes_read_;
  stats& stats_;
  handler_allocator read_allocator_;
  handler_allocator write_allocator_;
};

class client
{
public:
  client(asio::io_context& ioc,
      const asio::ip::tcp::resolver::results_type endpoints,
      size_t block_size, size_t session_count, int timeout)
    : io_context_(ioc),
      stop_timer_(ioc),
      sessions_(),
      stats_()
  {
    stop_timer_.expires_after(asio::chrono::seconds(timeout));
    stop_timer_.async_wait(std::bind(&client::handle_timeout, this));

    for (size_t i = 0; i < session_count; ++i)
    {
      session* new_session = new session(io_context_, block_size, stats_);
      new_session->start(endpoints);
      sessions_.push_back(new_session);
    }
  }

  ~client()
  {
    while (!sessions_.empty())
    {
      delete sessions_.front();
      sessions_.pop_front();
    }

    stats_.print();
  }

  void handle_timeout()
  {
    std::for_each(sessions_.begin(), sessions_.end(),
        std::mem_fn(&session::stop));
  }

private:
  asio::io_context& io_context_;
  asio::steady_timer stop_timer_;
  std::list<session*> sessions_;
  stats stats_;
};

int main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  try
  {
    using namespace std; // For atoi.
    const char* host = "127.0.0.1";
    const char* port = "8080";
    int thread_count = 1;
    size_t block_size = 1024;
    size_t session_count = 1;
    int timeout = 1000;

    asio::io_context ioc;

    asio::ip::tcp::resolver r(ioc);
    asio::ip::tcp::resolver::results_type endpoints =
      r.resolve(host, port);

    client c(ioc, endpoints, block_size, session_count, timeout);

    std::list<asio::thread*> threads;
    while (--thread_count > 0)
    {
		asio::thread* new_thread = new asio::thread([&ioc]()
		{
			ioc.run();
	  }
          /*std::bind(&asio::io_context::run, &ioc)*/);
      threads.push_back(new_thread);
    }

    ioc.run();

    while (!threads.empty())
    {
      threads.front()->join();
      delete threads.front();
      threads.pop_front();
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}