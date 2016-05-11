#include "hnmptablewidget.h"
#include "ui_hnmptablewidget.h"
#include "HNDefine.h"
#include "hngui.h"
#include "hnobjectfactory.h"

HNMPTableWidget::HNMPTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNMPTableWidget)
{
    ui->setupUi(this);

    m_db = newDatabaseConn();

    m_numPerPage = 14;
    selectionMode = QAbstractItemView::MultiSelection;
    altColor = true;
    resizeMode = QHeaderView::ResizeToContents;

    ui->btnJump->setFixedWidth(30);
    ui->btnLeft->setFixedWidth(30);
    ui->btnRight->setFixedWidth(30);
    ui->btnLeftHead->setFixedWidth(30);
    ui->btnRightHead->setFixedWidth(30);
    ui->leNum->setFixedWidth(40);
    ui->btnJump->setHidden(true);
    ui->leNum->setHidden(true);
    ui->btnRightHead->setHidden(true);
    ui->btnRight->setHidden(true);
    ui->btnLeftHead->setHidden(true);
    ui->btnLeft->setHidden(true);

}

HNMPTableWidget::~HNMPTableWidget()
{
    delete ui;
}

void HNMPTableWidget::setDB(QString db)
{
    m_name = db;
    setDatabaseName(m_db, m_name);
}

void HNMPTableWidget::setTable(QString table)
{
    m_table = table;
}

void HNMPTableWidget::query(QString filter)
{
    ptime(); //3ms

    QSqlQuery query(m_db);
    query.exec(QString("select count(*) from %1").arg(m_table));

    int num = 0;
    if(query.next())
    {
        num = query.value(0).toInt();
        //pline() << "record num:" << num;
    }
    query.finish();

    ptime();//11ms
    while(ui->stWidgetPage->count())
        ui->stWidgetPage->removeWidget(ui->stWidgetPage->widget(0));
    ptime();

    int pageNum = 0;
    if(num%m_numPerPage>0)
        pageNum = num / m_numPerPage+1;
    else
        pageNum = num / m_numPerPage;

    for(int i = 0; i < pageNum; i++)
    {
        ptime();//89ms
        HNTableWidget* page = new HNTableWidget(this);
        ptime();//2ms
        page->setDB(m_name);
        ptime();//8ms
        page->setTable(m_table);
        ptime();//14ms
        page->query(QString("%1 limit %2 offset %3")
                    .arg(filter)
                    .arg(m_numPerPage)
                    .arg(i*m_numPerPage));
        ptime();//3ms
        QAbstractItemModel* m_model = page->model();
        for(int i = 0; i < m_model->columnCount(); i++)
            m_model->setHeaderData(
                        i, Qt::Horizontal,
                        m_headerData.value(i, m_model->headerData(i, Qt::Horizontal).toString()));
        ptime();//1ms
        page->setSelectionMode(selectionMode);
        page->setAlternatingRowColors(altColor);
        page->horizontalHeader()->setResizeMode(resizeMode);
        ptime();//0ms
        for(int i = 0; i < m_model->columnCount(); i++)
            page->horizontalHeader()->setResizeMode(i, m_resizeMode.value(i, resizeMode));
        ptime();//QHash(338ms) QMap(372ms) 400ms(QHash等几乎不耗时)
        for(int i = 0; i < m_model->columnCount(); i++)
            page->setColumnWidth(i, m_columnWidth.value(i));
        ptime();//219ms
        ui->stWidgetPage->addWidget(page);
        ptime();
    }
}

int HNMPTableWidget::pageNum()
{
    return ui->stWidgetPage->count();
}

int HNMPTableWidget::currentPage()
{
    return ui->stWidgetPage->currentIndex()+1;
}

void HNMPTableWidget::setCurrentPage(int index)
{
    if(index < 1 || index > ui->stWidgetPage->count())
        return;
    ui->stWidgetPage->setCurrentIndex(index-1);
    ui->lbPos->setText(QString("%1/%2").arg(index).arg(ui->stWidgetPage->count()));
}

void HNMPTableWidget::setRecordNumPerPage(int num)
{
    m_numPerPage = num;
}

void HNMPTableWidget::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    selectionMode = mode;
}

void HNMPTableWidget::setResizeMode(QHeaderView::ResizeMode mode)
{
    resizeMode = mode;
}

void HNMPTableWidget::setResizeMode(int column, QHeaderView::ResizeMode mode)
{
    m_resizeMode.insert(column, mode);
}

void HNMPTableWidget::setAlternatingRowColors(bool alt)
{
    altColor = alt;
}

void HNMPTableWidget::setHeaderData(int i, Qt::Orientation, QString data)
{
    m_headerData.insert(i, data);
}

void HNMPTableWidget::setColumnWidth(int column, int width)
{
    m_columnWidth.insert(column, width);
}

void HNMPTableWidget::on_btnLeft_clicked()
{
    int index = ui->stWidgetPage->currentIndex();
    if(index > 0)
        index--;
    ui->stWidgetPage->setCurrentIndex(index);
    ui->lbPos->setText(QString("%1/%2").arg(index+1).arg(ui->stWidgetPage->count()));
}

void HNMPTableWidget::on_btnRight_clicked()
{
    int index = ui->stWidgetPage->currentIndex();
    if(index < ui->stWidgetPage->count()-1)
        index++;
    ui->stWidgetPage->setCurrentIndex(index);
    ui->lbPos->setText(QString("%1/%2").arg(index+1).arg(ui->stWidgetPage->count()));
}

void HNMPTableWidget::on_btnJump_clicked()
{
    int num = ui->leNum->text().toInt();
    if(num <= ui->stWidgetPage->count() && num > 0)
    {
        ui->stWidgetPage->setCurrentIndex(num-1);
        ui->lbPos->setText(QString("%1/%2").arg(num).arg(ui->stWidgetPage->count()));
    }
}

void HNMPTableWidget::on_btnLeftHead_clicked()
{
    ui->stWidgetPage->setCurrentIndex(0);
    ui->lbPos->setText(QString("%1/%2").arg(1).arg(ui->stWidgetPage->count()));
}

void HNMPTableWidget::on_btnRightHead_clicked()
{
    int index = ui->stWidgetPage->count()-1;
    ui->stWidgetPage->setCurrentIndex(index);
    ui->lbPos->setText(QString("%1/%2").arg(index+1).arg(ui->stWidgetPage->count()));
}


void HNMPTableWidget::selectedRows(int column, QVector<QStringList> &strl)
{
    for(int i = 0; i < ui->stWidgetPage->count(); i++)
    {
        HNTableWidget* page = (HNTableWidget*)ui->stWidgetPage->widget(i);
        QMap<int, QStringList> ids;
        page->selectedRows(column, ids);
        QMapIterator<int, QStringList> itor(ids);
        while (itor.hasNext()) {
            itor.next();
            strl.append(itor.value());
        }
    }
    return;
}

HNTableWidget* HNMPTableWidget::selectedRows(int column)
{
    QString sectionName;
    QSqlQuery query(m_db);
    query.exec(QString("select * from %1 limit 0").arg(m_table));
    sectionName = query.record().fieldName(column);
    query.finish();
    pline() << sectionName;

    QVector<QStringList> lid;
    selectedRows(column, lid);

    static HNTableWidget* page = new HNTableWidget(this);
    page->setDB(m_name);
    page->setTable(m_table);

    QString excp;
    for(int i = 0; i < lid.count() - 1; i++)
        excp += QString("%1 = '%2' or ").arg(sectionName).arg(lid[i].at(column));
    excp += QString("%1 = '%2'").arg(sectionName).arg(lid.last().at(column));
    page->query(excp);

    QAbstractItemModel* m_model = page->model();
    for(int i = 0; i < m_model->columnCount(); i++)
        m_model->setHeaderData(
                    i, Qt::Horizontal,
                    m_headerData.value(i, m_model->headerData(i, Qt::Horizontal).toString()));
    page->setSelectionMode(selectionMode);
    page->setAlternatingRowColors(altColor);
    page->horizontalHeader()->setResizeMode(resizeMode);
    for(int i = 0; i < m_model->columnCount(); i++)
        page->horizontalHeader()->setResizeMode(i, m_resizeMode.value(i, resizeMode));
    for(int i = 0; i < m_model->columnCount(); i++)
        page->setColumnWidth(i, m_columnWidth.value(i));
    return page;
}


void HNMPTableWidget::removeSelectedRows(int column)
{
    for(int i = 0; i < ui->stWidgetPage->count(); i++)
    {
        HNTableWidget* page = (HNTableWidget*)ui->stWidgetPage->widget(i);
        QMap<int, QStringList> ids;
        page->selectedRows(column, ids);
        page->removeRows(column, ids.values());
    }
}
