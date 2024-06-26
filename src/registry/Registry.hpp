#pragma once

#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <memory>
#include <vector>
#include "conf/Config.hpp"
#include "display/Screen.hpp"
#include "fractional-scale-v1-protocol.h"
#include "input/Seat.hpp"
#include "registry/BufferPool.hpp"
#include "registry/Compositor.hpp"
#include "registry/LayerShell.hpp"
#include "registry/Lock.hpp"
#include "registry/LockManager.hpp"
#include "registry/Subcompositor.hpp"
#include "registry/XdgBase.hpp"
#include "util/Loop.hpp"
#include "viewporter-protocol.h"

namespace wall {
class Display;
class XdgBase;
class Registry {
   public:
    explicit Registry(const Config& config, Display* display, Loop* loop, wl_registry* registry);
    virtual ~Registry();

    Registry(Registry&&) = delete;
    Registry(const Registry&) = delete;
    auto operator=(const Registry&) -> Registry = delete;
    auto operator=(Registry&&) -> Registry = delete;

    [[nodiscard]] auto get_lock() -> std::unique_ptr<Lock>;

    [[nodiscard]] auto get_screens() const -> const std::vector<std::unique_ptr<wall::Screen>>&;

    [[nodiscard]] auto get_screens_mut() -> std::vector<std::unique_ptr<wall::Screen>>*;

    [[nodiscard]] auto get_compositor() const -> Compositor* { return m_compositor.get(); }

    [[nodiscard]] auto get_subcompositor() const -> Subcompositor* { return m_subcompositor.get(); }

    [[nodiscard]] virtual auto get_buffer_pool() const -> const BufferPool& { return *m_buffer_pool; }

    [[nodiscard]] virtual auto get_buffer_pool_mut() -> BufferPool* { return m_buffer_pool.get(); }

    [[nodiscard]] virtual auto get_seat() const -> const Seat& { return *m_seat; }

    [[nodiscard]] virtual auto get_seat_mut() -> Seat* { return m_seat.get(); }

    [[nodiscard]] auto get_lock_manager() const -> LockManager* { return m_lock_manager.get(); }

    [[nodiscard]] auto get_xdg_base() const -> const XdgBase& { return *m_xdg_wm_base; }

    [[nodiscard]] auto get_layer_shell() const -> LayerShell* { return m_layer_shell.get(); }

    [[nodiscard]] auto get_display() const -> Display* { return m_display; }

    [[nodiscard]] auto get_fractional_scale_manager() const -> wp_fractional_scale_manager_v1* { return m_fractional_scale_manager; }

    [[nodiscard]] auto get_viewporter() const -> wp_viewporter* { return m_viewporter; }

    auto destory_screens() -> void;

    auto move_screens() -> std::vector<std::unique_ptr<wall::Screen>>;

   protected:
    auto on_global(uint32_t name, const char* interface, uint32_t version) -> void;

    auto on_global_remove(uint32_t name) -> void;

    [[nodiscard]] auto get_config() const -> const Config&;

    [[nodiscard]] auto get_loop() const -> Loop*;

   private:
    static const wl_registry_listener k_listener;

    const Config& m_config;

    Display* m_display{};

    Loop* m_loop{};

    wl_registry* m_registry{};

    wp_fractional_scale_manager_v1* m_fractional_scale_manager{};

    wp_viewporter* m_viewporter{};

    std::unique_ptr<Compositor> m_compositor{};

    std::unique_ptr<Subcompositor> m_subcompositor{};

    std::unique_ptr<LayerShell> m_layer_shell{};

    std::unique_ptr<LockManager> m_lock_manager{};

    std::unique_ptr<XdgBase> m_xdg_wm_base{};

    std::unique_ptr<BufferPool> m_buffer_pool{};

    std::unique_ptr<Seat> m_seat;

    std::vector<std::unique_ptr<wall::Screen>> m_screens;
};
}  // namespace wall
