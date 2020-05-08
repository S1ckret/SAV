#include "IDataRenderer.h"

#include <algorithm>
#include <thread>

#include "Log.h"
#include "glm/glm.hpp"
#include "Utility.h"

void IDataRenderer::SetData(Array * data) {
    m_array = data;

    auto max_value_it = std::max_element(m_array->Begin(), m_array->End());
    m_max_value = max_value_it->Data();

    // Position
    m_vbl.Push<float>(1);
    // Color
    m_vbl.Push<float>(3);
    
	m_vb.SetData(m_array->Begin(), m_array->Size() * sizeof(Column));
	m_va.AddBuffer(m_vb, m_vbl);

	m_shader.SetFilePath("res/shaders/New.shader");
	m_shader.Bind();
	m_shader.setUniform1ui("u_arr_size", m_array->Size());
	m_shader.setUniform1ui("u_arr_max_value", m_max_value);
}

void IDataRenderer::SetDelay(unsigned int ms) {
    m_delay = ms;
}

void IDataRenderer::RenderIterator(std::string name, unsigned int index, unsigned int color) {
    auto it = m_iterator_color.find(name);
    const auto & currentColumn = (*m_array)[index];
    if (it != m_iterator_color.end()) {
        // Restore previous color
        MarkColor(it->second.index, it->second.color);
        // Save current color and position
        it->second.color = ToHex(currentColumn.GetColor());
        it->second.index = index;
        // Mark with new color
        MarkColor(index, color);
    }
    else {
        // Save current color and position
        m_iterator_color.emplace(name, ColumnInfo{index, ToHex(currentColumn.GetColor())});
        // Mark with new color
        MarkColor(index, color);    
    }
    SleepFor(m_delay);
}

void IDataRenderer::RemoveIterator(std::string name) {
    auto it = m_iterator_color.find(name);
    if (it != m_iterator_color.end()) {
        MarkColor( it->second.index, it->second.color );
        m_iterator_color.erase(it);
        LOG_TRACE("Iterator has erased !");
    }
    else {
        LOG_ERROR("Error: Can not find \"{0}\" iterator !", name);
    }
}

void IDataRenderer::MarkColor(unsigned int index, unsigned int color) {
    (*m_array)[index] = ToRGB(color);
}

void IDataRenderer::MarkColorArea(unsigned int lhs_index, unsigned int rhs_index, unsigned int color) {
    // TODO: Debug flag
    assert(lhs_index < rhs_index);

    for (unsigned int i = lhs_index; i < rhs_index + 1; i++) {
        MarkColor(i, color);
    }    
}

void IDataRenderer::Draw() {
	m_vb.UpdateData(m_array->Begin(), m_array->Size() * sizeof(Column));
    Renderer::Draw(m_va, m_shader, m_array->Size());
}

void IDataRenderer::Reset() {
    m_iterator_color.clear();
    MarkColorArea(0, m_array->Size(), 0xffffff);
}

void IDataRenderer::SleepFor(unsigned int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
