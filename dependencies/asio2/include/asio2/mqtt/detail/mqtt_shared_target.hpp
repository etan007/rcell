/*
 * COPYRIGHT (C) 2017-2021, zhllxt
 *
 * author   : zhllxt
 * email    : 37792738@qq.com
 * 
 * Distributed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * (See accompanying file LICENSE or see <http://www.gnu.org/licenses/>)
 * 
 * refrenced from : mqtt_cpp/include/mqtt/broker/shared_target.hpp
 */

#ifndef __ASIO2_MQTT_SHARED_TARGET_HPP__
#define __ASIO2_MQTT_SHARED_TARGET_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <optional>
#include <thread>
#include <map>
#include <unordered_map>
#include <chrono>
#include <set>
#include <vector>

#include <asio2/mqtt/detail/mqtt_topic_util.hpp>

namespace asio2::mqtt
{
	// v is shared_entry
	template<typename Value>
	typename std::add_pointer_t<typename asio2::detail::remove_cvref_t<Value>::session_type>
	round_shared_target_method(Value& v)
	{
		using session_type = typename asio2::detail::remove_cvref_t<Value>::session_type;

		session_type* session = nullptr;

		if (!v.session_map.empty())
		{
			if (v.last == v.session_map.end())
				v.last = v.session_map.begin();
			else
			{
				v.last = std::next(v.last);

				if (v.last == v.session_map.end())
					v.last = v.session_map.begin();
			}

			session = v.last->second;
		}

		return session;
	}

	template<typename Value>
	class shared_target
	{
	public:
		shared_target()
		{
			set_policy(std::bind(round_shared_target_method<Value>, std::placeholders::_1));
		}
		~shared_target() = default;

		using session_type = typename Value::session_type;

		template<class Function>
		inline shared_target& set_policy(Function&& fun)
		{
			policy_ = std::forward<Function>(fun);
			return (*this);
		}

	public:
		void insert(session_type* session, std::string_view share_name, std::string_view topic_filter)
		{
			auto key = std::pair{ share_name, topic_filter };

			// std::map<std::chrono::nanoseconds::rep, session_t*> session_map;

			auto it = targets_.find(key);
			if (it == targets_.end())
			{
				Value v{ share_name, topic_filter };

				auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
					std::chrono::steady_clock::now().time_since_epoch()).count();

				session->shared_target_key_ = ns;

				v.session_map.emplace(ns, session);

				v.session_set.emplace(session);

				key = std::pair{ v.share_name_view(), v.topic_filter_view() };

				it = targets_.emplace(std::move(key), std::move(v)).first;
			}
			else
			{
				Value& v = it->second;

				if (v.session_set.find(session) != v.session_set.end())
					return;

				for (;;)
				{
					auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
						std::chrono::steady_clock::now().time_since_epoch()).count();

					auto it_map = v.session_map.find(ns);
					if (it_map != v.session_map.end())
					{
						std::this_thread::yield();
						continue;
					}

					session->shared_target_key_ = ns;

					v.session_map.emplace(ns, session);

					v.session_set.emplace(session);

					break;
				}
			}
		}
		void erase(session_type* session, std::string_view share_name, std::string_view topic_filter)
		{
			auto key = std::pair{ share_name, topic_filter };

			auto it = targets_.find(key);
			if (it == targets_.end())
				return;

			Value& v = it->second;

			auto it_map = v.session_map.find(session->shared_target_key_);
			if (it_map != v.session_map.end())
			{
				if (v.last == it_map)
				{
					if (it_map != v.session_map.begin())
						v.last = std::prev(v.last);
					else
						v.last = std::next(v.last);
				}

				v.session_map.erase(it_map);
			}

			auto it_set = v.session_set.find(session);
			if (it_set != v.session_set.end())
			{
				v.session_set.erase(it_set);
			}
		}
		//void erase(session_type* session, std::set<std::string_view> share_names)
		//{
		//	for (std::string_view share_name : share_names)
		//	{
		//		auto it = targets_.find(share_name);
		//		if (it == targets_.end())
		//			continue;

		//		std::unordered_map<std::string_view, entry>& map_inner = it->second;
		//		auto it_map = map_inner.find(session->client_id());
		//		if (it_map == map_inner.end())
		//			continue;

		//		map_inner.erase(it_map);
		//	}
		//}
		session_type* get_target(std::string_view share_name, std::string_view topic_filter)
		{
			auto key = std::pair{ share_name, topic_filter };

			auto it = targets_.find(key);
			if (it == targets_.end())
				return nullptr;

			Value& v = it->second;

			return policy_(v);
		}

	protected:
		/// key : share_name - topic_filter, val : shared_entry
		std::unordered_map<std::pair<std::string_view, std::string_view>, Value> targets_;

		std::function<session_type*(Value&)> policy_;
	};

	template<class session_t>
	struct shared_entry
	{
		template <class> friend class mqtt::shared_target;

		using session_type = session_t;

		explicit shared_entry(std::string_view _share_name, std::string_view _topic_filter)
		{
			share_name.resize(_share_name.size());
			std::memcpy((void*)share_name.data(), (const void*)_share_name.data(), _share_name.size());

			topic_filter.resize(_topic_filter.size());
			std::memcpy((void*)topic_filter.data(), (const void*)_topic_filter.data(), _topic_filter.size());

			last = session_map.end();
		}

		inline std::string_view share_name_view()
		{
			return std::string_view{ share_name.data(), share_name.size() };
		}

		inline std::string_view topic_filter_view()
		{
			return std::string_view{ topic_filter.data(), topic_filter.size() };
		}

		std::vector<char> share_name  ; // vector has no SSO
		std::vector<char> topic_filter;

		/// session map ordered by steady_clock
		std::map<std::chrono::nanoseconds::rep, session_t*> session_map;

		/// session unique
		std::set<session_t*> session_set;

		/// last session for shared subscribe
		typename std::map<std::chrono::nanoseconds::rep, session_t*>::iterator last;
	};
}

#endif // !__ASIO2_MQTT_SHARED_TARGET_HPP__
