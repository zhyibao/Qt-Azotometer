#ifndef HNUSERINFOMODEL_H
#define HNUSERINFOMODEL_H

#include "hnsqltablemodel.h"

class HNUserInfoModel : public HNSqlTableModel
{
    Q_OBJECT
public:
    explicit HNUserInfoModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

signals:

public slots:

};

#endif // HNUSERINFOMODEL_H
