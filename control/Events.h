#pragma once

#include <vector>
#include <functional>
#include <utility>
#include <type_traits>

namespace Ath::Control
{
    template <typename T>
    class EventOutput
    {
        using callbackType = std::function<void(const T&)>;

        std::vector<callbackType> callbacks;

    public:

        void fire(const T& value) const
        {
            for (const auto& callback : callbacks) callback(value);
        }

        template <typename F>
        void addCallback(F&& f)
        {
            static_assert(
                std::is_invocable_v<F, const T&>,
                "Callback must be callable with (const T&)"
            );

            callbacks.emplace_back(std::forward<F>(f));
        }

        template <typename InstanceType, typename MemberFunction>
        void addMemberCallback(InstanceType& instance, MemberFunction member_function)
        {
            addCallback([&instance, member_function](const T& v) {
                std::invoke(member_function, instance, v);
            });
        }
    };
}
