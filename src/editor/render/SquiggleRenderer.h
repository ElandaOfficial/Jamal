/**
    ===============================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any internal version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.

    Copyright (c) 2021 ElandaSunshine
    ===============================================================

    @author Elanda
    @file   SquiggleRenderer.cpp
    @date   10, January 2021

    ===============================================================
 */

#pragma once

#include <juce_graphics/juce_graphics.h>

struct SquiggleRendererWaved
{
    static void render(juce::Graphics &g, juce::Rectangle<float> bounds)
    {
        const float line_bottom = bounds.getBottom();
        const float line_top    = line_bottom - 3;
        const int   width       = static_cast<int>(bounds.getWidth());
        
        juce::Path path;
        
        for (int x_inc = 0; x_inc < width; x_inc += 12)
        {
            const float x = bounds.getX() + static_cast<float>(x_inc);
            path.addLineSegment({ x,     line_bottom, x +  3, line_top    }, 1.0f);
            path.addLineSegment({ x + 3, line_top,    x +  6, line_bottom }, 1.0f);
            path.addLineSegment({ x + 6, line_bottom, x +  9, line_top    }, 1.0f);
            path.addLineSegment({ x + 9, line_top,    x + 12, line_bottom }, 1.0f);
        }
        
        g.fillPath(path);
    }
};

struct SquiggleRendererBordered
{
    static void render(juce::Graphics &g, juce::Rectangle<float> bounds)
    {
        g.drawRect(bounds);
    }
};

struct SquiggleRendererDotted
{
    static void render(juce::Graphics &g, juce::Rectangle<float> bounds)
    {
        const float line_bottom = bounds.getBottom();
        static constexpr std::array<float, 2> dashes { 2.0f, 4.0f };
        g.drawDashedLine({ bounds.getX(), line_bottom, bounds.getRight(), line_bottom }, dashes.data(), 2, 2.0f);
    }
};

struct SquiggleRendererUnderlined
{
    static void render(juce::Graphics &g, juce::Rectangle<float> bounds)
    {
        g.fillRect(bounds.removeFromBottom(1));
    }
};
