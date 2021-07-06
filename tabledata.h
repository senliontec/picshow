#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <QObject>
#include <QTableWidget>

class tableData : public QTableWidget
{
    Q_OBJECT
public:
    tableData();
    ~tableData();

    void setTableHeader(QStringList header);
};

#endif // TABLEDATA_H
