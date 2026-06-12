#include <array>

#include "ErasedManager.hpp"

class ManagerRegistry {
    static constexpr std::size_t MaxManagers = 32;
    static inline std::array<ErasedManager, MaxManagers> managers{};
    static inline std::size_t count = 0;

public:
    template<typename Mgr>
    static bool registerManager(Mgr& mgr) {
        static_assert(is_manager_v<Mgr>, "Type does not satisfy manager interface");
        if (count >= MaxManagers) return false;
        managers[count++] = makeErased(mgr);
        return true;
    }

    static void clearAll() {
        for (std::size_t i = 0; i < count; ++i) {
            if (managers[i].clear_func)
                managers[i].clear_func(managers[i].instance);
        }
    }

    static std::size_t totalObjects() {
        std::size_t total = 0;
        for (std::size_t i = 0; i < count; ++i) {
            if (managers[i].size_func)
                total += managers[i].size_func(managers[i].instance);
        }
        return total;
    }
};