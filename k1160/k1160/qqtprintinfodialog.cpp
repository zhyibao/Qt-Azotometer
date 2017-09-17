#include "qqtprintinfodialog.h"
#include "ui_qqtprintinfodialog.h"
#include "qqtdefine.h"

QQTPrintInfoDialog::QQTPrintInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QQTPrintInfoDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, false);
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->btnPrint, SIGNAL(clicked()), this, SLOT(accept()));
}

QQTPrintInfoDialog::~QQTPrintInfoDialog()
{
    delete ui;
}

void QQTPrintInfoDialog::initAll()
{
    //BK
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground);
    this->setWindowModality(Qt::WindowModal);

    this->setGeometry(0, 0, 805, 325);
    moveCenter(this);
    this->setStyleSheet("QPrinterForm{image:url(./skin/default/bk_printer.png)}""QPrinterForm{background-color:transparent;}");

    //edit
    ui->leTitle->setGeometry(30,24,750, 43);
    ui->leTitle->setStyleSheet("QLineEdit{background-color:transparent;}""QLineEdit{background-image: url(./skin/default/bt_le_sobig.png);font-size:17px}");
    ui->leTitle->setAlignment(Qt::AlignCenter);
    ui->leHeader->setGeometry(30, 79, 750, 43);
    ui->leHeader->setStyleSheet("QLineEdit{background-color:transparent;}""QLineEdit{background-image: url(./skin/default/bt_le_sobig.png);font-size:17px}");
    ui->leHeader->setAlignment(Qt::AlignCenter);

    ui->leFooter->setGeometry(30, 134, 750, 43);
    ui->leFooter->setStyleSheet("QLineEdit{background-color:transparent;}""QLineEdit{background-image: url(./skin/default/bt_le_sobig.png);font-size:17px}");
    ui->leFooter->setAlignment(Qt::AlignCenter);

    //bt
    ui->btnCancel->setFlat(true);
    //ui->pushButton_back->setFocusPolicy(Qt::NoFocus);
    ui->btnCancel->setGeometry(505,250,144,48);
    ui->btnCancel->setStyleSheet("QPushButton{background-color:transparent;background-image: url(./skin/default/bt_back_big_normal.png)}""QPushButton:hover{background-image: url(./skin/default/bt_back_big_normal.png);}""QPushButton:pressed{background-image: url(./skin/default/bt_back_big_press.png);}");
    ui->btnCancel->setText("");
    ui->btnPrint->setFlat(true);
    //ui->pushButton_start->setFocusPolicy(Qt::NoFocus);
    ui->btnPrint->setGeometry(652,250,144,48);
    ui->btnPrint->setStyleSheet("QPushButton{background-color:transparent;background-image: url(./skin/default/bt_start_normal.png)}""QPushButton:hover{background-image: url(./skin/default/bt_start_normal.png);}""QPushButton:pressed{background-image: url(./skin/default/bt_start_press.png);}");
    ui->btnPrint->setText("");
}

void QQTPrintInfoDialog::printInfo(QString &title, QString &header, QString &footer)
{
    if(!ui->leTitle->text().isEmpty())
        title = ui->leTitle->text();
    if(!ui->leHeader->text().isEmpty())
        header = ui->leHeader->text();
    if(!ui->leFooter->text().isEmpty())
        footer = ui->leFooter->text();
}
