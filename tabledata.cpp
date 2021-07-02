#include "tabledata.h"

tableData::tableData()
    :QTableWidget()
{

}

tableData::~tableData()
{

}

void tableData::setTableHeader(QStringList header)
{
    this->setHorizontalHeaderLabels(header);
}
