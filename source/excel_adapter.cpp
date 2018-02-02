#include <QTextCodec>
#include <xlnt/xlnt.hpp>
#include "excel_adapter.h"


ExcelCell::ExcelCell(const std::string& text, bool header, bool empty, const std::string& background_color,
          const std::string& foreground_color)
    : m_text(text), isHeader(header), isEmpty(empty), background_color(background_color), foreground_color(foreground_color)
{
}

QString ExcelCell::cellText()
{
    if (isEmpty)
    {
        return QString();
    }
    QString string = QString::fromUtf8(QByteArray::fromStdString(m_text));
    return string;
}

std::string ExcelCell::text()
{
    return m_text;
}

ExcelAdapter::ExcelAdapter(xlnt::worksheet& ws) : m_ws(ws)
{ 
}

std::string colorToString(xlnt::color color)
{
    std::string color_string;
    if (color.type() == xlnt::color_type::indexed)
    {
        int color_index = color.indexed().index();
        color_string = std::to_string(color_index);
    }
    if (color.type() == xlnt::color_type::rgb)
    {
        color_string = color.rgb().hex_string();
    }

    return color_string;
}


ExcelCell ExcelAdapter::getExcelCell(QString cellNum) const
{
    xlnt::cell_reference reference(cellNum.toStdString());
    auto cell = m_ws.cell(reference);


    int color_index = -1;
    std::string foreground_color, background_color;
    if (cell.has_format())
    {
        xlnt::pattern_fill pat_fill = cell.fill().pattern_fill();

        auto color_forground = pat_fill.foreground(); // .background();
        if (color_forground.is_set())
        {
            foreground_color = colorToString(color_forground.get());
        }
        auto color_background = pat_fill.background();
        if (color_background.is_set())
        {
            background_color = colorToString(color_background.get());
        }
    }

    return ExcelCell(cell.to_string(), cell.is_merged(), !cell.has_value(), background_color, foreground_color);
}

int ExcelAdapter::countUsedRows() const
{
    return m_ws.highest_row();
}
