#include "tabledata.h"

tableData::tableData()
    : QTableWidget()
{
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
}

tableData::~tableData()
{
}

void tableData::setTableHeader(QStringList header)
{
    this->setHorizontalHeaderLabels(header);
}
