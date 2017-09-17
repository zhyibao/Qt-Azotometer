#include "qqtlocaldbdialog.h"
#include "qqtmsgbox.h"

QQTLocalDBDialog::QQTLocalDBDialog(QWidget *parent) :
    QQTDialog(parent)
{
    localWidget = new QQTTreeWidget(this);
    btnSelect = new QQTPushButton(this);

    connect(localWidget->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentRowChanged()));
    connect(btnSelect, SIGNAL(clicked()),
            this, SLOT(selected()));

    InitOCX();
}

void QQTLocalDBDialog::InitOCX()
{
    setFocusPolicy(Qt::ClickFocus);
    setFocus();

    int width = 600;
    int height = 420;

    setGeometry(89,90, width, height);
    moveCenter(this);

    int btnH = 44;
    int btnW = 108;
    int btnX = width / 2 - btnW / 2;
    int btnXS = btnW + 40;
    int btnY = height - 120 + btnH;

    btnSelect->setFlat(true);
    btnSelect->setFocusPolicy(Qt::NoFocus);
    btnSelect->setGeometry(btnX + btnXS * 0, btnY, 108,44);
    btnSelect->iconTable().initNormal("./skin/default/bt_login_normal.png",
                                    "./skin/default/bt_login_press.png");

    localWidget->setGeometry(20, 20, width-40, height - 120);
}

bool QQTLocalDBDialog::query(QString path)
{
    localWidget->setNameFilter("*");
    localWidget->setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    localWidget->setSorting();;
    localWidget->query(path);
    localWidget->setNameFilter("*.db");
    localWidget->setFilter();
}

void QQTLocalDBDialog::selectedItem(QString &path, QString &name)
{
    path = m_path;
    name = m_name;
}

void QQTLocalDBDialog::currentRowChanged()
{
    QModelIndex curIndex = localWidget->currentIndex();
    QModelIndex parIndex = curIndex.parent();

    if(!curIndex.isValid())
        return;

    if(!parIndex.isValid())
        return;

    m_path = localWidget->model()->index(curIndex.row(), FILE_PATH, parIndex).data().toString();
    m_name = localWidget->model()->index(curIndex.row(), FILE_NAME, parIndex).data().toString();

}

void QQTLocalDBDialog::selected()
{
    if(m_path.isEmpty() || m_name.isEmpty())
    {
        QQTMsgBox::warning(this, "请选择需要备份的数据库");
        return;
    }

    QModelIndex curIndex = localWidget->currentIndex();
    QModelIndex parIndex = curIndex.parent();


    if(!curIndex.isValid() || !parIndex.isValid())
    {
        QQTMsgBox::warning(this, "请选择需要备份的数据库");
        return;
    }


    accept();
}
