#include "dict_settings.hpp"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
Dict_settings::Dict_settings(QWidget *parent) :
    QDialog(parent),
    ui_{new Ui::Dict_settings}
{
    ui_->setupUi(this);
    connect(ui_->ChooseDictButton, &QPushButton::clicked,this,&Dict_settings::on_ChButton);
}

Dict_settings::~Dict_settings() = default;

void Dict_settings::update_list(){
    ui_->listWidget->clear();
    ui_->listWidget->addItems(*paths_);
}
void Dict_settings::on_RemovePathButton_clicked()
{
    if(!ui_->listWidget->count() || ui_->listWidget->currentRow()==-1) return;
    paths_->removeAt(ui_->listWidget->currentRow());
    update_list();
}

void Dict_settings::on_AddPathButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if(filename.length()>4){
        filename.remove(filename.length()-4,4);
        paths_->append(filename);
    }
    else QMessageBox::warning(this, "Error!", "Error in path");
    update_list();
}
void Dict_settings::on_ChButton(){
    if(!ui_->listWidget->count())
        QMessageBox::warning(this,"No Dictionary","No default dictionary");
    paths_->clear();
    for(int i=0;i<ui_->listWidget->count();i++)
        paths_->append(ui_->listWidget->item(i)->text());
    this->close();
}
