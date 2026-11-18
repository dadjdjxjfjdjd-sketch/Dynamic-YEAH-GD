// Copyright (c) 2026 dadjdjxjfjdjd-sketch. SPDX-License-Identifier: MIT
// Compile the pinned engine unchanged; the task-owned adapter keeps the search loop out of Python.
#include "vendor/gdgen/sim/core/gdsim.cpp"
#include "Protocol.hpp"

namespace {
iax::State wire(gd::Player const& p) {
    std::uint32_t flags = p.mode;
    if (p.mini) flags |= iax::Mini;
    if (p.upside_down) flags |= iax::UpsideDown;
    if (p.on_ground) flags |= iax::Ground;
    if (p.held) flags |= iax::Held;
    return {p.x, p.y, p.y_vel, p.speed, flags};
}
}

// Return 1 only for a fully checked correction; 0=direct, 2=deadline, 3=unsupported, -1=error.
extern "C" int iax_predict(void* level, void* state, int horizon, int budgetUs, int lead, int collect,
                           iax::Decision* output) noexcept {
    if (!level || !state || !output || horizon < 1 || horizon > 400 || budgetUs < 500 || budgetUs > 20000
        || lead < 1 || lead > 16 || collect < 0 || collect > 1) return -1;
    try {
        using Timer = std::chrono::steady_clock;
        auto deadline = Timer::now() + std::chrono::microseconds(budgetUs);
        auto& layout = *static_cast<gd::Level*>(level);
        gd::Simulator simulator(layout);
        auto base = *static_cast<gd::Player*>(state);
        bool held = base.held;
        for (int i = 0; i < lead; ++i) base = simulator.step(base, held);
        if (!layout.unsupported.empty()) return 3;
        if (base.dead) return 0;
        std::array<float, 64> points{};
        std::uint32_t pointCount = 0;
        auto survives = [&](gd::Player branch, bool action, bool capture) {
            pointCount = 0;
            for (int i = 0; i < horizon; ++i) {
                if (Timer::now() >= deadline) return -1;
                branch = simulator.step(branch, action);
                if (!layout.unsupported.empty()) return -2;
                if (branch.dead) return 0;
                if (capture && i % ((horizon + 31) / 32) == 0 && pointCount < 32) {
                    points[pointCount * 2] = static_cast<float>(branch.x);
                    points[pointCount * 2 + 1] = static_cast<float>(branch.y);
                    ++pointCount;
                }
            }
            return 1;
        };
        int immediate = survives(base, !held, false);
        if (immediate != 0) return immediate == 1 ? 0 : immediate == -1 ? 2 : 3;
        auto delayed = base;
        for (int delay = 1; delay <= horizon; ++delay) {
            if (Timer::now() >= deadline) return 2;
            delayed = simulator.step(delayed, held);
            if (!layout.unsupported.empty()) return 3;
            if (delayed.dead) return 0;
            int safe = survives(delayed, !held, collect != 0);
            if (safe < 0) return safe == -1 ? 2 : 3;
            if (safe == 1) {
                iax::Decision result;
                result.expected = wire(base); result.due = wire(delayed);
                result.delay = delay; result.down = unsigned(!held); result.pointCount = pointCount;
                result.points = points;
                *output = result;
                return 1;
            }
        }
        return 0;
    } catch (...) { return -1; }
}
