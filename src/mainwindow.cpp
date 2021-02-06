#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QTextStream>

#include <nuspell/dictionary.hxx>
#include <nuspell/finder.hxx>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow{parent},
    ui_{new Ui::MainWindow}
{
    ui_->setupUi(this);
    cur_pos=0;
    connect(ui_->actionQuit,&QAction::triggered,qApp,&QApplication::quit);
    connect(ui_->actionOpen_File, &QAction::triggered,this,&MainWindow::on_openAct);
    connect(ui_->actionSave_File, &QAction::triggered,this,&MainWindow::on_saveAct);
    connect(ui_->actionSave_As, &QAction::triggered,this,&MainWindow::on_saveAsAct);
    connect(ui_->actionStart, &QAction::triggered,this,&MainWindow::on_startAct);
    connect(ui_->actionPath_to_dictionary,&QAction::triggered,this,&MainWindow::on_settAct);
    connect(ui_->NextButton, &QPushButton::clicked,this,&MainWindow::on_nextAct);
    connect(ui_->ReplaceButton, &QPushButton::clicked,this,&MainWindow::on_replaceAct);
    connect(ui_->StopButton, &QPushButton::clicked,this,&MainWindow::on_stopAct);
    ui_->SPELL_WIDGET->hide();
}

MainWindow::~MainWindow() = default;
void MainWindow::on_settAct(){
    filename = QFileDialog::getOpenFileName(this);
    if(!filename.isEmpty()){
        dict_path=filename.toStdString();
    }
    else QMessageBox::warning(this, "Error!", "Error in path");
}
void MainWindow::on_openAct()
{

    filename = QFileDialog::getOpenFileName(this);
    if(!filename.isEmpty()){
        QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "Error!",
                                 file.errorString());
            return;
        }
        QTextStream in(&file);
        ui_->plainTextEdit->setPlainText(in.readAll());

    }
}

void MainWindow::save_file(QString name){
    QFile file(name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error!",
                             file.errorString());
        return;
    }

    QTextStream out(&file);
    out << ui_->plainTextEdit->toPlainText();
}
void MainWindow::on_saveAct(){
    if (filename.isEmpty()) {
        return on_saveAsAct();
    } else {
        return save_file(filename);
    };
}
void MainWindow::on_saveAsAct(){
    QString FN = QFileDialog::getSaveFileName(this);
    if (FN.isEmpty()) return;
    return save_file(FN);
}



void MainWindow::on_startAct(){
    ui_->SPELL_WIDGET->show();
    ui_->listWidget->clear();
    if(!dict_path.length()){
        std::vector<std::pair<std::string, std::string>> dict_list;
        nuspell::search_default_dirs_for_dicts(dict_list);
        auto dict_name_and_path = nuspell::find_dictionary(dict_list, "en_US");
        if (dict_name_and_path == std::end(dict_list)) {
            QMessageBox::warning(this,"Error","No default dictionary");
            return;
        }
        this->dict_path = dict_name_and_path->second;
    }
    try{
        dict = nuspell::Dictionary::load_from_path(dict_path);
    }
    catch(nuspell::Dictionary_Loading_Error& err){
        QMessageBox::warning(this,"Error","No Dictionary");
        return;
    }

    Word_list = ui_->plainTextEdit->toPlainText().split(QRegExp("\\W+"), QString::SkipEmptyParts);
    cur_pos=-1;
    return on_nextAct();
}
void MainWindow::on_nextAct(){
    ui_->listWidget->clear();
    cur_pos++;
    if(cur_pos>=Word_list.size()) cur_pos=0;
    if(Word_list.size()==0) return;
    auto sugs = std::vector<std::string>();
    for (int i=cur_pos;i<Word_list.size();i++) {
        auto Word=Word_list[cur_pos];
        if (!dict.spell(Word.toStdString())){
            QTextCursor text_cursor = QTextCursor(ui_->plainTextEdit->document()->find(Word_list[cur_pos]));
            QString r_="<span style='background-color: red;'>"+Word+"</p>";
            text_cursor.insertHtml(r_);
            dict.suggest(Word.toStdString(), sugs);
            if (!sugs.empty()){
                for(auto i : sugs){
                    QString i_(i.data());
                    ui_->listWidget->addItem(i_);
                }
            }
            break;
        }
        else cur_pos++;
    }
}

void MainWindow::on_replaceAct(){
    if(!ui_->listWidget->count() || ui_->listWidget->currentRow()==-1) return;
    QString r = ui_->listWidget->currentItem()->text();
    QString r_="<span style='background-color: none;'>"+r+"</p>";
    QTextCursor text_cursor = QTextCursor(ui_->plainTextEdit->document()->find(Word_list[cur_pos]));
    text_cursor.insertHtml(r_);
    Word_list[cur_pos]="";
    return on_nextAct();

}
void MainWindow::on_stopAct(){
    ui_->listWidget->clear();
    Word_list.clear();
    ui_->SPELL_WIDGET->hide();
}
