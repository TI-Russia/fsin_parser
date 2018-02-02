#pragma once
#include <memory>
#include <QString>

namespace xlnt {
    class workbook;
    class worksheet;
    class cell;
};

class ExcelCell
{
public:
    ExcelCell(const std::string& text, bool header, bool empty, const std::string& background_color,
              const std::string& foreground_color);
    bool isHeader;
    bool isEmpty;
    std::string background_color;
    std::string foreground_color;

    QString cellText();
    std::string text();
private:
    std::string m_text;
};

class ExcelAdapter
{
public:
    ExcelAdapter(xlnt::worksheet& ws);

    ExcelCell getExcelCell(QString cellNum) const;
    int countUsedRows() const;
private:
     xlnt::worksheet& m_ws;
};

